#include "AH.h"

namespace Day12
{

	std::map<int64_t, int64_t> g_cache;

	int64_t numberOfSolns(const std::string & pattern, const std::vector<int> & is,
		const int pattern_idx, const int is_idx)
	{
		if (is_idx >= (int)is.size()) {
			// make sure we don't have any '#' left over
			if (pattern.find('#', pattern_idx) != std::string::npos) {
				return 0;
			}
			return 1; // nothing left to pack
		}

		// how much space do we need to pack the remaining blocks
 		int need = std::reduce(is.begin() + is_idx, is.end()) + is.size() - is_idx - 1;
		if (need > (int)pattern.size() - pattern_idx) {
			return 0;
		}

		int64_t waysToPack = 0;
		int64_t globaLookUp = pattern_idx * 100 + is_idx;
		if (g_cache.count(globaLookUp)) { // we've seen this situation before...
			//..so just return it
			return g_cache[globaLookUp];
		}
	  // try to fit the first blocks
	  int toFit = is[is_idx];
	  auto from = pattern.find_first_of("?#", pattern_idx);
	  if (from == std::string::npos) {
	  	// there are no ?s or #s left
	  	return 0;
	  }
	  for (size_t i = from;  i < pattern.size() - (toFit - 1); ++i) { // check for OBOE
		  // is there a . in the next toFit characters, if so can't write here
	  	if (pattern.find(".", i) < i + toFit) {
	  		continue;
	  	}

	  	// if there is a # before i we will miss it so there are no valid packings
	  	if (pattern.find('#', pattern_idx) < i) {
	  		break;
	  	}

	  	// we can write out block here so check the next char isn't a #
	  	// some optimisation here if is.size() == 1
	  	if (i + toFit < pattern.size()) {
	  		if (pattern.at(i + toFit) == '#') {
	  			continue;
	  		}
	  	}

	  	// at this point we can def write out so shift indices and recur
	  	auto next_pattern_idx = pattern.find_first_of("?#", i + toFit + 1);
	  	waysToPack += numberOfSolns(pattern, is, next_pattern_idx, is_idx + 1);
	  }

	  // cache this result so other branches can use it
	  g_cache[globaLookUp] = waysToPack;
	  return waysToPack;
	}

	int64_t parseInput(const std::string & s, const bool expand)
	{
		const auto ps = AH::Split(s, ' ');
		const auto qs = AH::Split(ps[1], ',');
		std::vector<int> partitions;
		for (auto q : qs) {
			partitions.push_back(std::stoi(q));
		}

		// reset cache before we start
		g_cache.clear();

		if (expand) {
			std::string str5 = ps[0];
			std::vector<int> parts5;

			for (auto i = 0; i < 4; ++ i) {
				str5 += '?'; // READ THE BLOODY RULES
				str5 += ps[0];
			}

			for (auto i = 0; i < 5; ++i) {
    		std::copy(partitions.begin(), partitions.end(), std::back_inserter(parts5));
			}

			return numberOfSolns(str5, parts5, 0, 0);
		}

		return numberOfSolns(ps[0], partitions, 0, 0);
	}

	int64_t Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);

		int part1 = 0;
		int64_t part2 = 0;
		int line = 1;
		for (auto l : ls) {
			part1 += parseInput(l, false);
			part2 += parseInput(l, true);
			line++;
		}

		AH::PrintSoln(12, part1, part2);

		return 0;
	}

}
