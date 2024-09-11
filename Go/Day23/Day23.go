//go:build d23
// +build d23

package Day23

import (
	"math"
	"slices"

	AH "AoC/adventhelper"
)

type Grid [][]rune
type Pos struct {
	r, c int
}

func parseInput(ss []string) (Grid, int, int) {
	grid := [][]rune{}
	inCol := 0
	outCol := 0
	for r, row := range ss {
		rv := []rune{}
		for c, ch := range row {
			if (r == 0) && (ch == '.') {
				inCol = c
			}

			if (r == len(ss)-1) && (ch == '.') {
				outCol = c
			}

			rv = append(rv, ch)
		}
		grid = append(grid, rv)
	}

	return grid, inCol, outCol
}

func getNbrs1(here Pos, prev Pos, g Grid) []Pos {
	nbrs := []Pos{}
	r := here.r
	c := here.c

	if r != 0 {
		if (g[r-1][c] == '.') || (g[r-1][c] == '^') {
			n := Pos{r - 1, c}
			if n != prev {
				nbrs = append(nbrs, n)
			}
		}
	}

	if c != len(g)-1 {
		if (g[r][c+1] == '.') || (g[r][c+1] == '>') {
			n := Pos{r, c + 1}
			if n != prev {
				nbrs = append(nbrs, n)
			}
		}
	}

	if r != len(g)-1 {
		if (g[r+1][c] == '.') || (g[r+1][c] == 'v') {
			n := Pos{r + 1, c}
			if n != prev {
				nbrs = append(nbrs, n)
			}
		}
	}

	if c != 0 {
		if (g[r][c-1] == '.') || (g[r][c-1] == '<') {
			n := Pos{r, c - 1}
			if n != prev {
				nbrs = append(nbrs, n)
			}
		}
	}

	return nbrs
}

func getNbrs2(here Pos, prev Pos, g Grid) []Pos {
	nbrs := []Pos{}
	r := here.r
	c := here.c

	if r != 0 {
		if g[r-1][c] != '#' {
			n := Pos{r - 1, c}
			if n != prev {
				nbrs = append(nbrs, n)
			}
		}
	}

	if c != len(g)-1 {
		if g[r][c+1] != '#' {
			n := Pos{r, c + 1}
			if n != prev {
				nbrs = append(nbrs, n)
			}
		}
	}

	if r != len(g)-1 {
		if g[r+1][c] != '#' {
			n := Pos{r + 1, c}
			if n != prev {
				nbrs = append(nbrs, n)
			}
		}
	}

	if c != 0 {
		if g[r][c-1] != '#' {
			n := Pos{r, c - 1}
			if n != prev {
				nbrs = append(nbrs, n)
			}
		}
	}

	return nbrs
}

func calcMaxPath(from, to, prev Pos, g Grid, dist int, vs map[Pos]bool) int {
	p := from
	pp := prev

	for true {
		ns := getNbrs1(p, pp, g)
		rs := getNbrs2(p, pp, g)
		if len(rs) > 1 {
			vs[p] = true
		}

		dist++
		if len(ns) == 1 {
			pp = p
			p = ns[0]

			if p == to {
				return dist
			}
		}

		if len(ns) > 1 {
			maxDist := 0
			for _, pn := range ns {
				d := calcMaxPath(pn, to, p, g, dist, vs)
				if maxDist < d {
					maxDist = d
				}
			}

			return maxDist
		}

		if len(ns) == 0 {
			break
		}

	}

	return math.MinInt64
}

func calcMinPath(from, to, prev Pos, g Grid, dist int) int {
	p := from
	pp := prev

	for true {
		ns := getNbrs2(p, pp, g)
		dist++
		if len(ns) == 1 {
			pp = p
			p = ns[0]

			if p == to {
				return dist
			}
		}

		if len(ns) > 1 {
			if p != from {
				break
			}

			minDist := math.MaxInt64
			for _, pn := range ns {
				d := calcMinPath(pn, to, p, g, dist)
				if minDist > d {
					minDist = d
				}
			}

			return minDist
		}

		if len(ns) == 0 {
			break
		}

	}

	return math.MaxInt64
}

func buildGraph(vertices map[Pos]bool, g Grid) ([]Pos, [][]int) {
	vs := []Pos{}
	for v := range vertices {
		vs = append(vs, v)
	}

	adj := make([][]int, len(vs))
	for i := range adj {
		adj[i] = make([]int, len(vs))
	}

	for i := range vs {
		for j := range vs {
			if i == j {
				continue
			}

			temp := calcMinPath(vs[i], vs[j], Pos{0, 0}, g, 0)
			if temp == math.MaxInt64 {
				adj[i][j] = 0
			} else {
				adj[i][j] = temp
			}
		}
	}

	return vs, adj
}

func maxPathGraph(from, to Pos, vs []Pos, adj [][]int, visited map[Pos]bool) int {
	idx := slices.Index(vs, from)

	nbrs := make(map[Pos]int)

	for i, v := range adj[idx] {
		if v != 0 {
			if _, ok := visited[vs[i]]; !ok {
				nbrs[vs[i]] = v
			}
		}
	}

	if len(nbrs) == 0 {
		return math.MinInt64
	}

	maxDist := 0
	for n, nd := range nbrs {
		if n == to {
			return nd
		}
		visitedNew := make(map[Pos]bool)
		for k, v := range visited {
			visitedNew[k] = v
		}
		visitedNew[from] = true
		d := nd + maxPathGraph(n, to, vs, adj, visitedNew)
		if maxDist < d {
			maxDist = d
		}
	}

	return maxDist
}

func Run() {
	inputLines, _ := AH.ReadStrFile("../input/input23.txt")
	grid, start, end := parseInput(inputLines)
	vs := make(map[Pos]bool)
	vs[Pos{0, start}] = true
	vs[Pos{len(grid) - 1, end}] = true
	part1 := calcMaxPath(Pos{0, start}, Pos{len(grid) - 1, end}, Pos{0, 0}, grid, 0, vs)

	visited := make(map[Pos]bool)
	vv, adj := buildGraph(vs, grid)

	part2 := maxPathGraph(Pos{0, start}, Pos{len(grid) - 1, end}, vv, adj, visited)
	AH.PrintSoln(23, part1, part2)

	return
}
