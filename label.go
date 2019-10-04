package main

import (
	"image"
	"image/draw"
	"strings"

	"golang.org/x/image/font"
	"golang.org/x/image/math/fixed"
)

type labelCtx struct {
	dst  draw.Image
	src  image.Image
	face font.Face

	halign, valign int
}

func (ctx *labelCtx) drawString(r image.Rectangle, s string) {
	if s == "" {
		return
	}

	fr := fixed.R(r.Min.X, r.Min.Y, r.Max.X, r.Max.Y)

	lines := strings.Split(s, "\n")

	mw := fixed.I(0)
	for _, l := range lines {
		w := font.MeasureString(ctx.face, l)
		if w > mw {
			mw = w
		}
	}

	m := ctx.face.Metrics()

	// number of full lines
	nfl := fixed.Int26_6(len(lines) - 1)

	drawer := font.Drawer{
		Dst:  ctx.dst,
		Src:  ctx.src,
		Face: ctx.face,
	}

	var doty fixed.Int26_6

	switch {
	case ctx.valign < 0:
		// top
		doty = fr.Min.Y + m.Ascent

	case ctx.valign > 0:
		// bottom
		doty = fr.Max.Y - m.Descent - nfl*m.Height

	default:
		// center
		th := m.Ascent + nfl*m.Height + m.Descent
		c := (fr.Min.Y + fr.Max.Y) / 2
		doty = c - th/2 + m.Ascent
	}

	for _, l := range lines {
		drawer.Dot.Y = doty

		switch {
		case ctx.halign < 0:
			// left
			drawer.Dot.X = fr.Min.X

		case ctx.halign > 0:
			// right
			w := font.MeasureString(ctx.face, l)
			drawer.Dot.X = fr.Max.X - w

		default:
			// center
			w := font.MeasureString(ctx.face, l)
			c := (fr.Min.X + fr.Max.X) / 2
			drawer.Dot.X = c - w/2
		}

		drawer.DrawString(l)

		doty += m.Height
	}
}
