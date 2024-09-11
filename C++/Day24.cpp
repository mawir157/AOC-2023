#ifdef DAY24

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
		  HailStone() {};
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

	// How far back do we have to shift the rock (lhs) to hit the hailstone (rhs)?
	int64_t findShift(const HailStone lhs, const HailStone rhs)
	{
		const int64_t deltaX = lhs.x - rhs.x;
		const int64_t deltaY = lhs.y - rhs.y;

		int64_t disc = (lhs.dy*rhs.dx) - (lhs.dx*rhs.dy);
		int64_t tNum = (deltaX * rhs.dy - deltaY * rhs.dx);
		int64_t sNum = (deltaX * lhs.dy - deltaY * lhs.dx);

		// The rock hits the hail stone in the past - so we have to shift back
		// by the difference
		if (AH::sgn(tNum) != AH::sgn(disc)) {
			return (sNum / disc) - (tNum / disc);
		}

		return 0;
	}

	int64_t throwRock(const std::vector<HailStone> hs)
	{
		// Step 0 Find three mutually non-interseting paths probably the first 3
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

		// Step 2 The plane spanned by hs_norm[0] = (0,0,0) = O and the trajectory
		// of hs_norm[1] = P + t*dP is given by span {P, dP}
		// Normalise P so that it is not insane
		int64_t scale = std::gcd(hs_norm[1].x, hs_norm[1].y);
		scale = std::gcd(scale, hs_norm[1].z);

		hs_norm[1].x /= scale;
		hs_norm[1].y /= scale;
		hs_norm[1].z /= scale;

		// Step 3 PxdP gives the normal to P and dP
		std::vector<int64_t> n(3);
		n[0] = hs_norm[1].y * hs_norm[1].dz - hs_norm[1].z * hs_norm[1].dy;
		n[1] = hs_norm[1].z * hs_norm[1].dx - hs_norm[1].x * hs_norm[1].dz;
		n[2] = hs_norm[1].x * hs_norm[1].dy - hs_norm[1].y * hs_norm[1].dx;

		scale = std::gcd(n[0], n[1]);
		scale = std::gcd(scale, n[2]);
		
		// rescale to keep the numbers managable
		n[0] /= scale;
		n[1] /= scale;
		n[2] /= scale;

		int64_t t_top = (hs_norm[2].x * n[0] + hs_norm[2].y * n[1] + hs_norm[2].z * n[2]);
		int64_t t_bottom = (hs_norm[2].dx * n[0] + hs_norm[2].dy * n[1] + hs_norm[2].dz * n[2]);

		scale = std::gcd(t_top, t_bottom);

		t_top /= scale;
		t_bottom /= scale;

		std::vector<int64_t> pp(3);
		
		pp[0] = -hs_norm[2].x + (t_top / t_bottom) * hs_norm[2].dx;
		pp[1] = -hs_norm[2].y + (t_top / t_bottom) * hs_norm[2].dy;
		pp[2] = -hs_norm[2].z + (t_top / t_bottom) * hs_norm[2].dz;

		scale = pp[0];
		for (auto pr : pp) {
			if (pr != 0) {
				scale = std::gcd(scale, pr);
			}
		}

		pp[0] /= scale;
		pp[1] /= scale;
		pp[2] /= scale;

		// This is the path of the rock in the shifted frame of reference
		// We will need to shift the start vector (currently 0) to ensure that
		// we hit all the hailstones.
		HailStone rock;
		// make sure the vector is pointing in the correct direction
		if (AH::sgn(t_top) != AH::sgn(t_bottom)) {
			rock = HailStone(0,0,0, pp[0], pp[1], pp[2]);
		} else {
			rock = HailStone(0,0,0,-pp[0],-pp[1],-pp[2]);
		}

		int64_t shiftBy = 0;
		// iterate ove all the hailstones (in the new frame of reference) to
		// find the largest shift - this will be our starting position.
		for (auto h : hs) {
			auto hh = HailStone(h.x-hs[0].x, h.y-hs[0].y, h.z-hs[0].z,
			h.dx-hs[0].dx, h.dy-hs[0].dy, h.dz-hs[0].dz);
			const auto temp = findShift(rock, hh);
			shiftBy = std::max(shiftBy, temp);
		}

		// shift the rock to the correct position
		rock.x -= shiftBy *pp[0];
		rock.y -= shiftBy *pp[1];
		rock.z -= shiftBy *pp[2];

		// sum the values in the original frame of reference
		int64_t soln = rock.x + hs[0].x + rock.y + hs[0].y + rock.z + hs[0].z;

		return soln;
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

		int64_t part1 = 0;
		for (size_t i = 0; i < hs.size(); ++i) {
			for (size_t j = i + 1; j < hs.size(); ++j) {
				part1 += intersectXY(hs[i], hs[j]);
			}
		}

		int64_t part2 = throwRock(hs);
		AH::PrintSoln(24, part1, part2);

		return 0;
	}

}

#endif
