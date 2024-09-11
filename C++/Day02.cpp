#ifdef DAY02
#include "AH.h"

namespace Day02
{

	std::map<std::string, int> maxSeen(const std::string game)
	{
		std::map<std::string, int> colours = {
		  { "r", 0 }, { "g", 0 }, { "b", 0 }
		};

		const int space = game.find(":");
		std::string parsed = game.substr(space+1);
		const auto ss = AH::Split(parsed, ';');

		for (auto s : ss) {
			const auto cols = AH::Split(s, ',');
			for (auto col : cols) {
				const int gap = col.find(" ", 1);
				const int int_part = stoi(col.substr(1, gap));
				const std::string col_part = col.substr(gap+1, 1);

				if (colours.at(col_part) < int_part) {
					colours.at(col_part) = int_part;
				}
			}
		}

		return colours;
	}

	bool validateGame(const std::string game,
		const std::map<std::string, int> target, int & part2)
	{
		const auto seen = maxSeen(game);

		int prod = 1;
		for (auto & x: seen) {
			prod *= x.second;
		}
		part2 += prod;

		for (auto & x: target) {
			if (x.second < seen.at(x.first)) {
				return false;
			}
		}
		return true;
	}

	int Run(const std::string& filename)
	{
		const auto inputLines = AH::ReadTextFile(filename);
		const std::map<std::string, int> target =
		{ {"r", 12}, {"g", 13}, {"b", 14} };

		int part1 = 0, part2 = 0;
		for (size_t i = 0; i < inputLines.size(); ++i) {
			if (validateGame(inputLines[i], target, part2)) {
				part1 += (int)(i + 1);
			}
		}

		AH::PrintSoln(2, part1, part2);

		return 0;
	}

}

#endif
