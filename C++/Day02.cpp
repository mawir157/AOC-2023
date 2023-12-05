#include "AH.h"

namespace Day02
{

	std::map<std::string, int> maxSeen(const std::string game)
	{
		std::map<std::string, int> colours = {
                { "r", 0 },
                { "g", 0 },
                { "b", 0 } };

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

	bool validateGame(const std::map<std::string, int> seen,
		const std::map<std::string, int> target, int & part2) {

		int ret = 1;
		for (auto& x: seen) {
			ret *= x.second;
		}
		part2 += ret;

		for (auto& x: target) {
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

		int i = 0;
		int part1 = 0, part2 = 0;
		for (auto l : inputLines) {
			i++;
			const auto m = maxSeen(l);

			if (validateGame(m , target, part2)) {
				part1 += i;
			}
		}		

		AH::PrintSoln(2, part1, part2);

		return 0;
	}

}
