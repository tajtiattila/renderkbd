package main

import (
	"flag"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"

	"github.com/BurntSushi/toml"
	"github.com/tajtiattila/qmk-keymaps/renderkbd/keymapc"
)

func main() {
	flag.Parse()

	if flag.NArg() != 1 {
		log.Fatalln("need exactly one config argument")
	}

	confp := flag.Arg(0)
	raw, err := ioutil.ReadFile(confp)
	if err != nil {
		log.Fatalln("Read config:", err)
	}

	var conf Config
	if err := toml.Unmarshal(raw, &conf); err != nil {
		log.Fatalln("Parse config:", err)
	}

	conf.Source.Path = filepath.Join(filepath.Dir(confp), conf.Source.Path)

	if conf.Render.Image == "" {
		a, err := filepath.Abs(confp)
		if err != nil {
			log.Fatalln(err)
		}

		d := filepath.Dir(a)
		conf.Render.Image = filepath.Join(d, filepath.Base(d)+".png")
	}

	nzd := func(p *int, def int) {
		if *p == 0 {
			*p = def
		}
	}

	nzd(&conf.DPI, 96)
	nzd(&conf.Render.Dx, 64)
	nzd(&conf.Render.Dy, 64)
	nzd(&conf.Render.KeyBorder, 2)
	nzd(&conf.Render.HPad, 2)
	nzd(&conf.Render.VPad, 2)
	nzd(&conf.Render.ImageBorder, 32)
	nzd(&conf.Render.FontHeight, 12)

	srcf, err := os.Open(conf.Source.Path)
	if err != nil {
		log.Fatalln("Open source:", err)
	}
	defer srcf.Close()

	kbds, err := keymapc.ParseKeymaps(srcf)
	if err != nil {
		log.Fatalln("Parse source:", err)
	}

	if err := render(conf, kbds); err != nil {
		log.Fatalln("Render:", err)
	}
}
