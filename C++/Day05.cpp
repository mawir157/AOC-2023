#include "AH.h"

namespace Day05
{

	typedef std::pair<uint64_t, uint64_t> Range;
	
	std::vector<uint64_t> parseSeeds(std::string s) {
		std::vector<uint64_t> ints;

		const auto ss = s.substr(7);
		const auto sis = AH::Split(ss, ' ');

		for (auto si : sis) {
			ints.push_back(AH::stoui64(si));
		}

		return ints;
	}

	std::vector<Range> collapseRanges(std::vector<Range> ranges) {
		std::vector<Range> new_ranges;
		std::sort(ranges.begin(), ranges.end(), [](auto &left, auto &right) {
    	return left.first < right.first;
		});

		Range r_head = ranges[0];
		for (size_t i = 1; i < ranges.size(); ++i) {
			if (ranges[i].first < r_head.first + r_head.second) {
				r_head.second = r_head.first + r_head.second - ranges[i].first;
			} else {
				new_ranges.push_back(r_head);
				r_head = ranges[i];
			}
		}
		new_ranges.push_back(r_head);
		return new_ranges;
	}

	std::vector<Range> parseSeedsToRanges(std::string s) {
		std::vector<Range> ranges;

		const auto ss = s.substr(7);
		const auto sis = AH::Split(ss, ' ');

		for (size_t i = 0; i < sis.size(); i += 2) {
			ranges.emplace_back(AH::stoui64(sis[i]), AH::stoui64(sis[i+1]));
		}

		return collapseRanges(ranges);
	}

	struct mapping
	{
		uint64_t r;
		uint64_t s;
		uint64_t t;

		mapping(const uint64_t r, const uint64_t s, const uint64_t t) :
		r(r), s(s), t(t) {}
		mapping(std::string & s);
	};

	mapping::mapping(std::string & st) {
		const auto sis = AH::Split(st, ' ');
		r = AH::stoui64(sis[0]);
		s = AH::stoui64(sis[1]);
		t = AH::stoui64(sis[2]);
	}

	std::vector<mapping> parseMapping(const std::string s) {
		std::vector<mapping> ms;
		const int hdr = s.find(":");
		const std::string parsed = s.substr(hdr+1);
		const auto ps = AH::Split(parsed, '|');

		for (auto p : ps) {
			if (p.size() == 0) continue;
			mapping m(p);
			ms.push_back(m);
		}

		return ms;
	}

	uint64_t applyMappingsToInt(const uint64_t n, const std::vector<mapping> ms) {
		for (auto m :  ms) {
			if ((n - m.s) < m.t)
				return m.r + (n - m.s);
		}
		return n;
	}

	std::vector<Range> applyMappingToRange(const Range r, const mapping m) {
		std::vector<Range> new_ranges;
		uint64_t overlap_lo = 0;
		uint64_t overlap_hi = 0;
		if ( (r.first > (m.s + m.t)) || (m.s > (r.first + r.second)) ) {
			std::cout << "No Overlap" << std::endl;
			new_ranges.push_back(r);
		} else {
			overlap_lo = std::max(r.first, m.s);
			overlap_hi = std::min(r.first + r.second, m.s + m.t);
			if (overlap_lo != r.first) {
				new_ranges.emplace_back(r.first, overlap_lo - r.first);
			}
			std::cout << "Overlap = " << overlap_lo << "," << overlap_hi << std::endl;
			std::cout << "m = (" << m.r << "," << m.s << "," << m.t << ")\n";
			new_ranges.emplace_back(m.r + overlap_lo - m.s, overlap_hi - overlap_lo);
			if (overlap_hi != r.first + r.second) {
				new_ranges.emplace_back(overlap_hi, r.first + r.second - overlap_hi);
			}
		}
		return new_ranges;
	}

	std::vector<Range> applyMappingsToRange(const std::vector<Range> rs,
		const std::vector<mapping> ms) {
		std::vector<Range> new_ranges_1 = rs;
		std::vector<Range> new_ranges_2;
		for (auto m : ms) {
			for (auto r : new_ranges_1) {
				auto temp_ranges = applyMappingToRange(r, m);
				for (auto t : temp_ranges)
					new_ranges_2.push_back(t);
			}
			new_ranges_1 = new_ranges_2;
			new_ranges_2.clear();
		}

		return new_ranges_1;
	}

	int Run(const std::string& filename)
	{
		const auto inputLines = AH::ReadTextFile(filename);
		const auto ss = AH::ParseLineGroups(inputLines, '|');

		std::vector<mapping> m1 = parseMapping(ss[1]);

		std::vector<std::vector<mapping>> mss;
		for (size_t i = 1; i < ss.size(); ++i) {
			mss.push_back(parseMapping(ss[i]));
		}

		const auto seeds = parseSeeds(ss[0]);

		std::vector<uint64_t> seed_final(seeds.size());

		for (size_t i = 0; i < seeds.size(); ++i) {
			uint64_t seed = seeds[i];
			for (auto ms : mss) {
				seed = applyMappingsToInt(seed, ms);
			}
			seed_final[i] = seed;
		}

		uint64_t part1 = *std::min_element(seed_final.begin(), seed_final.end());

		auto seed_ranges = parseSeedsToRanges(ss[0]);
		for (auto r : seed_ranges) {
			std::cout << r.first << "," << r.second << std::endl;
		}
		std::cout << std::endl;

		for (auto ms : mss) {
			seed_ranges = applyMappingsToRange(seed_ranges, ms);
		for (auto r : seed_ranges) {
			std::cout << r.first << "," << r.second << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		// break;
		}

		// for (auto r : seed_ranges) {
		// 	std::cout << r.first << "," << r.second << std::endl;
		// }

		uint64_t part2 = 0;

		AH::PrintSoln(5, part1, part2);

		return 0;
	}

}
