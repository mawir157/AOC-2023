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

		return (aDivC * bDivC * c) + (aDivC * bModC + aModC * bDivC) + ((aModC * bModC) / c);
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

		const int64_t boundLo = 7, boundHi = 27;
		// const int64_t boundLo = 200000000000000, boundHi = 400000000000000;

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

	int64_t throwRock(const std::vector<HailStone> hs)
	{
		// Step 0 Find three mutually non-interseting paths probably the first
		std::vector<HailStone> hs_norm;
		hs_norm.push_back(hs[0]);
		int start = 1;
		while(hs_norm.size() < 3) {
			for (size_t i = start; i < hs.size(); ++i) {
				bool good = true;
				for (auto h : hs_norm) {
					if (intersectXY(hs[i], h) == 1) {
						good = false;
						break;
					}
				}
				if (good) {
					hs_norm.push_back(hs[i]);
					start = i + 1;
					break;
				}
			}
		}

		// Step 1 change frame of reference so that [0] is stationary at (0,0,0)
		for (auto & h : hs_norm) {
			h.x  -=  hs[0].x;
			h.y  -=  hs[0].y;
			h.z  -=  hs[0].z;
			h.dx -=  hs[0].dx;
			h.dy -=  hs[0].dy;
			h.dz -=  hs[0].dz;
		}

		for (auto h : hs_norm) {
			printf("(%ld, %ld, %ld, %ld, %ld, %ld)\n", h.x,h.y,h.z,h.dx,h.dy,h.dz);
		}

		// Step 2 The plane spanned by hs_norm[0] = (0,0,0) = Q and the trajectory
		// of hs_norm[1] = P + t*dT is given by span {P, proj_v PQ}
		// PQ = hs_norm[1](0,1,2) - vector from Q-to-P
		// v  = hs_horm[1](3,4,5)
		// proj_v PQ = [(v.PQ) / (PQ.PQ)]*v + P
		int64_t vPQ = 0;
		vPQ += hs_norm[1].x * hs_norm[1].dx;
		vPQ += hs_norm[1].y * hs_norm[1].dy;
		vPQ += hs_norm[1].z * hs_norm[1].dz;

		int64_t PQPQ = 0;
		PQPQ += hs_norm[1].x * hs_norm[1].x;
		PQPQ += hs_norm[1].y * hs_norm[1].y;
		PQPQ += hs_norm[1].z * hs_norm[1].z;
		
		std::vector<double> proj = {0,0,0};

		proj[0] = ((double)vPQ / (double)PQPQ) * hs_norm[1].dx + hs_norm[1].x;
		proj[1] = ((double)vPQ / (double)PQPQ) * hs_norm[1].dy + hs_norm[1].y;
		proj[2] = ((double)vPQ / (double)PQPQ) * hs_norm[1].dz + hs_norm[1].z;

		printf("(v.PQ) = %ld, (PQ.PQ) = %ld\n", vPQ, PQPQ);
		printf("(%f, %f, %f)\n", proj[0], proj[1], proj[2]);

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
		// for (size_t i = 0; i < hs.size(); ++i) {
		// 	for (size_t j = i + 1; j < hs.size(); ++j) {
		// 		part1 += intersectXY(hs[i], hs[j]);
		// 	}
		// }

		throwRock(hs);

		int64_t part2 = 0;
		AH::PrintSoln(24, part1, part2);

		return 0;
	}

}
