package main

import (
	"sort"
	"strconv"
	"strings"

	AH "./adventhelper"
)

var CARDS = "AKQJT98765432"
var CARDSJ = "AKQT98765432J"

type Hand struct {
	cards string
	bet   int
}

func parseInput(s string) Hand {
	ss := strings.Split(s, " ")
	bet, _ := strconv.Atoi(ss[1])

	return Hand{ss[0], bet}
}

func handRank(hand string) int {
	cardCount := make(map[rune]int)
	for _, r := range hand {
		cardCount[r] += 1
	}

	if len(cardCount) == 1 {
		return 6
	}

	if len(cardCount) == 5 {
		return 0
	}

	maxCard := 0
	for _, v := range cardCount {
		if maxCard < v {
			maxCard = v
		}
	}

	if maxCard == 4 {
		return 5
	}

	if maxCard == 3 {
		if len(cardCount) == 2 {
			return 4
		} else {
			return 3
		}
	}

	if maxCard == 2 {
		if len(cardCount) == 3 {
			return 2
		} else {
			return 1
		}
	}

	return 0
}

func handRankWithJokers(hand string) int {
	best := 0
	for _, c := range CARDS {
		if c == 'J' {
			continue
		}
		hand_copy := strings.Replace(hand, "J", string(c), 5)
		v := handRank(hand_copy)
		if best < v {
			best = v
		}
	}

	return best
}

func handLT(lhs string, rhs string, part2 bool) bool {
	score_lhs, score_rhs := 0, 0
	if !part2 {
		score_lhs = handRank(lhs)
		score_rhs = handRank(rhs)
	} else {
		score_lhs = handRankWithJokers(lhs)
		score_rhs = handRankWithJokers(rhs)
	}

	if score_lhs != score_rhs {
		return score_lhs < score_rhs
	}

	for i := 0; i < 5; i++ {
		l, r := 0, 0
		if !part2 {
			l = strings.Index(CARDS, string(lhs[i]))
			r = strings.Index(CARDS, string(rhs[i]))
		} else {
			l = strings.Index(CARDSJ, string(lhs[i]))
			r = strings.Index(CARDSJ, string(rhs[i]))
		}

		if l != r {
			return l > r
		}
	}

	return false
}

func main() {
	inputLines, _ := AH.ReadStrFile("../input/input07.txt")
	hands := []Hand{}
	for _, l := range inputLines {
		hands = append(hands, parseInput(l))
	}
	part1, part2 := 0, 0
	sort.Slice(hands, func(i, j int) bool {
		return handLT(hands[i].cards, hands[j].cards, false)
	})
	for i, h := range hands {
		part1 += (i + 1) * h.bet
	}

	sort.Slice(hands, func(i, j int) bool {
		return handLT(hands[i].cards, hands[j].cards, true)
	})
	for i, h := range hands {
		part2 += (i + 1) * h.bet
	}

	AH.PrintSoln(7, part1, part2)

	return
}
