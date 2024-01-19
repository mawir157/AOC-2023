package main

import (
	"strconv"
	"strings"

	AH "./adventhelper"
)

func maxPerColour(s string) (count map[rune]int) {
	count = map[rune]int{'r': 0, 'g': 0, 'b': 0}

	parts := strings.Split(s, ":")
	ss := strings.Split(parts[1], ";")

	for _, s := range ss {
		cols := strings.Split(s, ",")
		for _, col := range cols {
			split := strings.Split(col, " ")
			val, _ := strconv.Atoi(split[1])
			col_rune := AH.FirstRune(split[2])

			if count[col_rune] < val {
				count[col_rune] = val
			}
		}
	}

	return
}

func validateGame(game string, target map[rune]int) (bool, int) {
	seen := maxPerColour(game)
	part2 := 1
	for _, val := range seen {
		part2 *= val
	}

	for col, val := range target {
		if val < seen[col] {
			return false, part2
		}
	}

	return true, part2
}

func main() {
	inputLines, _ := AH.ReadStrFile("../input/input02.txt")

	part1, part2 := 0, 0
	target := map[rune]int{'r': 12, 'g': 13, 'b': 14}

	for i, l := range inputLines {
		b, p := validateGame(l, target)
		if b {
			part1 += (i + 1)
		}
		part2 += p
	}

	AH.PrintSoln(2, part1, part2)

	return
}
