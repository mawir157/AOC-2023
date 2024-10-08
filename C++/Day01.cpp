#ifdef DAY01

#include "AH.h"

namespace Day01
{

	const std::map<std::string, int> lut = 
	{{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
	 {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"zero", 0}};

	int calibrationValue(const std::string cal, const bool part2=false)
	{
		int ret = 0;
		for (size_t i = 0; i < cal.size(); ++i) {
			const char c = cal[i];
			if ( isdigit(c) ) {
				ret += 10*(c - '0');
				break;
			}
			if (part2) {
				bool brk = false;
				for (auto const& [key, val] : lut) {
					if (cal.compare(i, key.size(), key) == 0) {
						ret += 10 * val;
						brk = true;
						break;
					}
				}
				if (brk) break;
			}
		}

		for (int i = (int)cal.size() - 1; i >= 0; --i) {
			const char c = cal[i];
			if ( isdigit(c) ) {
				ret += (c - '0');
				break;
			}

			if (part2) {
				bool brk = false;
				for (auto const& [key, val] : lut) {
					if (cal.compare(i, key.size(), key) == 0) {
						ret += val;
						brk = true;
						break;
					}
				}
				if (brk) break;
			}
		}
		return ret;
	}

	int Run(const std::string& filename)
	{
		const auto inputLines = AH::ReadTextFile(filename);
		int part1 = 0, part2 = 0;

		for (auto e : inputLines) {
			part1 += calibrationValue(e);
			part2 += calibrationValue(e, true);
		}

		AH::PrintSoln(1, part1, part2);

		return 0;
	}

}

#endif
