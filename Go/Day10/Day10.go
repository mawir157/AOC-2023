package Day10

import (
	AH "AoC/adventhelper"
)

type Pos struct {
	r, c int
}

func moves(p Pos, ps []string) (Pos, Pos) {
	c := AH.RuneAt(ps[p.r], p.c)

	if c == 'F' {
		return Pos{p.r + 1, p.c}, Pos{p.r, p.c + 1}
	} else if c == 'L' {
		return Pos{p.r - 1, p.c}, Pos{p.r, p.c + 1}
	} else if c == '7' {
		return Pos{p.r + 1, p.c}, Pos{p.r, p.c - 1}
	} else if c == 'J' {
		return Pos{p.r - 1, p.c}, Pos{p.r, p.c - 1}
	} else if c == '|' {
		return Pos{p.r - 1, p.c}, Pos{p.r + 1, p.c}
	} else if c == '-' {
		return Pos{p.r, p.c - 1}, Pos{p.r, p.c + 1}
	} else if c == '.' {
		return Pos{0, 0}, Pos{0, 0}
	}

	// c == 'S'
	sNbrs := []Pos{}
	above, left, down, right := "F7|", "FL-", "JL|", "J7-"
	if AH.ContainsChar(above, AH.RuneAt(ps[p.r-1], p.c)) {
		sNbrs = append(sNbrs, Pos{p.r - 1, p.c})
	}
	if AH.ContainsChar(left, AH.RuneAt(ps[p.r], p.c-1)) {
		sNbrs = append(sNbrs, Pos{p.r, p.c - 1})
	}
	if AH.ContainsChar(down, AH.RuneAt(ps[p.r+1], p.c)) {
		sNbrs = append(sNbrs, Pos{p.r + 1, p.c})
	}
	if AH.ContainsChar(right, AH.RuneAt(ps[p.r], p.c+1)) {
		sNbrs = append(sNbrs, Pos{p.r, p.c + 1})
	}

	return sNbrs[0], sNbrs[1]
}

func findStart(ps []string) Pos {
	for ir, s := range ps {
		for ic, c := range s {
			if c == 'S' {
				return Pos{ir, ic}
			}
		}
	}

	return Pos{0, 0}
}

func pipes(ps []string) (int, [][]int) {

	bound := make([][]int, len(ps))
	for i := range bound {
		bound[i] = make([]int, len(ps[0]))
	}
	ret := 0

	start := findStart(ps)
	cur := start
	bound[cur.r][cur.c] = 1
	ret++

	nbr, _ := moves(cur, ps)
	prev := cur
	cur = nbr

	for cur != start {
		bound[cur.r][cur.c] = 1
		ret++

		nbrs0, nbrs1 := moves(cur, ps)
		if nbrs0 == prev {
			cur, prev = nbrs1, cur
		} else {
			cur, prev = nbrs0, cur
		}
	}

	return ret / 2, bound
}

func jct(grid []string, bound [][]int) int {
	ret := 0
	for ir, s := range grid {
		bd_count := 0
		r := 'X'
		for ic, t := range s {
			if bound[ir][ic] != 0 {
				if (t == '|') ||
					((t == 'J') && (r == 'F')) ||
					((t == '7') && (r == 'L')) {
					bd_count++
				}

				if (t == 'F') || (t == 'L') {
					r = t
				}
			} else {
				if (bd_count % 2) == 1 {
					ret++
				}
			}
		}
	}

	return ret
}

func Run() {
	ls, _ := AH.ReadStrFile("../input/input10.txt")
	part1, bound := pipes(ls)
	part2 := jct(ls, bound)

	AH.PrintSoln(10, part1, part2)

	return
}
