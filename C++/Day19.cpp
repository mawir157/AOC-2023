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

		AH::PrintSoln(19, part1, part2);

		return 0;
	}

}
    