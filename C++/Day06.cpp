#include "AH.h"

namespace Day06
{
	/*
	I don't want to have to resort to floats for an integer problem 
	Leave this here as a warning to others...
	uint64_t race2(const uint64_t time, const uint64_t distance)
	{
		const double t_d = (double)time;
		const double d_d = (double)distance;
		const double discriminant = t_d * t_d - 4.0 * d_d;

		const auto min_c_d = 0.5 * (t_d - std::sqrt(discriminant));
		const auto min_c = (uint64_t)std::ceil(min_c_d);

		const auto max_c_d = 0.5 * (t_d + std::sqrt(discriminant));
		const auto max_c = (uint64_t)std::floor(max_c_d);

		return max_c - min_c + 1;
	}
	*/

	// use a binary search to find the roots of the quadratic
	uint64_t race(const uint64_t time, const uint64_t distance)
	{
		const uint64_t half_t = time / 2;
		uint64_t min_lo = 0;
		uint64_t min_hi = half_t;

		while (min_hi - min_lo > 1) {
			const uint64_t cand = (min_hi + min_lo) / 2;
			if (cand * (time - cand) > distance) {
				min_hi = cand;
			} else {
				min_lo = cand;
			}
		}
		return time - min_lo - min_hi;
	}

	int Run(const std::string& filename)
	{
		std::vector<std::pair<uint64_t, uint64_t>> input1 = { 
			{52, 426}, {94, 1374}, {75, 1279}, {94, 1216}
		};
		std::pair<uint64_t, uint64_t> input2 = { 52947594, 426137412791216 };
		uint64_t part1 = 1;
		for (auto [time, dist] : input1) {
			part1 *= race(time, dist);
		}

		uint64_t part2 = race(input2.first, input2.second);

		AH::PrintSoln(6, part1, part2);

		return 0;
	}

}