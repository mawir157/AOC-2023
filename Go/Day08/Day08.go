//go:build d08
// +build d08

package Day08

import (
	"strings"

	AH "AoC/adventhelper"
)

type Pair struct {
	l, r string
}

func parseInstruction(s string) []int {
	ret := []int{}
	for _, r := range s {
		if r == 'R' {
			ret = append(ret, 1)
		} else {
			ret = append(ret, 0)
		}
	}

	return ret
}

func parseRules(ss []string) map[string]Pair {
	rs := make(map[string]Pair)

	for _, s := range ss {
		t := strings.Split(s, " = ")

		b := t[1][1:(len(t[1]) - 1)]
		p := strings.Split(b, ", ")

		rs[t[0]] = Pair{p[0], p[1]}
	}

	return rs
}

func runUntilMatch(src string, tgt string, ins []int, rs map[string]Pair, part2 bool) int {
	cur := src
	for i := 0; ; {
		for _, in := range ins {
			i++
			if in == 0 {
				cur = rs[cur].l
			} else {
				cur = rs[cur].r
			}

			if !part2 {
				if cur == tgt {
					return i
				}
			} else {
				if AH.FinalRune(cur) == AH.FinalRune(tgt) {
					return i
				}
			}
		}
	}
}

func findNodesEndingIn(rs map[string]Pair, r rune) []string {
	goodNodes := []string{}
	for k := range rs {
		if AH.FinalRune(k) == r {
			goodNodes = append(goodNodes, k)
		}
	}

	return goodNodes
}

func Run() {
	inputLines, _ := AH.ReadStrFile("../input/input08.txt")
	ins := parseInstruction(inputLines[0])
	rs := parseRules(inputLines[1:])

	part1 := runUntilMatch("AAA", "ZZZ", ins, rs, false)

	part2 := 1
	gns := findNodesEndingIn(rs, 'A')

	for _, n := range gns {
		part2 = AH.LCM(part2, runUntilMatch(n, "ZZZ", ins, rs, true))
	}

	AH.PrintSoln(8, part1, part2)

	return
}
