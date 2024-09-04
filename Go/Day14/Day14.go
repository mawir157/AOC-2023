package Day14

import (
	AH "AoC/adventhelper"
)

func parseInput(ls []string) [][]int {
	grid := [][]int{}
	for _, row := range ls {
		rowVector := []int{}
		for _, c := range row {
			t := 2
			if c == '.' {
				t = 0
			} else if c == 'O' {
				t = 1
			}
			rowVector = append(rowVector, t)
		}
		grid = append(grid, rowVector)
	}

	return grid
}

func shiftNorth(grid *[][]int) {
	moved := true
	for moved {
		moved = false
		for r := 0; r < len(*grid)-1; r++ {
			for c := 0; c < len((*grid)[0]); c++ {
				if ((*grid)[r][c] == 0) && ((*grid)[r+1][c] == 1) {
					moved = true
					(*grid)[r][c] = 1
					(*grid)[r+1][c] = 0
				}
			}
		}
	}
}

func rotateNegative(grid *[][]int) {
	N := len(*grid)
	for r := 0; r < N/2; r++ {
		for c := r; c < N-r-1; c++ {
			(*grid)[r][c], (*grid)[N-1-c][r] = (*grid)[N-1-c][r], (*grid)[r][c]
			(*grid)[N-1-c][r], (*grid)[N-1-r][N-1-c] = (*grid)[N-1-r][N-1-c], (*grid)[N-1-c][r]
			(*grid)[N-1-r][N-1-c], (*grid)[c][N-1-r] = (*grid)[c][N-1-r], (*grid)[N-1-r][N-1-c]
		}
	}
}

func spinCycle(grid *[][]int) {
	for i := 0; i < 4; i++ {
		shiftNorth(grid)
		rotateNegative(grid)
	}

	return
}

func calculateLoad(grid [][]int) int {
	load := 0
	for r, row := range grid {
		rowCount := 0
		for _, col := range row {
			if col == 1 {
				rowCount++
			}
		}
		load += (len(grid) - r) * rowCount
	}
	return load
}

func determinePeriod(vs []int) int {
	for p := 1; p < len(vs); p++ {
		if vs[0] == vs[p] {
			return p
		}
	}

	return -1
}

func Run() {
	ls, _ := AH.ReadStrFile("../input/input14.txt")
	grid := parseInput(ls)

	shiftNorth(&grid)
	part1 := calculateLoad(grid)

	grid = parseInput(ls)
	burnIn := 750
	i := 0
	for i = 0; i < burnIn; i++ {
		spinCycle(&grid)
	}
	cycleMap := make(map[int]int)
	cycleVector := []int{}
	target := 1000000000

	for ; i < burnIn+100; i++ {
		load := calculateLoad(grid)
		cycleVector = append(cycleVector, load)
		cycleMap[i] = load
		spinCycle(&grid)
	}

	period := determinePeriod(cycleVector)
	finaSteps := (target - burnIn) % period
	part2 := cycleMap[burnIn+finaSteps]

	AH.PrintSoln(14, part1, part2)

	return
}
