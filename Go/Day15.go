package main

import (
	"strconv"
	"strings"

	AH "./adventhelper"
)

type P struct {
	s string
	n int
}

func parseInput(s string) (string, bool, int) {
	eq := strings.Index(s, "=")
	if eq > 0 {
		value, _ := strconv.Atoi(s[eq+1:])
		return s[:eq], true, value
	} else {
		return AH.TrimLastRune(s), false, 0
	}
}

func hash(s string) int {
	hash := 0

	for _, r := range s {
		hash += int(r)
		hash *= 17
		hash %= 256
	}

	return hash
}

func dropBox(boxes []P, label string) []P {
	n := -1
	for i, b := range boxes {
		if b.s == label {
			n = i
			break
		}
	}

	if n == -1 {
		return boxes
	} else {
		return append(boxes[:n], boxes[n+1:]...)
	}
}

func scoreBoxes(boxes [][]P) int {
	score := 0
	for b, box := range boxes {
		for i, l := range box {
			score += (b + 1) * (i + 1) * l.n
		}
	}

	return score
}

func main() {
	ls, _ := AH.ReadStrFile("../input/input15.txt")
	inputs := strings.Split(ls[0], ",")

	boxes := make([][]P, 256)
	part1 := 0

	for _, i := range inputs {
		label, add, value := parseInput(i)
		boxId := hash(label)
		part1 += hash(i)

		if add {
			replaced := false
			for i, l := range boxes[boxId] {
				if label == l.s {
					boxes[boxId][i].n = value
					replaced = true
					break
				}
			}
			if !replaced {
				boxes[boxId] = append(boxes[boxId], P{s: label, n: value})
			}
		} else {
			boxes[boxId] = dropBox(boxes[boxId], label)
		}
	}

	AH.PrintSoln(15, part1, scoreBoxes(boxes))

	return
}
