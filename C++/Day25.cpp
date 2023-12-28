#include "AH.h"

namespace Day25
{

	typedef std::vector<std::vector<int>> Graph;
	
	Graph parseInput(const std::vector<std::string> input)
	{
		Graph adj;
		std::map<std::string, size_t> nameToIndex;
		// first pass get all the node names
		for (auto l : input) {
			int i = nameToIndex.size();
			const auto name = l.substr(0, 3);
			if (nameToIndex.count(name) == 0) {
				nameToIndex[name] = i;
				// g.indexToName[i] = name;
			}
			const auto to = AH::SplitOnString(l.substr(5), " ");
			for (auto t : to) {
				i = nameToIndex.size();
				if (nameToIndex.count(t) == 0) {
					nameToIndex[t] = i;
					// g.indexToName[i] = t;
				}
			}
		}

		// second pass fill in the adjacency matrix
		adj = std::vector<std::vector<int> >(nameToIndex.size(),
		std::vector<int>(nameToIndex.size()));

		for (auto l : input) {
			const auto from = l.substr(0, 3);
			const auto from_idx = nameToIndex[from];
			const auto to = AH::SplitOnString(l.substr(5), " ");
			for (auto t : to) {
				const auto to_idx = nameToIndex.at(t);
				adj[from_idx][to_idx] = 1;
				adj[to_idx][from_idx] = 1;
			}
		}

		return adj;
	}

	// Stolen from Wikipedia 'cause I'm a scrub who knows nothing about
	// computation graph theory
	std::pair<int, std::vector<int>> StoerWagner(Graph mat) {
		std::pair<int, std::vector<int>> best = {INT_MAX, {}};
		int n = mat.size();
		std::vector<std::vector<int>> co(n);

		for (int i = 0; i < n; i++)
			co[i] = {i};

		for (int ph = 1; ph < n; ph++) {
			std::vector<int> w = mat[0];
			size_t s = 0, t = 0;
			for (int it = 0; it < n - ph; it++) { // O(V^2) -> O(E log V) with prio. queue
				w[t] = INT_MIN;
				s = t, t = max_element(w.begin(), w.end()) - w.begin();
				for (int i = 0; i < n; i++) w[i] += mat[t][i];
			}
			best = min(best, {w[t] - mat[t][t], co[t]});
			co[s].insert(co[s].end(), co[t].begin(), co[t].end());
			for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
			for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
			mat[0][t] = INT_MIN;
		}

		return best;
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);
		auto g = parseInput(ls);

		auto [s, w] = StoerWagner(g);

		auto part1 = w.size() * (g.size() - w.size());
		AH::PrintSoln(25, part1, "See you next year.");

		return 0;
	}

}
