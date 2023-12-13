#include "AH.h"

namespace Day08
{
	typedef std::map<std::string, std::vector<std::string>> Rules;

	std::vector<int> parseInstruction(const std::string s)
	{
		std::vector<int> ret;
		for (auto & c : s) {
			ret.push_back( (c == 'R' ? 1 : 0) );
		}

		return ret;
	}

	Rules parseRules(const std::vector<std::string> ss)
	{
		Rules rs;

		for (auto & s : ss) {
			if (s.find("=") == std::string::npos) continue;

			const auto t = AH::SplitOnString(s, " = ");
			const auto key = t[0];

			const auto brackets = t[1].substr(1, t[1].size() - 2);
			const auto pair = AH::SplitOnString(brackets, ", ");

			rs[key] = {pair[0], pair[1]};
		}

		return rs;
	}

	int runUntilMatch(const std::string src, const std::string tgt,
		const std::vector<int> ins, const Rules rs, const bool part2=false)
	{
		int i = 0;
		auto cur = src;
		while (true) {
			for (auto in : ins) {
				cur = rs.at(cur)[in];
				i++;
				if (!part2) {
					if (cur == tgt) {
						return i;
					}
				} else {
					if (cur.back() == tgt.back()) {
						return i;
					}
				}
			}
		}
		return 0;
	}

	std::vector<std::string> findNodesEndingIn(const Rules rs, const char c)
	{
		std::vector<std::string> goodNodes;
		for (auto & [k, v] : rs) {
			if (k.back() == c) {
				goodNodes.push_back(k);
			}
		}

		return goodNodes;
	}

	int Run(const std::string& filename)
	{
		const auto inputLines = AH::ReadTextFile(filename);
		const auto ins = parseInstruction(inputLines[0]);
		const auto rs = parseRules(inputLines);

		int part1 = 0;
		uint64_t part2 = 1;
		part1 = runUntilMatch("AAA", "ZZZ", ins, rs);

		auto gn = findNodesEndingIn(rs, 'A');
		for (auto n : gn) {
			part2 = std::lcm(part2, runUntilMatch(n, "ZZZ", ins, rs, true));
		}

		AH::PrintSoln(8, part1, part2);

		return 0;
	}

}
