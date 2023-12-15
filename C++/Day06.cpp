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
		uint64_t min_lo = 0;
		uint64_t min_hi = time / 2;

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

	std::vector<std::pair<uint64_t, uint64_t>> parseInput(
		std::string times, std::string distances, const bool part2=false)
	{
		if (part2) {
			times.erase(std::remove_if(times.begin(),  times.end(),
			            [](unsigned char x) { return std::isspace(x); }),
			            times.end());
			distances.erase(std::remove_if(distances.begin(),  distances.end(),
			                [](unsigned char x) { return std::isspace(x); }),
			                distances.end());
		}
		
		auto colon = times.find(":");
		auto temp = times.substr(colon+1);
		std::vector<std::pair<uint64_t, uint64_t>> pairs;
		auto ps = AH::Split(temp, ' ');
		for (auto p : ps) {
			if (p.size() > 0) {
				pairs.emplace_back(AH::stoui64(p), 0);
			}
		}

		colon = distances.find(":");
		temp = distances.substr(colon+1);
		ps = AH::Split(temp, ' ');
		int i = 0;
		for (auto p : ps) {
			if (p.size() > 0) {
				pairs[i].second = AH::stoui64(p);
				++i;
			}
		}


		return pairs;
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);
		const auto input1 = parseInput(ls[0], ls[1]);
		const auto input2 = parseInput(ls[0], ls[1], true)[0];
		uint64_t part1 = 1;
		for (auto [time, dist] : input1) {
			part1 *= race(time, dist);
		}

		uint64_t part2 = race(input2.first, input2.second);

		AH::PrintSoln(6, part1, part2);

		return 0;
	}

}
