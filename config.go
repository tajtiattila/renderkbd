package main

import (
	"fmt"
	"image/color"
)

type LabelSpec struct {
	Layer    string   // layer name to show
	Position string   // position
	Scale    float64  // font scale
	Main     bool     // main layer
	Color    cfgColor // label color
	Ignore   []string // ignored labels
}

type KeyFontSpec struct {
	Font   string
	Scale  float64
	Labels []string
}

type Config struct {
	DPI int // font DPI

	Source struct {
		Path string // source path

		HScale int // horizontal size of 1u key in source
		VScale int // vertical size of 1u in source
	}

	Render struct {
		Font string // font to use for rendering

		Image string // destination image path

		Dx int // horizontal size of 1u key in pixels
		Dy int // vertical size of 1u key in pixels

		KeyBorder int // key border in pixels
		HPad      int // horizontal padding inside border
		VPad      int // vertical padding inside border

		ImageBorder int // image border in pixels

		FontHeight int
	}

	Label []LabelSpec

	Remap map[string]string

	FontMap []KeyFontSpec

	KeyColor map[string]cfgColor
}

type cfgColor struct {
	C color.Color
}

func (c *cfgColor) UnmarshalTOML(v interface{}) error {
	s, ok := v.(string)
	if !ok {
		return fmt.Errorf("Can't unmarshal %v (type %T) as color", v, v)
	}

	var r, g, b uint8
	_, err := fmt.Sscanf(s, "#%02x%02x%02x", &r, &g, &b)
	if err != nil {
		return fmt.Errorf("Can't parse %s as color", s)
	}

	c.C = color.RGBA{R: r, G: g, B: b, A: 0xFF}
	return nil
}
