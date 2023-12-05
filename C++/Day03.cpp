#include "AH.h"

namespace Day03
{

	std::pair<int, int> fn(const std::vector<std::string> ss) {
		int part1 = 0;
		int part2 = 0;
		std::map<int, int> gears;
		for (int r = 0; r < ss.size(); ++r) {
			const auto s = ss[r];
			for (int c = 0; c < s.size(); ++c) {
				const auto ch = s.at(c);
				if (isdigit(ch)) {
					auto idx = s.find_first_not_of("0123456789", c);
					if (idx == std::string::npos) // the number runs right to the edge of the grid
						idx = s.size();
					// does string have a neighbouring symbol?
					bool is_good = false;
					int gear_idx = 0;
					const auto digits = s.substr(c, idx - c);
					for (int ir = std::max(0, r - 1); ir < std::min((int)ss.size(), r + 2); ++ir) {
						for (int ic = std::max(0, c - 1); ic < std::min((int)s.size(), (int)idx + 1); ++ic) {
							const auto qs = ss[ir];
							if ((qs.at(ic) != '.') && (!isdigit(qs.at(ic)))) {
								is_good = true;
								if (qs.at(ic) == '*') {
									gear_idx = ir * s.size() + ic;
								}
								break;
							}
						}
						if (is_good) break;
					}

					if (is_good) {
						const auto v = stoi(s.substr(c, idx - c));
						part1 += v;
						if (gear_idx != 0) {
							if (gears.count(gear_idx)) // we have seen this gear before
								part2 += gears[gear_idx] * v;
							else
								gears[gear_idx] += v;
						}
					}

					c = idx;
				}
			}
		}

		return std::make_pair(part1, part2);
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> inputLines = AH::ReadTextFile(filename);

		auto [part1, part2] = fn(inputLines);

		AH::PrintSoln(3, part1, part2);

		return 0;
	}

}
