//go:build d17
// +build d17

package Day17

import (
	"math"

	AH "AoC/adventhelper"
)

type Grid2 [][]int
type Grid3 [][][]int

type Pos struct {
	r, c, d int
}

func parseInput(ss []string) Grid2 {
	grid := [][]int{}
	for _, s := range ss {
		row := []int{}
		for _, c := range s {
			row = append(row, int(c-'0'))
		}
		grid = append(grid, row)
	}

	return grid
}

func hashPos(p Pos) int {
	ret := p.r
	ret *= 1000
	ret += p.c
	ret *= 10
	ret += p.d

	return ret
}

func deHashPos(i int) Pos {
	d := i % 10
	i /= 10
	c := i % 1000
	i /= 1000
	r := i % 1000

	return Pos{r, c, d}
}

func nbrs(p Pos, rs int, cs int, ultra bool) []Pos {
	ret := []Pos{}

	if ultra {
		if p.d%2 == 1 {
			for c := 0; c < cs; c++ {
				if (AH.AbsInt(p.c-c) < 4) || (AH.AbsInt(p.c-c) > 10) {
					continue
				} else {
					d := 0
					if c < p.c {
						d = 2
					}
					ret = append(ret, Pos{p.r, c, d})
				}
			}
		} else {
			for r := 0; r < rs; r++ {
				if (AH.AbsInt(p.r-r) < 4) || (AH.AbsInt(p.r-r) > 10) {
					continue
				} else {
					d := 3
					if r < p.r {
						d = 1
					}
					ret = append(ret, Pos{r, p.c, d})
				}
			}
		}
	} else {
		rowLo, rowHi := p.r, p.r+1
		colLo, colHi := p.c, p.c+1
		if p.d%2 == 0 {
			rowLo = AH.Max(p.r-3, 0)
			rowHi = AH.Min(p.r+3, rs-1) + 1
		} else {
			colLo = AH.Max(p.c-3, 0)
			colHi = AH.Min(p.c+3, cs-1) + 1
		}
		for r := rowLo; r < rowHi; r++ {
			for c := colLo; c < colHi; c++ {
				if (r == p.r) && (c == p.c) {
					continue
				}
				d := 0
				if r > p.r {
					d = 3
				} else if c < p.c {
					d = 2
				} else if r < p.r {
					d = 1
				}

				ret = append(ret, Pos{r, c, d})
			}
		}
	}

	return ret
}

func findSmallest(Q map[int]int, dist Grid3, minSeen int) Pos {
	ret := Pos{-1, -1, -1}
	smallest := math.MaxInt
	for k := range Q {
		p := deHashPos(k)
		d := dist[p.r][p.c][p.d%2]
		if d < smallest {
			smallest = d
			ret = p
			if smallest == minSeen {
				break
			}
		}
	}

	return ret
}

func cost(start Pos, end Pos, grid Grid2) int {
	cost := 0
	rowLo := AH.Min(start.r, end.r)
	rowHi := AH.Max(start.r, end.r) + 1
	colLo := AH.Min(start.c, end.c)
	colHi := AH.Max(start.c, end.c) + 1

	for r := rowLo; r < rowHi; r++ {
		for c := colLo; c < colHi; c++ {
			cost += grid[r][c]
		}
	}

	return cost - grid[start.r][start.c]
}

func shortestPath(grid Grid2, start Pos, target Pos, ultra bool) int {
	dist := make([][][]int, len(grid))
	for i := range dist {
		dist[i] = make([][]int, len(grid))
		for j := range dist[i] {
			dist[i][j] = make([]int, 2)
		}
	}
	V := make(map[int]int)
	huge := math.MaxInt

	for i := 0; i < len(grid); i++ {
		for j := 0; j < len(grid[i]); j++ {
			for d := 0; d < 2; d++ {
				dist[i][j][d] = huge
			}
		}
	}

	dist[start.r][start.c][start.d%2] = grid[start.r][start.c]
	V[hashPos(start)] = 1
	minSeen := dist[start.r][start.c][start.d%2]

	for len(V) > 0 {
		u := findSmallest(V, dist, minSeen)
		minSeen = dist[u.r][u.c][u.d%2]
		delete(V, hashPos(u))

		nbrs := nbrs(u, len(grid), len(grid[0]), ultra)
		for _, n := range nbrs {
			l := minSeen + cost(u, n, grid)
			if l < dist[n.r][n.c][n.d%2] {
				dist[n.r][n.c][n.d%2] = l
				V[hashPos(n)] = 1
			}
		}
	}

	best := AH.Min(dist[target.r][target.c][0], dist[target.r][target.c][1])

	return best - grid[start.r][start.c]
}

func Run() {
	ls, _ := AH.ReadStrFile("../input/input17.txt")
	grid := parseInput(ls)

	part1W := shortestPath(grid, Pos{0, 0, 0}, Pos{len(grid) - 1, len(grid[0]) - 1, 0}, false)
	part1S := shortestPath(grid, Pos{0, 0, 3}, Pos{len(grid) - 1, len(grid[0]) - 1, 0}, false)

	part2W := shortestPath(grid, Pos{0, 0, 0}, Pos{len(grid) - 1, len(grid[0]) - 1, 0}, true)
	part2S := shortestPath(grid, Pos{0, 0, 3}, Pos{len(grid) - 1, len(grid[0]) - 1, 0}, true)

	AH.PrintSoln(17, AH.Min(part1S, part1W), AH.Min(part2S, part2W))

	return
}
