package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/golang/freetype"
	"github.com/golang/freetype/truetype"
	"github.com/tajtiattila/qmk-keymaps/renderkbd/googlefont"
)

func loadFont(spec string) (*truetype.Font, error) {
	fontdata, err := loadFontData(spec)
	if err != nil {
		return nil, err
	}

	font, err := freetype.ParseFont(fontdata)
	if err != nil {
		return nil, fmt.Errorf("Parse font: %w", err)
	}

	return font, nil
}

func loadFontData(spec string) ([]byte, error) {
	const gpx = "google:"
	if strings.HasPrefix(spec, gpx) {
		return loadGoogleFontData(strings.TrimPrefix(spec, gpx))
	}

	const lpx = "local:"
	spec = strings.TrimPrefix(spec, lpx)

	return loadLocalFontData(spec)
}

func loadGoogleFontData(spec string) ([]byte, error) {
	n := strings.Index(spec, ",")
	var family string
	var weight int
	if n > 0 {
		family = spec[:n]
		var err error
		if weight, err = strconv.Atoi(spec[n+1:]); err != nil {
			return nil, fmt.Errorf("Unrecognised fontspec %s", spec)
		}
	} else {
		family = spec
		weight = 400 // regular
	}

	fontdata, err := googlefont.Get(family, weight)
	if err != nil {
		return nil, fmt.Errorf("Get font: %w", err)
	}

	return fontdata, nil
}

func loadLocalFontData(name string) ([]byte, error) {
	var fontPaths = []string{
		os.ExpandEnv("$LOCALAPPDATA/Microsoft/Windows/Fonts"),
		"C:/Windows/Fonts",
	}

	var err error
	for _, d := range fontPaths {
		var fontdata []byte
		fontdata, err = ioutil.ReadFile(filepath.Join(d, name+".ttf"))
		if err == nil {
			return fontdata, nil
		}
	}

	return nil, fmt.Errorf("Read font: %w", err)
}
