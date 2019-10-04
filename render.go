package main

import (
	"fmt"
	"image"
	"image/color"
	"image/draw"
	"image/png"
	"log"
	"math"
	"os"
	"strings"

	"github.com/golang/freetype"
	"github.com/golang/freetype/truetype"
	"github.com/tajtiattila/beermap/googlefont"
	"github.com/tajtiattila/qmk-keymaps/renderkbd/keymapc"
	"golang.org/x/image/font"
)

type renderer struct {
	conf   Config
	keymap []keymapc.Keymap

	font *truetype.Font

	sx, sy int // top left of source keymaps

	im *image.RGBA
}

func render(conf Config, src []keymapc.Keymap) error {
	r := renderer{
		conf:   conf,
		keymap: src,
	}

	if err := r.loadFont(); err != nil {
		return err
	}

	r.initImage()

	r.drawKeyShapes(r.mainKeymap())

	for _, l := range conf.Label {
		m, ok := r.findKeymap(l.Layer)
		if !ok {
			log.Println("Missing layer:", l.Layer)
		}

		r.drawKeyLabels(l, m)
	}

	f, err := os.Create(conf.Render.Image)
	if err != nil {
		return fmt.Errorf("Create image file: %w", err)
	}
	defer f.Close()

	return png.Encode(f, r.im)
}

func (r *renderer) loadFont() error {
	fc := r.conf.Font
	fontdata, err := googlefont.Get(fc.Family, fc.Weight)
	if err != nil {
		return fmt.Errorf("Get font: %w", err)
	}

	r.font, err = freetype.ParseFont(fontdata)
	if err != nil {
		return fmt.Errorf("Parse font: %w", err)
	}
	return nil
}

func (r *renderer) initImage() {
	xmi, xma := math.MaxInt32, 0
	ymi, yma := math.MaxInt32, 0

	for _, m := range r.keymap {
		for _, k := range m.Keys {
			l, t := k.X, k.Y
			r, b := k.X+k.Dx, k.Y+k.Dy
			if l < xmi {
				xmi = l
			}
			if t < ymi {
				ymi = t
			}
			if r > xma {
				xma = r
			}
			if b > yma {
				yma = b
			}
		}
	}

	r.sx = xmi
	r.sy = ymi

	rc := r.conf.Render
	sc := r.conf.Source

	dx := 2*rc.ImageBorder + (xma-xmi)*rc.Dx/sc.HScale
	dy := 2*rc.ImageBorder + (yma-ymi)*rc.Dy/sc.VScale

	r.im = image.NewRGBA(image.Rect(0, 0, dx, dy))

	for i := range r.im.Pix {
		r.im.Pix[i] = 0xFF
	}
}

func (r *renderer) findKeymap(title string) (keymapc.Keymap, bool) {
	for _, m := range r.keymap {
		if m.Title == title {
			return m, true
		}
	}
	return keymapc.Keymap{}, false
}

func (r *renderer) mainKeymap() keymapc.Keymap {
	var ml LabelSpec
	found := false
	for _, l := range r.conf.Label {
		if l.Main {
			ml = l
			found = true
			break
		}
	}

	if found {
		if m, ok := r.findKeymap(ml.Layer); ok {
			return m
		}
	}

	// return keymap of first printed layer
	for _, l := range r.conf.Label {
		if m, ok := r.findKeymap(l.Layer); ok {
			return m
		}
	}

	return r.keymap[0]
}

func (r *renderer) drawKeyShapes(km keymapc.Keymap) {
	b := r.conf.Render.KeyBorder
	c := color.Gray{0xdd}
	for _, k := range km.Keys {
		p0 := r.pt(k.X, k.Y)
		p0.X += b
		p0.Y += b
		p1 := r.pt(k.X+k.Dx, k.Y+k.Dy)
		p1.X -= b
		p1.Y -= b
		for x := p0.X; x < p1.X; x++ {
			r.im.Set(x, p0.Y, c)
			r.im.Set(x, p1.Y-1, c)
		}
		for y := p0.Y; y < p1.Y; y++ {
			r.im.Set(p0.X, y, c)
			r.im.Set(p1.X-1, y, c)
		}
	}
}

func (r *renderer) drawKeyLabels(l LabelSpec, km keymapc.Keymap) {
	if l.Scale == 0 {
		l.Scale = 1
	}

	fc := freetype.NewContext()
	fc.SetDPI(96)
	fc.SetFont(r.font)
	fc.SetFontSize(float64(r.conf.Render.FontHeight) * l.Scale)
	fc.SetHinting(font.HintingFull)
	fc.SetSrc(image.NewUniform(color.RGBA{0, 0, 0, 0xFF}))

	b := r.conf.Render.KeyBorder * 3 / 2

KeyLoop:
	for _, k := range km.Keys {
		label := k.Label
		if x, ok := r.conf.Remap[label]; ok {
			label = x
		}

		if label == "" {
			continue
		}

		for _, ignore := range l.Ignore {
			if label == ignore {
				continue KeyLoop
			}
		}

		tr := textRect(fc, label)

		p0 := r.pt(k.X, k.Y)
		p1 := r.pt(k.X+k.Dx, k.Y+k.Dy)

		lr := image.Rect(p0.X+b, p0.Y+b, p1.X-b, p1.Y-b)

		p := adjustText(l.Position, tr, lr)

		// destination needs to be set before painting
		// because textRect resets it
		fc.SetDst(r.im)
		fc.SetClip(lr)

		fc.DrawString(label, freetype.Pt(p.X, p.Y))
	}
}

func adjustText(rel string, tr, lr image.Rectangle) image.Point {

	lc := rectCenter(lr)
	tc := rectCenter(tr)

	// default is fully centered
	x := lc.X - tc.X
	y := lc.Y - tc.Y

	if strings.HasPrefix(rel, "top") {
		y = lr.Min.Y - tr.Min.Y
	} else if strings.HasPrefix(rel, "bottom") {
		y = lr.Max.Y - tr.Max.Y
	}

	if strings.HasSuffix(rel, "left") {
		x = lr.Min.X - tr.Min.X
	} else if strings.HasSuffix(rel, "right") {
		x = lr.Max.X - tr.Max.X
	}

	return image.Pt(x, y)
}

func (r *renderer) pt(kx, ky int) image.Point {
	rc := r.conf.Render
	sc := r.conf.Source

	x := rc.ImageBorder + (kx-r.sx)*rc.Dx/sc.HScale
	y := rc.ImageBorder + (ky-r.sy)*rc.Dy/sc.VScale

	return image.Pt(x, y)
}

func textRect(c *freetype.Context, s string) image.Rectangle {
	const d = 1024
	im := &dimImage{
		bounds: image.Rect(-d, -d, d, d),
	}
	c.SetDst(im)
	c.SetClip(im.Bounds())
	c.DrawString(s, freetype.Pt(0, 0))
	return im.dirty
}

func rectCenter(r image.Rectangle) image.Point {
	x := (r.Min.X + r.Max.X) / 2
	y := (r.Min.Y + r.Max.Y) / 2
	return image.Pt(x, y)
}

// dimImage accumulates painted pixel positions
type dimImage struct {
	bounds image.Rectangle

	// dirty is the rectangle of painted pixels
	dirty image.Rectangle
}

func (im *dimImage) Bounds() image.Rectangle { return im.bounds }
func (im *dimImage) ColorModel() color.Model { return color.RGBAModel }
func (im *dimImage) At(x, y int) color.Color { return color.White }

func (im *dimImage) Set(x, y int, c color.Color) {
	//fmt.Println(x, y, c)
	r := image.Rect(x, y, x+1, y+1)
	im.dirty = im.dirty.Union(r)
}

var _ draw.Image = &dimImage{}
