//go:build d11
// +build d11

package Day11

import (
	AH "AoC/adventhelper"
)

type Pos struct {
	r, c int
}

func parseInput(ss []string, ex int) (ret int) {
	ret = 0
	row_offset := 0
	empty_cols := []int{}

	for ic := 0; ic < len(ss[0]); ic++ {
		occ := false
		for ir := 0; ir < len(ss); ir++ {

			if AH.RuneAt(ss[ir], ic) == '#' {
				occ = true
				break
			}
		}
		if !occ {
			empty_cols = append(empty_cols, ic)
		}
	}

	ps := []Pos{}
	for ir := 0; ir < len(ss); ir++ {
		s := ss[ir]
		if !AH.ContainsChar(s, '#') {
			row_offset += ex
		}
		col_offset := 0
		for ic := 0; ic < len(s); ic++ {
			if AH.ContainsInt(empty_cols, ic) {
				col_offset += ex
			}
			if AH.RuneAt(ss[ir], ic) == '#' {
				ps = append(ps, Pos{ir + row_offset, ic + col_offset})
			}
		}
	}

	for i := 0; i < len(ps); i++ {
		for j := i + 1; j < len(ps); j++ {
			ret += AH.AbsInt(ps[i].r-ps[j].r) + AH.AbsInt(ps[i].c-ps[j].c)
		}
	}

	return
}

func Run() {
	ls, _ := AH.ReadStrFile("../input/input11.txt")

	AH.PrintSoln(11, parseInput(ls, 1), parseInput(ls, 999999))

	return
}
