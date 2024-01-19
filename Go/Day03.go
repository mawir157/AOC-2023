package main

import (
	"strconv"
	"unicode"

	AH "./adventhelper"
)

func fn(ss []string) (part1 int, part2 int) {
	part1, part2 = 0, 0

	gears := make(map[int]int)

	for r := 0; r < len(ss); r++ {
		s := ss[r]
		for c := 0; c < len(s); c++ {
			digits := AH.TakeWhileDigit(s[c:])
			n := len(digits)
			if len(digits) > 0 { // we have a number
				hasNbr := false
				gearIdx := 0
				for ir := AH.Max(0, r-1); ir < AH.Min(len(ss), r+2); ir++ {
					for ic := AH.Max(0, c-1); ic < AH.Min(len(s), c+n+1); ic++ {
						qs := ss[ir]
						qc := AH.RuneAt(qs, ic)
						if (qc != '.') && (!unicode.IsDigit(qc)) {
							hasNbr = true
							if qc == '*' {
								gearIdx = ir*len(s) + ic
							}
							break
						}
					}
					if hasNbr {
						break
					}
				}

				if hasNbr {
					v, _ := strconv.Atoi(digits)
					part1 += v
					if gearIdx != 0 {
						_, ok := gears[gearIdx]
						if ok {
							part2 += gears[gearIdx] * v
						} else {
							gears[gearIdx] = v
						}

					}
				}

				c += len(digits)
			}
		}
	}

	return
}

func main() {
	inputLines, _ := AH.ReadStrFile("../input/input03.txt")

	part1, part2 := fn(inputLines)

	AH.PrintSoln(3, part1, part2)

	return
}
