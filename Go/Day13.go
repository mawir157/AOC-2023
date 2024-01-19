package main

import (
	"strings"

	AH "./adventhelper"
)

func colRef(row string) map[int]bool {
	idxs := make(map[int]bool)

	for r := 0; r < len(row)-1; r++ {
		good := true
		for i := 0; i <= r; i++ {
			if ((r - i) < 0) || ((r + i + 1) >= len(row)) {
				break
			}

			if row[r-i] != row[r+i+1] {
				good = false
				break
			}
		}

		if !good {
			idxs[r+1] = true
		}
	}

	return idxs
}

func rowRef(rs []string, skip int) int {
	for r := 0; r < len(rs)-1; r++ {
		if (r + 1) == skip {
			continue
		}

		good := true

		for i := 0; i < len(rs); i++ {
			if ((r - i) < 0) || ((r + i + 1) >= len(rs)) {
				break
			}

			if rs[r-i] != rs[r+i+1] {
				good = false
				break
			}
		}

		if good {
			return r + 1
		}
	}

	return -1
}

func findRefLines(grid string, skip int) int {
	gd := strings.Split(grid, "|")

	row := rowRef(gd, skip/100)
	if row > 0 {
		return 100 * row
	}

	cols := make(map[int]bool)
	for i := 1; i < len(gd[0]); i++ {
		cols[i] = true
	}
	if skip > 0 {
		delete(cols, skip%100)
	}

	for _, r := range gd {
		colRefs := colRef(r)
		for c := range colRefs {
			delete(cols, c)
		}
	}

	if len(cols) > 0 {
		for k := range cols {
			return k
		}
	}

	return -1
}

func findAfterSmudge(grid string, bad int) int {
	for i := 0; i < len(grid); i++ {
		c := grid[i]
		if c == '|' {
			continue
		}
		copyGrid := ""
		if c == '.' {
			copyGrid = grid[:i] + "#" + grid[(i+1):]
		} else {
			copyGrid = grid[:i] + "." + grid[(i+1):]
		}

		t := findRefLines(copyGrid, bad)

		if t > 0 {
			return t
		}
	}

	return -1
}

func main() {
	ls, _ := AH.ParseLineGroups("../input/input13.txt", "|")
	part1, part2 := 0, 0
	for _, l := range ls {
		s := findRefLines(l, -1)
		part1 += s
		part2 += findAfterSmudge(l, s)
	}

	AH.PrintSoln(13, part1, part2)

	return
}
