#ifdef DAY11

#include "AH.h"

namespace Day11
{

	int64_t	 parseInput(std::vector<std::string> ss, const int64_t expansion = 1)
	{
		int64_t row_offset = 0;
		std::vector<int64_t> empty_cols;

		// find empty cols
		for (int64_t ic = 0; ic < (int64_t)ss[0].size(); ++ic) {
			int64_t occpd = false; 
			for (int64_t ir = 0; ir < (int64_t)ss.size(); ++ir) {
				if (ss[ir].at(ic) == '#') {
					occpd = true;
					break;
				}
			}
			if (!occpd) {
				empty_cols.push_back(ic);
			}
		}

		std::vector<std::pair<int64_t, int64_t>> ps;
		for (int64_t ir = 0; ir < (int64_t)ss.size(); ++ir) {
			const auto s = ss[ir];
			if (s.find_first_not_of(s[0]) == std::string::npos) {
				row_offset += expansion;
			}
			int64_t col_offset = 0;
			for (int64_t ic = 0; ic < (int64_t)s.size(); ++ic) {
				// this column is empty
				if(std::find(empty_cols.begin(), empty_cols.end(), ic) != empty_cols.end()) {
					col_offset += expansion;
				}
				if (s.at(ic) == '#') {
					ps.emplace_back(ir + row_offset, ic + col_offset);
				}
			}
		}

		const int64_t gals = ps.size();

		int64_t ret = 0;
		for (int64_t i = 0; i < gals; ++i) {
			for (int64_t j = i + 1; j < gals; ++j) {
				ret += std::abs(ps[i].first - ps[j].first) +
				       std::abs(ps[i].second - ps[j].second);
			}
		}

		return ret;
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);

		AH::PrintSoln(11, parseInput(ls), parseInput(ls, 999999));

		return 0;
	}

}

#endif
