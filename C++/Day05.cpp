#ifdef DAY05

#include "AH.h"

namespace Day05
{

	typedef std::pair<uint64_t, uint64_t> Range;

	std::vector<uint64_t> parseSeeds(std::string s)
	{
		std::vector<uint64_t> ints;

		const auto ss = s.substr(7);
		const auto sis = AH::Split(ss, ' ');

		for (auto si : sis) {
			ints.push_back(AH::stoui64(si));
		}

		return ints;
	}

	std::list<Range> parseSeedsToRanges(std::string s)
	{
		std::list<Range> ranges;

		const auto ss = s.substr(7);
		const auto sis = AH::Split(ss, ' ');

		for (size_t i = 0; i < sis.size(); i += 2) {
			ranges.emplace_back(AH::stoui64(sis[i]), AH::stoui64(sis[i+1]));
		}

		return ranges;
	}

	struct mapping
	{
		uint64_t r;
		uint64_t s;
		uint64_t t;

		mapping(std::string & s);
	};

	mapping::mapping(std::string & st)
	{
		const auto sis = AH::Split(st, ' ');
		r = AH::stoui64(sis[0]);
		s = AH::stoui64(sis[1]);
		t = AH::stoui64(sis[2]);
	}

	std::vector<mapping> parseMapping(const std::string s)
	{
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

	uint64_t applyMappingsToInt(const uint64_t n, const std::vector<mapping> ms)
	{
		for (auto m :  ms) {
			if ((n - m.s) < m.t)
				return m.r + (n - m.s);
		}
		return n;
	}

	void applyMappingToRanges(std::list<Range> & pre, const std::vector<mapping> ms)
	{
		std::list<Range> post; // done, we pass these onto the next mapping group
		std::list<Range> next; // can still be affected by a mapping in this group
		for (auto m : ms) {
			while (pre.size() > 0) {
				auto r = pre.front();
				pre.pop_front();
				// if there is no overlap - this range remains unchanged
				// so stick it unchanged into next since another interval could affect it
				if ( (r.first >= (m.s + m.t)) || (m.s >= (r.first + r.second)) ) {
					next.push_back(r);
				} else {
					// the interval of overlap
					uint64_t overlap_lo = std::max(r.first, m.s);
					uint64_t overlap_hi = std::min(r.first + r.second, m.s + m.t);

					// there is an untouched interval at the start...
					if (overlap_lo > r.first) {
						// ...so stick that on the end of the list to process
						next.emplace_back(r.first, overlap_lo - r.first);
					}

					// the region of overlap is reallocated, and not touched again in this group
					post.emplace_back(m.r + overlap_lo - m.s, overlap_hi - overlap_lo);

					// there is an untouched interval at the end...
					if (overlap_hi < r.first + r.second) {
						// ... so stick that on the end of the list to process
						next.emplace_back(overlap_hi, r.first + r.second - overlap_hi);
					}
				}
			}
			// these are the intervals that could still be affected by mappings in this group
			pre = next;
			next.clear();
		}

		pre.insert(pre.end(), post.begin(), post.end());

		return;
	}

	int Run(const std::string& filename)
	{
		const auto inputLines = AH::ReadTextFile(filename);
		const auto ss = AH::ParseLineGroups(inputLines, '|');

		std::vector<std::vector<mapping>> mss;
		for (size_t i = 1; i < ss.size(); ++i) {
			mss.push_back(parseMapping(ss[i]));
		}

		const auto seeds = parseSeeds(ss[0]);

		uint64_t part1 = std::numeric_limits<uint64_t>::max();
		for (auto seed : seeds) {
			for (auto ms : mss) {
				seed = applyMappingsToInt(seed, ms);
			}
			part1 = std::min(part1, seed);
		}

		auto seed_ranges = parseSeedsToRanges(ss[0]);
		for (auto ms : mss) {
			applyMappingToRanges(seed_ranges, ms);
		}

		uint64_t part2 = std::numeric_limits<uint64_t>::max();
		for (auto r : seed_ranges) {
			part2 = std::min(r.first, part2);
		}

		AH::PrintSoln(5, part1, part2);

		return 0;
	}

}

#endif
