//go:build d18
// +build d18

package Day18

import (
	"strconv"
	"strings"

	AH "AoC/adventhelper"
)

type P struct {
	x, y int
}

func parseInput(s string, part1 bool) (string, int) {
	ss := strings.Split(s, " ")
	if part1 {
		value, _ := strconv.Atoi(ss[1])
		return ss[0], value
	} else {
		col := ss[2][2:8]
		i := col[5]
		dir := "_"
		switch i {
		case '0':
			dir = "R"
		case '1':
			dir = "D"
		case '2':
			dir = "L"
		case '3':
			dir = "U"
		}
		hex := "000" + col[0:5]
		x, _ := strconv.ParseInt(hex, 16, 64)
		return dir, int(x)
	}
}

func digTrench(d *int, vs *[]P, s string, part1 bool) {
	id, im := parseInput(s, part1)

	p := (*vs)[len(*vs)-1]
	switch id {
	case "L":
		p.x -= im
	case "U":
		p.y -= im
	case "R":
		p.x += im
	case "D":
		p.y += im
	}
	*d += im

	*vs = append(*vs, p)

	return
}

func simplePolygonArea(vs []P, offset int) int {
	sum := offset
	for i := 1; i < len(vs); i++ {
		sum += (vs[i].y * vs[i-1].x) - (vs[i-1].y * vs[i].x)
	}

	return 1 + AH.AbsInt(sum)/2
}

func Run() {
	ls, _ := AH.ReadStrFile("../input/input18.txt")

	l1, l2 := 0, 0
	v1 := []P{{0, 0}}
	v2 := []P{{0, 0}}

	for _, l := range ls {
		digTrench(&l1, &v1, l, true)
		digTrench(&l2, &v2, l, false)
	}

	AH.PrintSoln(18, simplePolygonArea(v1, l1), simplePolygonArea(v2, l2))

	return
}
