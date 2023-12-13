#include "AH.h"

namespace Day12
{

	int numberOfSolns(const std::string & pattern, const std::vector<int> & is)
	{
		if (is.size() == 0) {
			// make sure we don't have any '#' left over
			if (pattern.find('#') != std::string::npos) {
				return 0;
			}
			return 1; // nothing left to pack
		}

		// how much space do we need to pack the remaining blocks
 		int need = std::reduce(is.begin(), is.end()) + is.size() - 1;
		if (need > (int)pattern.size()) {
			return 0;
		}

		int waysToPack = 0;
	  // try to fit the first blocks
	  int toFit = is[0];
	  int from = pattern.find_first_of("?#");
	  if (from == (int)std::string::npos) {
	  	// there are no ?s or #s left
	  	return 0;
	  }
	  for (int i = from;  i < (int)pattern.size() - (toFit - 1); ++i) { // check for OBOE
	  	if (pattern.at(i) == '.') {
	  		continue;
	  	}

		  // is there a . in the next toFit characters, if so can't write here
	  	if (pattern.find(".", i) < i + toFit) {
	  		continue;
	  	}

	  	// if there is a # before i we will miss it so there are no valid packings
	  	if (pattern.find('#') < i) {
	  		break;
	  	}

	  	// we can write out block here so check the next char isn't a #
	  	// some optimisation here if is.size() == 1
	  	if (i + toFit < (int)pattern.size()) {
	  		if (pattern.at(i + toFit) == '#') {
	  			continue;
	  		}
	  	}

	  	// at this point we can def write out so truncate, pop and recur
	  	auto new_pattern = pattern;
	  	new_pattern.erase(0, i + toFit + 1);
	  	auto new_is = is;
	  	new_is.erase(new_is.begin());
	  	waysToPack += numberOfSolns(new_pattern, new_is);
	  }

	  return waysToPack;
	}

	int parseInput(const std::string & s, const bool expand)
	{
		const auto ps = AH::Split(s, ' ');
		const auto qs = AH::Split(ps[1], ',');
		std::vector<int> partitions;
		for (auto q : qs) {
			partitions.push_back(std::stoi(q));
		}

		printf("%s ", ps[0].c_str());
		for (auto p : partitions) {
			printf("%d,", p);
		}
		printf("\n");

		if (expand) {
			std::string str5;
			std::vector<int> parts5;

			for (auto i = 0; i < 5; ++i) {
    		std::copy(ps[0].begin(), ps[0].end(), std::back_inserter(str5));
    		std::copy(partitions.begin(), partitions.end(), std::back_inserter(parts5));
			}

			printf("%s ", str5.c_str());
			for (auto p : parts5) {
				printf("%d,", p);
			}
			printf("\n");

			return numberOfSolns(str5, parts5);
		}

		return numberOfSolns(ps[0], partitions);
	}

	int64_t Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);

		int part1 = 0;
		int part2 = 0;
		for (auto l : ls) {
			part1 += parseInput(l, false);
			part2 += parseInput(l, true);
		}
		
		AH::PrintSoln(12, part1, part2);

		return 0;
	}

}
