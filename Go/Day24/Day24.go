package Day24

import (
	"strconv"
	"strings"

	AH "AoC/adventhelper"
)

type HailStone struct {
	x, y, z    int
	dx, dy, dz int
}

func parseInput(s string) HailStone {
	ps := strings.Split(s, " @ ")
	pos := strings.Split(ps[0], ", ")
	vel := strings.Split(ps[1], ", ")

	x, _ := strconv.Atoi(pos[0])
	y, _ := strconv.Atoi(pos[1])
	z, _ := strconv.Atoi(pos[2])

	dx, _ := strconv.Atoi(vel[0])
	dy, _ := strconv.Atoi(vel[1])
	dz, _ := strconv.Atoi(vel[2])

	return HailStone{x, y, z, dx, dy, dz}
}

func intersectXY(lhs, rhs HailStone) bool {
	deltaX := lhs.x - rhs.x
	deltaY := lhs.y - rhs.y
	disc := (lhs.dy * rhs.dx) - (lhs.dx * rhs.dy)
	if disc == 0 { //  parallel trajectories
		return false
	}

	tNum := deltaX*rhs.dy - deltaY*rhs.dx
	sNum := deltaX*lhs.dy - deltaY*lhs.dx

	if (AH.Sign(tNum) != AH.Sign(disc)) || (AH.Sign(sNum) != AH.Sign(disc)) {
		return false
	}

	boundLo := 200000000000000
	boundHi := 400000000000000

	tNumX := AH.MulDiv(tNum, lhs.dx, disc)
	tNumY := AH.MulDiv(tNum, lhs.dy, disc)

	if ((boundLo - lhs.x) < tNumX) && (tNumX < (boundHi - lhs.x)) &&
		((boundLo - lhs.y) < tNumY) && (tNumY < (boundHi - lhs.y)) {
		return true
	} else {
		return false
	}
}

func findShift(lhs, rhs HailStone) int {
	deltaX := lhs.x - rhs.x
	deltaY := lhs.y - rhs.y
	disc := (lhs.dy * rhs.dx) - (lhs.dx * rhs.dy)
	tNum := deltaX*rhs.dy - deltaY*rhs.dx
	sNum := deltaX*lhs.dy - deltaY*lhs.dx

	if AH.Sign(tNum) != AH.Sign(disc) {
		return (sNum / disc) - (tNum / disc)
	}

	return 0
}

func throwRock(hs []HailStone) int {
	hsNorm := []HailStone{}
	hsNorm = append(hsNorm, hs[0])

	start := 1
	for len(hsNorm) < 3 {
		for i := start; i < len(hs); i++ {
			good := true
			for _, h := range hsNorm {
				if intersectXY(hs[i], h) {
					good = false
					break
				}
			}
			if good {
				hsNorm = append(hsNorm, hs[i])
				start = i + 1
				break
			}
		}
	}

	for i := 0; i < len(hsNorm); i++ {
		hsNorm[i].x -= hs[0].x
		hsNorm[i].y -= hs[0].y
		hsNorm[i].z -= hs[0].z
		hsNorm[i].dx -= hs[0].dx
		hsNorm[i].dy -= hs[0].dy
		hsNorm[i].dz -= hs[0].dz
	}

	scale := AH.GCD(hsNorm[1].x, hsNorm[1].y)
	scale = AH.GCD(scale, hsNorm[1].z)

	hsNorm[1].x /= scale
	hsNorm[1].y /= scale
	hsNorm[1].z /= scale

	normal := [3]int{}
	normal[0] = hsNorm[1].y*hsNorm[1].dz - hsNorm[1].z*hsNorm[1].dy
	normal[1] = hsNorm[1].z*hsNorm[1].dx - hsNorm[1].x*hsNorm[1].dz
	normal[2] = hsNorm[1].x*hsNorm[1].dy - hsNorm[1].y*hsNorm[1].dx

	scale = AH.GCD(normal[0], normal[1])
	scale = AH.GCD(scale, normal[2])

	// rescale to keep the numbers managable
	normal[0] /= scale
	normal[1] /= scale
	normal[2] /= scale

	tTop := (hsNorm[2].x*normal[0] + hsNorm[2].y*normal[1] + hsNorm[2].z*normal[2])
	tBottom := (hsNorm[2].dx*normal[0] + hsNorm[2].dy*normal[1] + hsNorm[2].dz*normal[2])

	scale = AH.GCD(tTop, tBottom)

	tTop /= scale
	tBottom /= scale

	pp := [3]int{}
	pp[0] = -hsNorm[2].x + (tTop/tBottom)*hsNorm[2].dx
	pp[1] = -hsNorm[2].y + (tTop/tBottom)*hsNorm[2].dy
	pp[2] = -hsNorm[2].z + (tTop/tBottom)*hsNorm[2].dz

	scale = pp[0]
	scale = AH.GCD(scale, pp[1])
	scale = AH.GCD(scale, pp[2])

	pp[0] /= scale
	pp[1] /= scale
	pp[2] /= scale

	var rock HailStone
	if AH.Sign(tTop) != AH.Sign(tBottom) {
		rock = HailStone{0, 0, 0, pp[0], pp[1], pp[2]}
	} else {
		rock = HailStone{0, 0, 0, -pp[0], -pp[1], -pp[2]}
	}

	shiftBy := 0
	for _, h := range hs {
		hh := HailStone{h.x - hs[0].x, h.y - hs[0].y, h.z - hs[0].z,
			h.dx - hs[0].dx, h.dy - hs[0].dy, h.dz - hs[0].dz}
		temp := findShift(rock, hh)
		shiftBy = AH.Max(shiftBy, temp)
	}

	rock.x -= shiftBy * pp[0]
	rock.y -= shiftBy * pp[1]
	rock.z -= shiftBy * pp[2]

	soln := rock.x + hs[0].x + rock.y + hs[0].y + rock.z + hs[0].z

	return soln
}

func Run() {
	inputLines, _ := AH.ReadStrFile("../input/input24.txt")
	hs := []HailStone{}
	for _, l := range inputLines {
		hs = append(hs, parseInput(l))
	}

	part1 := 0
	for i := 0; i < len(hs); i++ {
		for j := i + 1; j < len(hs); j++ {
			if intersectXY(hs[i], hs[j]) {
				part1 += 1
			}
		}
	}

	part2 := throwRock(hs)
	AH.PrintSoln(24, part1, part2)

	return
}
