package main

type LabelSpec struct {
	Layer    string   // layer name to show
	Position string   // position
	Scale    float64  // font scale
	Main     bool     // main layer
	Ignore   []string // ignored labels
}

type Config struct {
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
}
