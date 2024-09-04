package Day09

import (
	"slices"
	"strconv"
	"strings"

	AH "AoC/adventhelper"
)

func parseInput(s string, b bool) []int {
	vs := []int{}
	ts := strings.Split(s, " ")

	for _, t := range ts {
		v, _ := strconv.Atoi(t)
		vs = append(vs, v)
	}

	if b {
		slices.Reverse(vs)
	}

	return vs
}

func diff(vs []int) []int {
	df := []int{}

	for i := 0; i < (len(vs) - 1); i++ {
		df = append(df, vs[i+1]-vs[i])
	}

	return df
}

func newton(vs []int) int {
	all_equal := true
	for _, v := range vs {
		if v != vs[0] {
			all_equal = false
			break
		}
	}

	if all_equal {
		return vs[0]
	}

	return newton(diff(vs)) + vs[len(vs)-1]
}

func Run() {
	inputLines, _ := AH.ReadStrFile("../input/input09.txt")

	part1, part2 := 0, 0
	for _, l := range inputLines {
		part1 += newton(parseInput(l, false))
		part2 += newton(parseInput(l, true))
	}

	AH.PrintSoln(9, part1, part2)

	return
}
