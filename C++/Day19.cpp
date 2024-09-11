#ifdef DAY19

#include "AH.h"

namespace Day19
{
	
	struct Instruction
	{
		char mode;
		char comp;
		int value;
		std::string out;

		Instruction(const char mode, const char comp, const int value,
		const std::string out) :
		  mode(mode), comp(comp), value(value), out(out) {};
		Instruction() : comp('#'), value(-1), out("BAD") {};
	};

	std::map<char, int> modeToIndex { {'x', 0}, {'m', 1}, {'a', 2}, {'s', 3} };

	struct Workflow
	{
		std::vector<Instruction> ins;
		std::string fallback;
	};

	typedef std::vector<std::pair<int, int>> Ranges;

	typedef std::vector<int> Part;

	
	std::map<std::string, Workflow> getWorkflows(const std::string ss)
	{
		std::map<std::string, Workflow> wfs;
		
		const auto ll = AH::SplitOnString(ss, "|");
		for (auto l : ll) {
			const auto bra = l.find('{');
			const auto ket = l.find('}');
			const auto prefix = l.substr(0, bra);
			const auto contents = l.substr(bra + 1, ket - bra - 1);

			const auto cs = AH::SplitOnString(contents, ",");

			Workflow wf;
			for (auto c : cs) {
				if (c.find(':') != std::string::npos) {
					const auto qq = AH::SplitOnString(c, ":");
					const char mode = qq[0].at(0);
					const char comp = qq[0].at(1);
					const int value = stoi(qq[0].substr(2));
					const std::string out = qq[1];
					const Instruction in(mode, comp, value, out);

					wf.ins.push_back(in);
				} else {
					wf.fallback = c;
				}
			}
			wfs[prefix] = wf;
		}

		return wfs;
	}

	std::vector<Part> getParts(const std::string ss)
	{
		const auto ll = AH::SplitOnString(ss, "|");
		std::vector<Part> parts;

		for (auto l : ll) {
			Part pt(4, -1);
			const std::string trimmed = l.substr(1, l.size() - 2);
			const auto qq = AH::SplitOnString(trimmed, ",");

			for (auto q : qq) {
				const auto mode = q.at(0);
				const auto int_str = q.substr(2);
				const int i = stoi(int_str);
				pt[modeToIndex[mode]] = i;
			}

			parts.push_back(pt);
		}

		return parts;
	}

	std::string applySingleWorkflow(const Part p, const Workflow wf)
	{
		for (auto is : wf.ins) { // resolve instructions in order
			const auto idx = modeToIndex[is.mode];
			const auto value = p[idx];

			const auto c = is.comp;
			if (c == '>') {
				if (value > is.value) {
					return is.out;
				}
			} else {
				if (value < is.value) {
					return is.out;
				}
			}
		}
		// failed to find matching order so use fallback
		return wf.fallback;
	}

	int applyWorkflows(const Part p, const std::map<std::string, Workflow> wfs)
	{
		Workflow wf = wfs.at("in"); // we start here
		std::string here = applySingleWorkflow(p, wf);

		while ((here != "A") && (here != "R")) {
			wf = wfs.at(here);
			here = applySingleWorkflow(p, wf);
		}

		if (here == "A") {
			return std::accumulate(std::begin(p), std::end(p), 0);
		} else {
			return 0;
		}
	}

	int64_t rangeSize(const Ranges r)
	{
		int64_t ret = 1;
		for (auto [lo, hi] : r) {
			ret *= (hi - lo + 1);
		}

		return ret;
	}

	int64_t applyWorkToRanges(Ranges rng, const std::map<std::string, Workflow> wfs,
		const std::string here)
	{
		int64_t ret = 0;
		Workflow wf = wfs.at(here); // we start here
		for (auto i : wf.ins) {
			const auto c = i.comp;
			if (c != '#') { // this is the final instruction, with no conditions
				if (c == '>') {
					auto new_rng = rng;
					auto idx = modeToIndex[i.mode];
					if (new_rng[idx].second > i.value) {
						new_rng[idx].first = std::max(new_rng[idx].first, i.value + 1);
						if (i.out == "A") {
							ret += rangeSize(new_rng);
						} else if (i.out != "R") {
							ret += applyWorkToRanges(new_rng, wfs, i.out);
						}
						rng[idx].second = std::min(rng[idx].second, i.value);
					}
				}
				if (c == '<') {
					auto new_rng = rng;
					auto idx = modeToIndex[i.mode];
					if (new_rng[idx].first < i.value) {
						new_rng[idx].second = std::min(new_rng[idx].second, i.value - 1);
						if (i.out == "A") {
							ret += rangeSize(new_rng);
						} else if (i.out != "R") {
							ret += applyWorkToRanges(new_rng, wfs, i.out);
						}
						rng[idx].first = std::max(rng[idx].first, i.value);
					}
				}
			} else{
			}
		}
		// applied all the legit instruction so finally apply the fallback condition
		if (wf.fallback == "A") {
			ret += rangeSize(rng);
		} else if (wf.fallback != "R") {
			ret += applyWorkToRanges(rng, wfs, wf.fallback);
		}
		return ret;
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);
		const auto gs = AH::ParseLineGroups(ls, '|');
		const auto wfs = getWorkflows(gs[0]);
		const auto parts = getParts(gs[1]);

		int64_t part1 = 0;
		int64_t part2 = 0;

		for (auto p : parts) {
			part1 += applyWorkflows(p, wfs);
		}

		Ranges rng = { {1,4000}, {1,4000}, {1,4000}, {1,4000} };
		part2 = applyWorkToRanges(rng, wfs, "in");

		AH::PrintSoln(19, part1, part2);

		return 0;
	}

}
   
#endif
 