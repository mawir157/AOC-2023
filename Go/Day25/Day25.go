package Day25

import (
	"math"
	"strings"

	AH "AoC/adventhelper"
)

type Graph [][]int

func parseInput(ss []string) Graph {
	name2Idx := make(map[string]int)

	for _, s := range ss {
		name := s[:3]
		if _, ok := name2Idx[name]; !ok {
			name2Idx[name] = len(name2Idx)
		}

		to := strings.Split(s[5:], " ")
		for _, t := range to {
			if _, ok := name2Idx[t]; !ok {
				name2Idx[t] = len(name2Idx)
			}
		}
	}

	g := make([][]int, len(name2Idx))
	for i := range g {
		g[i] = make([]int, len(name2Idx))
	}

	for _, s := range ss {
		from := s[:3]
		fromIdx := name2Idx[from]

		to := strings.Split(s[5:], " ")
		for _, t := range to {
			toIdx := name2Idx[t]
			g[fromIdx][toIdx] = 1
			g[toIdx][fromIdx] = 1
		}
	}

	return g
}

func stoerWagner(g Graph) []int {
	bestI := math.MaxInt
	bestV := []int{}
	n := len(g)
	co := make([][]int, n)

	for i := range g {
		co[i] = []int{i}
	}

	w := make([]int, n)
	for ph := 1; ph < n; ph++ {
		copy(w, g[0])
		s, t := 0, 0
		for it := 0; it < n-ph; it++ {
			w[t] = math.MinInt
			s = t
			t, _ = AH.MaxAndMinIdx(w)
			for i := range g {
				w[i] += g[t][i]
			}
		}
		if w[t]-g[t][t] < bestI {
			bestI = w[t] - g[t][t]
			bestV = co[t]
		}
		co[s] = append(co[s], co[t]...)
		for i := range g {
			g[s][i] += g[t][i]
		}
		for i := range g {
			g[i][s] = g[s][i]
		}
		g[0][t] = math.MinInt
	}

	return bestV
}

func Run() {
	ls, _ := AH.ReadStrFile("../input/input25.txt")
	g := parseInput(ls)
	w := stoerWagner(g)

	part1 := len(w) * (len(g) - len(w))
	AH.PrintSoln(25, part1, "P.S. You Rock My World")

	return
}
