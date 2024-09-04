package Day12

import (
	"strconv"
	"strings"

	AH "AoC/adventhelper"
)

func numberOfSolns(pattern string, is []int, cache map[int]int) int {
	if 0 == len(is) {
		if AH.ContainsChar(pattern, '#') {
			return 0
		}
		return 1
	}

	need := 0
	for _, v := range is {
		need += (v + 1)
	}
	need -= 1
	if need > len(pattern) {
		return 0
	}

	hash := len(pattern)*100 + len(is)
	if v, ok := cache[hash]; ok {
		return v
	}

	toFit := is[0]
	from := strings.IndexAny(pattern, "?#")
	if from < 0 {
		return 0
	}

	waysToPack := 0
	for i := from; i < len(pattern)-toFit+1; i++ {
		t := strings.Index(pattern[i:], ".")
		if (t >= 0) && (t < toFit) {
			continue
		}

		t = strings.Index(pattern, "#")
		if (t >= 0) && (t < i) {
			break
		}

		if i+toFit < len(pattern) {
			if pattern[i+toFit] == '#' {
				continue
			}
		}

		next_idx := i + toFit + 1
		if next_idx > len(pattern) {
			next_idx = len(pattern)
		}
		waysToPack += numberOfSolns(pattern[next_idx:], is[1:], cache)
	}

	cache[hash] = waysToPack
	return waysToPack
}

func parseInput(s string, expand bool) int {
	ps := strings.Split(s, " ")
	qs := strings.Split(ps[1], ",")
	partitions := []int{}
	for _, q := range qs {
		i, _ := strconv.Atoi(q)
		partitions = append(partitions, i)
	}

	cache := make(map[int]int)

	ss := ps[0]
	pp := partitions
	if expand {
		for i := 0; i < 4; i++ {
			ss = ss + "?" + ps[0]
			pp = append(pp, partitions...)
		}
	}
	return numberOfSolns(ss, pp, cache)
}

func Run() {
	ls, _ := AH.ReadStrFile("../input/input12.txt")
	part1, part2 := 0, 0
	for _, l := range ls {
		part1 += parseInput(l, false)
		part2 += parseInput(l, true)
	}

	AH.PrintSoln(12, part1, part2)

	return
}
