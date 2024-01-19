package main

import AH "./adventhelper"

func race(time int, dist int) int {
	lo := 0
	hi := time / 2

	for hi-lo > 1 {
		nxt := (hi + lo) / 2
		if nxt*(time-nxt) > dist {
			hi = nxt
		} else {
			lo = nxt
		}
	}

	return time - (hi + lo)
}

func main() {
	times := []int{52, 94, 75, 94}
	dists := []int{426, 1374, 1279, 1216}

	part1 := 1
	for i, t := range times {
		part1 *= race(t, dists[i])
	}

	part2 := race(52947594, 426137412791216)

	AH.PrintSoln(6, part1, part2)

	return
}
