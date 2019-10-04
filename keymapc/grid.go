package keymapc

import (
	"strings"
)

const (
	gridN uint = 1 << iota
	gridS
	gridW
	gridE
)

var gridCharMap = map[rune]uint{
	'─': gridW | gridE,
	'│': gridN | gridS,
	'┌': gridS | gridE,
	'┐': gridS | gridW,
	'└': gridN | gridE,
	'┘': gridN | gridW,
	'├': gridN | gridS | gridE,
	'┤': gridN | gridS | gridW,
	'┴': gridN | gridW | gridE,
	'┬': gridS | gridW | gridE,
	'┼': gridN | gridS | gridW | gridE,
}

func gridFlags(r rune) uint {
	return gridCharMap[r]
}

func isGridChar(r rune) bool {
	return gridCharMap[r] != 0
}

type gridParser struct {
	title string
	grid  bool
	lines [][]rune

	cur Keymap

	keymaps []Keymap
}

func (p *gridParser) line(s string) {
	trimd := strings.TrimSpace(s)
	if strings.HasPrefix(trimd, "/*") {
		p.title = strings.TrimSpace(s[2:])
		p.grid = true
		return
	}

	if !p.grid {
		return
	}

	if trimd == "*/" {
		p.finishGrid()
		return
	}

	l := trimGridLine(s)
	if l != nil {
		p.lines = append(p.lines, l)
	}
}

func (p *gridParser) finishGrid() {
	p.grid = false

	if len(p.lines) == 0 {
		return
	}

	dx, dy := p.dimLines()

	p.cur = Keymap{
		Title: p.title,
	}

	for y := 0; y < dy; y++ {
		sx := dx
		for x := 0; x < dx; x++ {
			if p.gc(x, y) {
				sx = x
			} else if p.gc(x+1, y) && p.gc(x, y+1) {
				p.addKey(sx, x+1, y+1)
			}
		}
	}

	if len(p.cur.Keys) != 0 {
		p.keymaps = append(p.keymaps, p.cur)
	}

	p.lines = nil
}

func (p *gridParser) addKey(left, right, bottom int) {
	if left+1 >= right || p.gc(left+1, bottom-1) {
		return
	}

	// find top
	top := bottom - 1
	for top > 0 && !p.gc(left+1, top) {
		top--
	}

	label, ok := p.keyText(left, top, right, bottom)
	if !ok {
		return
	}

	p.cur.Keys = append(p.cur.Keys, Key{
		X:     left,
		Y:     top,
		Dx:    right - left,
		Dy:    bottom - top,
		Label: label,
	})
}

func (p *gridParser) keyText(l, t, r, b int) (string, bool) {
	var builder strings.Builder
	for y := t; y < b; y++ {
		for x := l; x < r; x++ {
			isedge := x == l || y == t
			if p.gc(x, y) != isedge {
				return "", false
			}
			if !isedge {
				builder.WriteRune(p.at(x, y))
			}
		}
	}

	return strings.TrimSpace(builder.String()), true
}

func (p *gridParser) dimLines() (dx, dy int) {
	for _, line := range p.lines {
		if l := len(line); l > dx {
			dx = l
		}
	}
	return dx, len(p.lines)
}

func (p *gridParser) at(x, y int) rune {
	if y < 0 || y >= len(p.lines) {
		return ' '
	}
	l := p.lines[y]
	if x < 0 || x >= len(l) {
		return ' '
	}
	return l[x]
}

func (p *gridParser) gc(x, y int) bool {
	return isGridChar(p.at(x, y))
}

// trimGridLine removes excess characters outside grid lines,
// by replacing characters at the front with spaces, and removing characters
// at the end.
// It returns an empty string if there are no grid characters in s.
func trimGridLine(s string) []rune {
	has := false
	i, last := 0, 0
	for _, r := range s {
		i++
		if isGridChar(r) {
			has = true
			last = i
		}
	}

	if !has {
		return nil
	}

	v := make([]rune, 0, last)

	in := false
	for _, r := range s {
		if isGridChar(r) {
			in = true
		}
		if !in {
			r = ' '
		}
		v = append(v, r)
		if len(v) == last {
			break
		}
	}

	return v
}
