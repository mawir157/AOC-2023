package main

import AH "./adventhelper"

import (
	"math"
	"strconv"
	"strings"
)

type Mapping struct {
	r, s, t int
}

type Range struct {
	s, w int
}

func parseSeeds(s string) []int {
	is := []int{}
	ss := strings.Split(s[7:], " ")

	for _, s := range ss {
		i, _ := strconv.Atoi(s)
		is = append(is, i)
	}

	return is
}

func parseSeedsToRanges(s string) []Range {
	rs := []Range{}

	ss := strings.Split(s[7:], " ")

	for i := 0; i < len(ss); i += 2 {
		start, _ := strconv.Atoi(ss[i])
		width, _ := strconv.Atoi(ss[i+1])
		rs = append(rs, Range{start, width})
	}

	return rs
}

func parseMapping(s string) []Mapping {
	ms := []Mapping{}

	ss := strings.Split(s, "|")
	for _, s := range ss {
		if AH.ContainsChar(s, ':') {
			continue
		}
		p := strings.Split(s, " ")
		r, _ := strconv.Atoi(p[0])
		s, _ := strconv.Atoi(p[1])
		t, _ := strconv.Atoi(p[2])
		ms = append(ms, Mapping{r,s,t})
	}

	return ms
}

func applyMappingToRanges(rs []Range, ms []Mapping) []Range {
	post := []Range{}
	next := []Range{}

	for _, m := range ms {
		for ; 0 < len(rs); {
			r := rs[0]
			rs = rs[1:]
			if (r.s >= (m.s + m.t)) || (m.s >= (r.s + r.w)) {
				next = append(next, r)
			} else {
				overlap_s := AH.Max(r.s, m.s)
				overlap_e := AH.Min(r.s + r.w, m.s + m.t)

				if (overlap_s > r.s) {
					next = append(next, Range{r.s, overlap_s - r.s})
				}

				post = append(post, Range{overlap_s + m.r - m.s, overlap_e - overlap_s})

				if (overlap_e < r.s + r.w) {
					next = append(next, Range{overlap_e, r.s + r.w - overlap_e})
				}
			}
		}
		rs = next
		next = []Range{}
	}

	rs = append(rs, post...)

	return rs
}

func applyMappingToInt(n int, ms []Mapping) int {
	for _, m := range ms {
		if (n >= m.s) && ((n - m.s) < m.t) {
			return m.r + (n - m.s)
		}
	}
	return n
}

func main() {
	inputLines, _ := AH.ReadStrFile("../input/input05.txt")
	ss, _ := AH.ParseLineGroups("../input/input05.txt", "|")

	mss := [][]Mapping{}
	for _, s := range ss[1:] {
		mss = append(mss, parseMapping(s))
	}

	seeds := parseSeeds(inputLines[0])

	part1 := math.MaxInt64
	for _, s := range seeds {
		for _, ms := range mss {
			s = applyMappingToInt(s, ms)
		}
		if (part1 > s) {
			part1 = s
		}
	}

	seedRanges := parseSeedsToRanges(inputLines[0])
	for _, ms := range mss {
		seedRanges = applyMappingToRanges(seedRanges, ms)
	}

	part2 := math.MaxInt64
	for _, r := range seedRanges {
		if part2 > r.s {
			part2 = r.s
		}
	}

	AH.PrintSoln(5, part1, part2)

	return
}
