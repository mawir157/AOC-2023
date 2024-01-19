package main

import (
	"strconv"
	"unicode"

	AH "./adventhelper"
)

var LUT = map[string]int{
	"one":   1,
	"two":   2,
	"three": 3,
	"four":  4,
	"five":  5,
	"six":   6,
	"seven": 7,
	"eight": 8,
	"nine":  9,
	"zero":  0,
}

func calibrate(s string, b bool) int {
	ret := 0
	for n, r := range s {
		done := false
		if unicode.IsDigit(r) {
			val, _ := strconv.Atoi(string(r))
			ret += 10 * val
			break
		}
		if b {
			for key, val := range LUT {
				if (n + len(key)) > len(s) {
					continue
				}
				ss := s[n:(n + len(key))]
				if ss == key {
					ret += 10 * val
					done = true
					break
				}
			}
			if done {
				break
			}
		}
	}

	for n := len(s) - 1; n >= 0; n-- {
		done := false
		r := AH.RuneAt(s, n)
		if unicode.IsDigit(r) {
			val, _ := strconv.Atoi(string(r))
			ret += val
			break
		}

		if b {
			for key, val := range LUT {
				if (n + len(key)) > len(s) {
					continue
				}
				ss := s[n:(n + len(key))]
				if ss == key {
					ret += val
					done = true
					break
				}
			}
			if done {
				break
			}
		}
	}

	return ret
}

func main() {
	inputLines, _ := AH.ReadStrFile("../input/input01.txt")

	part1, part2 := 0, 0

	for _, l := range inputLines {
		part1 += calibrate(l, false)
		part2 += calibrate(l, true)
	}

	AH.PrintSoln(1, part1, part2)

	return
}
