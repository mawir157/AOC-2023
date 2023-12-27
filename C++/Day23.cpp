#include "AH.h"

namespace Day23
{
	typedef std::vector<std::vector<char>> Grid;
	typedef std::pair<size_t, size_t> Pos;

	Grid parseInput(const std::vector<std::string> ls, size_t & inCol, size_t & outCol)
	{
		Grid g;
		for (size_t r = 0; r < ls.size(); ++r) {
			const auto row = ls[r];
			std::vector<char> rv;
			for (size_t c = 0; c < row.size(); ++c) {
				const auto ch = row.at(c);
				if ((r == 0) && (ch == '.')) {
					inCol = c;
				}
				
				if ((r == ls.size()-1) && (ch == '.')) {
					outCol = c;
				}

				rv.push_back(ch);
			}
			g.push_back(rv);
		}

		return g;
	}

	std::vector<Pos> getNbrs(const Pos & here, const Pos & prev, const Grid g)
	{
		std::vector<Pos> nbrs;
		const auto r = here.first;
		const auto c = here.second;
		// North
		if (here.first != 0) {
			if ((g[r-1][c] == '.') || (g[r-1][c] == '^')) {
				Pos n(r-1, c);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// East
		if (here.second != g.size() - 1) {
			if ((g[r][c+1] == '.') || (g[r][c+1] == '>')) {
				Pos n(r, c+1);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// South
		if (here.first != g.size() - 1) {
			if ((g[r+1][c] == '.') || (g[r+1][c] == 'v')) {
				Pos n(r+1, c);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// West
		if (here.second != 0) {
			if ((g[r][c-1] == '.') || (g[r][c-1] == '<')) {
				Pos n(r, c-1);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}

		return nbrs;
	}

	std::vector<Pos> getNbrs2(const Pos & here, const Pos & prev, const Grid g)
	{
		std::vector<Pos> nbrs;
		const auto r = here.first;
		const auto c = here.second;
		// North
		if (here.first != 0) {
			if (g[r-1][c] != '#') {
				Pos n(r-1, c);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// East
		if (here.second != g.size() - 1) {
			if (g[r][c+1] != '#') {
				Pos n(r, c+1);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// South
		if (here.first != g.size() - 1) {
			if (g[r+1][c] != '#') {
				Pos n(r+1, c);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// West
		if (here.second != 0) {
			if (g[r][c-1] != '#') {
				Pos n(r, c-1);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}

		return nbrs;
	}

	int CalcMaxPath(const Pos & from, const Pos & to, const Pos & prev, const Grid g,
	int dist, std::set<Pos> & vertices)
	{
		auto p = from;
		auto pp = prev;
		std::vector<int> distances;

		while(true) {
			const auto ns = getNbrs(p, pp, g);

			const auto rs = getNbrs2(p, pp, g);
			if (rs.size() > 1) {
				vertices.insert(p);
			}

			dist++;
			if (ns.size() == 1) { // only one route we can take
				pp = p;
				p = ns.front();
				
				if (p == to) {
					return dist;
				}
			}
		
			if (ns.size() > 1) {
				for (auto pn : ns) {
					const auto d = CalcMaxPath(pn, to, p, g, dist, vertices);
					distances.push_back(d);
				}
				return *std::max_element(std::begin(distances), std::end(distances));
			}

			if (ns.size() == 0) {
				break;
			}

		}

		return -1;
	}

	int CalcMinPath(const Pos & from, const Pos & to, const Pos & prev, const Grid g,
	int dist)
	{
		bool arrived = (from == to);
		if (arrived) {
			return dist;
		}

		auto p = from;
		auto pp = prev;
		std::vector<int> distances;

		while(true) {
			const auto ns = getNbrs2(p, pp, g);
			dist++;
			if (ns.size() == 1) { // only one route we can take
				pp = p;
				p = ns.front();
				
				if (p == to) {
					return dist;
				}
			}
		
			if (ns.size() > 1) {
				if (p != from) {
					break;
				}

				for (auto pn : ns) {
					const auto d = CalcMinPath(pn, to, p, g, dist);
					distances.push_back(d);
				}
				return *std::min_element(std::begin(distances), std::end(distances));
			}

			if (ns.size() == 0) {
				break;
			}
		}

		return 1e6;
	}

	void buildGraph(std::set<Pos> & vertices, const Grid g,
	std::vector<Pos> & vs, std::vector<std::vector<int>> & adj)
	{
		vs.clear();
		std::copy(vertices.begin(), vertices.end(), std::back_inserter(vs));
		std::set<Pos> null;
		adj = std::vector<std::vector<int> >(vs.size(),
											 std::vector<int>(vs.size()));
		for (size_t i = 0; i < vs.size(); ++i) {
			for (size_t j = 0; j < vs.size(); ++j) {
				if (i == j) {
					continue;
				}
				int temp = CalcMinPath(vs[i], vs[j], {0,0}, g, 0);
				adj[i][j] = (temp == 1e6) ? 0 : temp;
			}
		}

		return;
	}

	int maxPathThroughGraph(const Pos from, const Pos to,
		const std::vector<Pos> vs, const std::vector<std::vector<int>> adj,
		const std::set<Pos> visited, int depth=0)
	{
		std::vector<int> distances;

		auto it = find(vs.begin(), vs.end(), from);
		int idx = it - vs.begin(); 

		std::vector<std::pair<Pos, int>> nbrs;
		// nbrs in the adjaceny graph
		for (size_t i = 0; i < adj[idx].size(); ++i) {
			if (adj[idx][i] != 0) {
				// make sure we haven't visited here before
				if (visited.find(vs[i]) == visited.end()) {
					nbrs.push_back( { vs[i], adj[idx][i] } );
				}
			}
		}

		if (nbrs.size() == 0) {
			return -1e6;
		}

		for (auto [n, nd] : nbrs) {
				if (n == to) {
					return nd;
				}
				auto visitedNew = visited;
				visitedNew.insert(from);
				const auto d = nd + maxPathThroughGraph(n, to, vs, adj, visitedNew, depth+1);
				distances.push_back(d);
		}
		return *std::max_element(std::begin(distances), std::end(distances));

		return -1e6;
	}

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		size_t start, end;
		const auto grid = parseInput(ls, start, end);

		std::set<Pos> vertices;
		vertices.insert({0, start});
		vertices.insert({grid.size() - 1, end});
		const int part1 = CalcMaxPath({0, start}, {grid.size() - 1, end}, {0,0}, grid, 0, vertices);

		std::vector<Pos> vv;
		std::vector<std::vector<int>> adj;
		std::set<Pos> visited;
		buildGraph(vertices, grid, vv, adj);

		const int part2 = maxPathThroughGraph({0, start}, {grid.size() - 1, end}, vv, adj, visited);

		AH::PrintSoln(23, part1, part2);

		return 0;
	}

}
