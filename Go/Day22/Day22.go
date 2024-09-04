package Day22

import (
	"strconv"
	"strings"

	AH "AoC/adventhelper"
)

type Block struct {
	xlo, xhi, ylo, yhi, zlo, zhi int
	hasFallen                    bool
	index                        int
}

func (b *Block) CanFallBy(bs []Block, invisible int) int {
	base := AH.Min(b.zlo, b.zhi)
	if base <= 1 {
		return 0
	}

	fallBy := base - 1
	for i, bl := range bs {
		if i == b.index || i == invisible {
			continue
		}

		if (bl.xlo <= b.xhi) && (b.xlo <= bl.xhi) &&
			(bl.ylo <= b.yhi) && (b.ylo <= bl.yhi) {
			top := AH.Max(bl.zlo, bl.zhi)
			if base > top {
				fallBy = AH.Min(fallBy, base-top-1)
			}
		}
	}

	return fallBy
}

func (b *Block) Fall(by int, part2 bool) {
	b.zlo -= by
	b.zhi -= by
	if part2 {
		b.hasFallen = true
	}

	return
}

func parseInput(s string, i int) Block {
	ll := strings.Split(s, "~")
	lows := strings.Split(ll[0], ",")
	highs := strings.Split(ll[1], ",")

	xlo, _ := strconv.Atoi(lows[0])
	xhi, _ := strconv.Atoi(highs[0])
	ylo, _ := strconv.Atoi(lows[1])
	yhi, _ := strconv.Atoi(highs[1])
	zlo, _ := strconv.Atoi(lows[2])
	zhi, _ := strconv.Atoi(highs[2])

	return Block{xlo, xhi, ylo, yhi, zlo, zhi, false, i}
}

func part1(bs []Block) int {
	ret := 0
	for invisible := range bs {
		for i, b := range bs {
			if i == invisible {
				continue
			}
			if b.CanFallBy(bs, invisible) != 0 {
				ret++
				break
			}
		}
	}

	return len(bs) - ret
}

func part2(bs []Block) int {
	ret := 0
	bbs := make([]Block, len(bs))
	for invisible := range bs {
		copy(bbs, bs)
		falling := true
		for falling {
			falling = false
			for i := range bbs {
				if i == invisible {
					continue
				}
				by := bbs[i].CanFallBy(bbs, invisible)
				if by > 0 {
					falling = true
					bbs[i].Fall(by, true)
				}
			}
		}

		for i := range bs {
			if bbs[i].hasFallen {
				ret++
			}
		}
	}

	return ret
}

func Run() {
	inputLines, _ := AH.ReadStrFile("../input/input22.txt")
	bs := []Block{}
	for i, l := range inputLines {
		bs = append(bs, parseInput(l, i))
	}

	falling := true
	for falling {
		falling = false
		for i := range bs {
			by := bs[i].CanFallBy(bs, len(bs))
			if by > 0 {
				falling = true
				bs[i].Fall(by, false)
			}
		}
	}

	AH.PrintSoln(22, part1(bs), part2(bs))

	return
}
