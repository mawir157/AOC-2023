//go:build d19
// +build d19

package Day19

import (
	"strconv"
	"strings"

	AH "AoC/adventhelper"
)

type mode int
type comp int

const (
	x mode = iota
	m
	a
	s
	f
)

func runeToMode(r rune) mode {
	switch r {
	case 'x':
		return x
	case 'm':
		return m
	case 'a':
		return a
	case 's':
		return s
	}

	return f
}

const (
	lt comp = iota
	gt
	none
)

func runeToComp(r rune) comp {
	switch r {
	case '<':
		return lt
	case '>':
		return gt
	}

	return none
}

type Instruction struct {
	m mode
	c comp
	v int
	o string
}

type WorkFlow struct {
	ins      []Instruction
	fallback string
}

type Range struct {
	lo, hi int
}

func getWorkflows(ss string) map[string]WorkFlow {
	wfs := make(map[string]WorkFlow)
	ls := strings.Split(ss, "|")
	for _, l := range ls {
		bra := strings.Index(l, "{")
		prefix := l[:bra]
		contents := l[bra+1 : len(l)-1]
		cs := strings.Split(contents, ",")
		wf := WorkFlow{ins: []Instruction{}, fallback: "X"}
		for _, cc := range cs {
			if strings.Index(cc, ":") > 0 {
				qq := strings.Split(cc, ":")
				m := runeToMode(AH.RuneAt(qq[0], 0))
				c := runeToComp(AH.RuneAt(qq[0], 1))
				v, _ := strconv.Atoi(qq[0][2:])
				o := qq[1]

				ins := Instruction{m, c, v, o}
				wf.ins = append(wf.ins, ins)
			} else {
				wf.fallback = cc
			}
		}

		wfs[prefix] = wf
	}

	return wfs
}

func getParts(ss string) [][]int {
	ls := strings.Split(ss, "|")
	parts := [][]int{}

	for _, l := range ls {
		pt := make([]int, 4)
		trimmed := l[1 : len(l)-1]
		qq := strings.Split(trimmed, ",")

		for i, q := range qq {
			val, _ := strconv.Atoi(q[2:])
			pt[i] = val
		}

		parts = append(parts, pt)
	}

	return parts
}

func applyWorkflows(p []int, wfs map[string]WorkFlow, here string) int {
	for (here != "A") && (here != "R") {
		wf := wfs[here]

		here = wf.fallback
		for _, is := range wf.ins {
			v := p[is.m]
			if ((is.c == gt) && (v > is.v)) || ((is.c == lt) && (v < is.v)) {
				here = is.o
				break
			}
		}
	}

	if here == "A" {
		sum := 0
		for _, pp := range p {
			sum += pp
		}
		return sum
	}

	return 0
}

func rangeSize(rs []Range) int {
	ret := 1
	for _, r := range rs {
		ret *= (r.hi - r.lo + 1)
	}

	return ret
}

func applyWorkflowToRanges(rngs []Range, wfs map[string]WorkFlow, here string) int {
	ret := 0
	wf := wfs[here]
	for _, i := range wf.ins {
		newRngs := make([]Range, 4)
		copy(newRngs, rngs)
		switch i.c {
		case gt:
			if newRngs[i.m].hi > i.v {
				newRngs[i.m].lo = AH.Max(newRngs[i.m].lo, i.v+1)
				if i.o == "A" {
					ret += rangeSize(newRngs)
				} else if i.o != "R" {
					ret += applyWorkflowToRanges(newRngs, wfs, i.o)
				}
				rngs[i.m].hi = AH.Min(rngs[i.m].hi, i.v)
			}
		case lt:
			if newRngs[i.m].lo < i.v {
				newRngs[i.m].hi = AH.Min(newRngs[i.m].hi, i.v-1)
				if i.o == "A" {
					ret += rangeSize(newRngs)
				} else if i.o != "R" {
					ret += applyWorkflowToRanges(newRngs, wfs, i.o)
				}
				rngs[i.m].lo = AH.Max(rngs[i.m].lo, i.v)
			}
		}
	}
	if wf.fallback == "A" {
		ret += rangeSize(rngs)
	} else if wf.fallback != "R" {
		ret += applyWorkflowToRanges(rngs, wfs, wf.fallback)
	}

	return ret
}

func Run() {
	gs, _ := AH.ParseLineGroups("../input/input19.txt", "|")
	wfs := getWorkflows(gs[0])
	parts := getParts(gs[1])

	part1 := 0
	for _, p := range parts {
		part1 += applyWorkflows(p, wfs, "in")
	}
	rngs := []Range{{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}}

	AH.PrintSoln(19, part1, applyWorkflowToRanges(rngs, wfs, "in"))

	return
}
