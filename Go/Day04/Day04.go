//go:build d04
// +build d04

package Day04

import (
	"slices"
	"strconv"
	"strings"

	AH "AoC/adventhelper"
)

func parseLine(game string) ([]int, []int) {
	winning := []int{}
	scratchcard := []int{}
	parts := strings.Split(game, ":")
	s := strings.Split(parts[1], "|")

	wins := strings.Split(s[0], " ")
	for _, win := range wins {
		if len(win) == 0 {
			continue
		}
		win_val, _ := strconv.Atoi(win)
		winning = append(winning, win_val)
	}

	cards := strings.Split(s[1], " ")
	for _, card := range cards {
		if len(card) == 0 {
			continue
		}
		card_val, _ := strconv.Atoi(card)
		scratchcard = append(scratchcard, card_val)
	}

	return winning, scratchcard
}

func scoreCard(winning []int, card []int) (score int) {
	score = 0
	for _, number := range card {
		if slices.Contains(winning, number) {
			score++
		}
	}

	return
}

func Run() {
	inputLines, _ := AH.ReadStrFile("../input/input04.txt")

	part1, part2 := 0, 0
	copies := make([]int, len(inputLines))
	for i := range copies {
		copies[i] = 1
	}
	for i, l := range inputLines {
		winning, scratchcard := parseLine(l)
		score := scoreCard(winning, scratchcard)
		if score > 0 {
			part1 += (1 << (score - 1))

			for j := 0; j < score; j++ {
				if (i + j + 1) < len(copies) {
					copies[i+j+1] += copies[i]
				}
			}
		}
	}

	for _, c := range copies {
		part2 += c
	}

	AH.PrintSoln(4, part1, part2)

	return
}
