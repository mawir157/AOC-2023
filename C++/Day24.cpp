#include "AH.h"

namespace Day24
{
	struct HailStone
    {
        int64_t  x,  y,  z;
        int64_t dx, dy, dz;
        HailStone(const int64_t x, const int64_t y, const int64_t z,
                  const int64_t dx, const int64_t dy, const int64_t dz) :
          x(x), y(y), z(z), dx(dx), dy(dy), dz(dz) {}
    };

	// returns (a*b)/c when a*b overflows, e.g. muldiv(3,5,2) = 7
	int64_t muldiv(int64_t a, int64_t b, int64_t c)
	{
		const int64_t aDivC = a / c;
		const int64_t bDivC = b / c;
		const int64_t aModC = a % c;
		const int64_t bModC = b % c;

		return (aDivC * bDivC * c) + ((aDivC * bModC + aModC * bDivC)) + ((aModC * bModC) / c);
	}

	int64_t intersectXY(const HailStone lhs, const HailStone rhs)
	{
		const int64_t deltaX = lhs.x - rhs.x;
		const int64_t deltaY = lhs.y - rhs.y;
		int64_t disc = (lhs.dy*rhs.dx) - (lhs.dx*rhs.dy);
		if (disc == 0) { // the trajectories are parallel and never intersect
			return 0;
		}

		int64_t tNum = (deltaX * rhs.dy - deltaY * rhs.dx);
		int64_t sNum = (deltaX * lhs.dy - deltaY * lhs.dx);
		if (AH::sgn(tNum) != AH::sgn(disc)) { // the intersection point is in the past
			return 0;
		}

		if (AH::sgn(sNum) != AH::sgn(disc)) { // the intersection point is in the past
			return 0;
		}

		const int64_t boundLo = 200000000000000, boundHi = 400000000000000;

		const auto tNumX = muldiv(tNum, lhs.dx, disc);
		const auto tNumY = muldiv(tNum, lhs.dy, disc);
		// This can't overflow if boundLo and boundHi are not insane
		if (
			((boundLo - lhs.x) < tNumX) &&
			(tNumX < (boundHi - lhs.x)) &&
			((boundLo - lhs.y) < tNumY) &&
			(tNumY < (boundHi - lhs.y))
			)
		{
			return 1;
		} else {
			return 0;
		}

		return 0;
	}

    HailStone parseInput(const std::string & l)
    {
        const auto  ps = AH::SplitOnString(l, " @ ");
        const auto pos = AH::SplitOnString(ps[0], ", ");
        const auto vel = AH::SplitOnString(ps[1], ", ");

        const auto x = AH::stoi64(pos[0]);
        const auto y = AH::stoi64(pos[1]);
        const auto z = AH::stoi64(pos[2]);

	    const auto dx = AH::stoi64(vel[0]);
        const auto dy = AH::stoi64(vel[1]);
        const auto dz = AH::stoi64(vel[2]);

		return HailStone(x,y,z,dx,dy,dz);
    }

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		std::vector<HailStone> hs;
		for (auto l : ls) {
			hs.push_back(parseInput(l));
		}

		intersectXY(hs[0], hs[10]);

		int64_t part1 = 0;
		for (size_t i = 0; i < hs.size(); ++i) {
			for (size_t j = i + 1; j < hs.size(); ++j) {
				part1 += intersectXY(hs[i], hs[j]);
			}
		}

		int64_t part2 = 0;
		AH::PrintSoln(24, part1, part2);

		return 0;
	}

}
