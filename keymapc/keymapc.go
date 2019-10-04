package keymapc

import (
	"bufio"
	"errors"
	"io"
)

type Key struct {
	X, Y   int
	Dx, Dy int
	Label  string
}

type Keymap struct {
	Title string
	Keys  []Key
}

// ErrNotFound is reported when no keymaps were found.
var ErrNotFound = errors.New("No keymap found")

func ParseKeymaps(r io.Reader) ([]Keymap, error) {
	var p gridParser
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		p.line(scanner.Text())
	}
	err := scanner.Err()
	if err == nil && len(p.keymaps) == 0 {
		err = ErrNotFound
	}
	return p.keymaps, err
}
