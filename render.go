package main

import (
	"fmt"
	"image"
	"image/color"
	"image/png"
	"log"
	"math"
	"os"
	"strings"

	"github.com/golang/freetype/truetype"
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
	var err error
	r.font, err = loadFont(r.conf.Render.Font)

	return err
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

	lc := labelCtx{
		dst: r.im,
		src: image.NewUniform(color.RGBA{0, 0, 0, 0xFF}),
		face: truetype.NewFace(r.font, &truetype.Options{
			Size:    float64(r.conf.Render.FontHeight) * l.Scale,
			DPI:     96,
			Hinting: font.HintingFull,
		}),
	}

	if strings.HasPrefix(l.Position, "top") {
		lc.valign = -1
	} else if strings.HasPrefix(l.Position, "bottom") {
		lc.valign = 1
	}

	if strings.HasSuffix(l.Position, "left") {
		lc.halign = -1
	} else if strings.HasSuffix(l.Position, "right") {
		lc.halign = 1
	}

	rc := r.conf.Render
	hp := rc.KeyBorder + rc.HPad
	vp := rc.KeyBorder + rc.VPad

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

		p0 := r.pt(k.X, k.Y)
		p1 := r.pt(k.X+k.Dx, k.Y+k.Dy)

		lr := image.Rect(p0.X+hp, p0.Y+vp, p1.X-hp, p1.Y-vp)

		lc.drawString(lr, label)
	}
}

func (r *renderer) pt(kx, ky int) image.Point {
	rc := r.conf.Render
	sc := r.conf.Source

	x := rc.ImageBorder + (kx-r.sx)*rc.Dx/sc.HScale
	y := rc.ImageBorder + (ky-r.sy)*rc.Dy/sc.VScale

	return image.Pt(x, y)
}
