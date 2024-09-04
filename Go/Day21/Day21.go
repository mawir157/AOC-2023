package Day21

import (
	AH "AoC/adventhelper"
)

type Pos struct {
	r, c int
}

func parseInput(ss []string) ([][]int, Pos) {
	grid := [][]int{}
	p := Pos{0, 0}

	for r, s := range ss {
		row := []int{}
		for c, ch := range s {
			v := 0
			if ch == '#' {
				v = 1
			}
			row = append(row, v)
			if ch == 'S' {
				p.r = r
				p.c = c
			}
		}
		grid = append(grid, row)
	}

	return grid, p
}

func nbrs(p Pos, rlim, clim int) []Pos {
	ns := []Pos{}
	if p.r > 0 {
		ns = append(ns, Pos{p.r - 1, p.c})
	}
	if p.c > 0 {
		ns = append(ns, Pos{p.r, p.c - 1})
	}
	if p.r < rlim {
		ns = append(ns, Pos{p.r + 1, p.c})
	}
	if p.c < clim {
		ns = append(ns, Pos{p.r, p.c + 1})
	}
	return ns
}

func spread(cur map[Pos]bool, grid [][]int) map[Pos]bool {
	rlim := len(grid) - 1
	clim := len(grid[0]) - 1
	next := make(map[Pos]bool)
	for p := range cur {
		ns := nbrs(p, rlim, clim)
		for _, n := range ns {
			if grid[n.r][n.c] == 0 {
				next[n] = true
			}
		}
	}

	return next
}

func getMagicNumbers(grid [][]int, cur map[Pos]bool) [4]int {
	for i := 0; i < 66; i++ {
		cur = spread(cur, grid)
	}

	stable := [4]int{0, 0, 0, 0}
	for r, row := range grid {
		for c := range row {
			p := Pos{r, c}
			if _, ok := cur[p]; ok {
				absV := AH.AbsInt(r-65) + AH.AbsInt(c-65)
				if (absV % 2) == 0 {
					stable[0]++
					if absV > 65 {
						stable[2]++
					}
				}
			}
		}
	}

	cur = spread(cur, grid)
	for r, row := range grid {
		for c := range row {
			p := Pos{r, c}
			if _, ok := cur[p]; ok {
				absV := AH.AbsInt(r-65) + AH.AbsInt(c-65)
				if (absV % 2) == 1 {
					stable[1]++
					if absV > 65 {
						stable[3]++
					}
				}
			}
		}
	}

	return stable
}

func Run() {
	inputLines, _ := AH.ReadStrFile("../input/input21.txt")
	grid, start := parseInput(inputLines)
	cur := make(map[Pos]bool)
	cur[start] = true

	for i := 0; i < 64; i++ {
		cur = spread(cur, grid)
	}
	part1 := len(cur)

	magic := getMagicNumbers(grid, cur)
	bigN := 26501365
	n := (bigN - (len(grid) / 2)) / len(grid)
	part2 := ((n + 1) * (n + 1) * magic[1]) + (n * n * magic[0]) - ((n + 1) * magic[3]) + (n * magic[2])

	AH.PrintSoln(21, part1, part2)

	return
}
