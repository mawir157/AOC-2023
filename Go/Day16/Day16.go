package Day16

import (
	AH "AoC/adventhelper"
)

func parseInput(ss []string) [][]int {
	grid := [][]int{}
	for _, row := range ss {
		rowVector := []int{}
		for _, c := range row {
			t := -1
			if c == '.' {
				t = 0
			} else if c == '/' {
				t = 1
			} else if c == '\\' {
				t = 2
			} else if c == '-' {
				t = 3
			} else if c == '|' {
				t = 4
			}
			rowVector = append(rowVector, t)
		}
		grid = append(grid, rowVector)
	}

	return grid
}

type Beam struct {
	r, c, d int
}

func updateBeam(b Beam, loc int) Beam {
	if loc == 1 {
		if (b.d % 2) == 1 {
			b.d -= 1
		} else {
			b.d += 1
		}
	} else if loc == 2 {
		b.d = (3 - b.d)
	}

	switch b.d {
	case 0:
		b.c++
	case 1:
		b.r--
	case 2:
		b.c--
	case 3:
		b.r++
	}

	return b
}

func fireTheLaser(grid [][]int, grid2 *[][]int, b Beam, path map[Beam]int) {
	for true {
		if _, ok := path[b]; ok {
			break
		}

		if (b.r < 0) || (b.c < 0) || (b.r >= len(grid)) ||
			(b.c >= len(grid[b.r])) {
			break
		}

		(*grid2)[b.r][b.c] = 1
		path[b] = 1

		val := grid[b.r][b.c]
		if (val == 4) && (b.d%2 == 0) {
			bn := b
			bn.d = 1
			bn.r--
			fireTheLaser(grid, grid2, bn, path)
			bs := b
			bs.d = 3
			bs.r++
			fireTheLaser(grid, grid2, bs, path)
		} else if (val == 3) && (b.d%2 == 1) {
			bw := b
			bw.d = 0
			bw.c++
			fireTheLaser(grid, grid2, bw, path)
			be := b
			be.d = 2
			be.c--
			fireTheLaser(grid, grid2, be, path)
		} else {
			b = updateBeam(b, val)
		}
	}

	return
}

func sumGrid(grid [][]int) (sum int) {
	sum = 0
	for _, row := range grid {
		for _, col := range row {
			sum += col
		}
	}

	return
}

func part2(grid [][]int) (best int) {
	best = 0
	for c := range grid[0] {
		grid2 := make([][]int, len(grid))
		for i := range grid2 {
			grid2[i] = make([]int, len(grid[i]))
		}
		path := make(map[Beam]int)
		initial := Beam{0, c, 3}
		fireTheLaser(grid, &grid2, initial, path)
		t := sumGrid(grid2)
		if t > best {
			best = t
		}
	}

	for c := range grid[0] {
		grid2 := make([][]int, len(grid))
		for i := range grid2 {
			grid2[i] = make([]int, len(grid[i]))
		}
		path := make(map[Beam]int)
		initial := Beam{len(grid) - 1, c, 1}
		fireTheLaser(grid, &grid2, initial, path)
		t := sumGrid(grid2)
		if t > best {
			best = t
		}
	}

	for r := range grid[0] {
		grid2 := make([][]int, len(grid))
		for i := range grid2 {
			grid2[i] = make([]int, len(grid[i]))
		}
		path := make(map[Beam]int)
		initial := Beam{r, 0, 2}
		fireTheLaser(grid, &grid2, initial, path)
		t := sumGrid(grid2)
		if t > best {
			best = t
		}
	}

	for r := range grid[0] {
		grid2 := make([][]int, len(grid))
		for i := range grid2 {
			grid2[i] = make([]int, len(grid[i]))
		}
		path := make(map[Beam]int)
		initial := Beam{r, len(grid) - 1, 0}
		fireTheLaser(grid, &grid2, initial, path)
		t := sumGrid(grid2)
		if t > best {
			best = t
		}
	}

	return
}

func Run() {
	ls, _ := AH.ReadStrFile("../input/input16.txt")
	grid1 := parseInput(ls)
	initial := Beam{0, 0, 0}
	path := make(map[Beam]int)
	grid2 := make([][]int, len(grid1))
	for i := range grid2 {
		grid2[i] = make([]int, len(grid1[i]))
	}
	fireTheLaser(grid1, &grid2, initial, path)

	part1, part2 := sumGrid(grid2), part2(grid1)

	AH.PrintSoln(16, part1, part2)

	return
}
