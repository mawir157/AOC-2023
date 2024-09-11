#ifdef DAY09

#include "AH.h"

namespace Day09
{

	std::vector<int64_t> parseInput(const std::string s, const bool part2=false)
	{
		std::vector<int64_t> vs;
		const auto ts = AH::SplitOnString(s, " ");

		for (auto t : ts) {
			vs.push_back(AH::stoui64(t));
		}

		if (part2) {
			std::reverse(vs.begin(), vs.end());
		}

		return vs;
	}

	std::vector<int64_t> diff(const std::vector<int64_t> vs)
	{
		std::vector<int64_t>	 df(vs.size() - 1);

		for (size_t i = 0; i < df.size(); ++i) {
			df[i] = vs[i+1] - vs[i];
		}

		return df;
	}

	int64_t Newton(std::vector<int64_t> vs)
	{
		if (std::equal(vs.begin() + 1, vs.end(), vs.begin()) ) {
			return vs.front();
		}

		const auto df = diff(vs);
		return Newton(df) + vs.back();
	}

	int Run(const std::string& filename)
	{
		const auto inputLines = AH::ReadTextFile(filename);

		int64_t part1 = 0, part2 = 0;
		for (auto l : inputLines) {
			part1 += Newton(parseInput(l));
			part2 += Newton(parseInput(l, true));
		}

		AH::PrintSoln(9, part1, part2);

		return 0;
	}

}

#endif
