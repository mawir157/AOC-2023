#include "AH.h"

namespace Day23
{
	typedef std::vector<std::vector<char>> Grid;
	typedef std::pair<size_t, size_t> Pos;

	std::map<int64_t, int> g_memo;

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
		// printf("Finding nbrs of (%lld, %lld):\n", here.first, here.second);
		std::vector<Pos> nbrs;
		const auto r = here.first;
		const auto c = here.second;
		// North
		if (here.first != 0) {
			// printf("Looking North: %c\n", g[r-1][c]);
			if ((g[r-1][c] == '.') || (g[r-1][c] == '^')) {
				Pos n(r-1, c);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// East
		if (here.second != g.size() - 1) {
			// printf("Looking East: %c\n", g[r][c+1]);
			if ((g[r][c+1] == '.') || (g[r][c+1] == '>')) {
				Pos n(r, c+1);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// South
		if (here.first != g.size() - 1) {
			// printf("Looking South: %c\n", g[r+1][c]);
			if ((g[r+1][c] == '.') || (g[r+1][c] == 'v')) {
				Pos n(r+1, c);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// West
		if (here.second != 0) {
			// printf("Looking West: %c\n", g[1][c-1]);
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
		// printf("Finding nbrs(2) of (%lld, %lld):\n", here.first, here.second);
		std::vector<Pos> nbrs;
		const auto r = here.first;
		const auto c = here.second;
		// North
		if (here.first != 0) {
			// printf("Looking North: %c\n", g[r-1][c]);
			if (g[r-1][c] != '#') {
				Pos n(r-1, c);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// East
		if (here.second != g.size() - 1) {
			// printf("Looking East: %c\n", g[r][c+1]);
			if (g[r][c+1] != '#') {
				Pos n(r, c+1);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// South
		if (here.first != g.size() - 1) {
			// printf("Looking South: %c\n", g[r+1][c]);
			if (g[r+1][c] != '#') {
				Pos n(r+1, c);
				if (n != prev) {
					nbrs.push_back(n);
				}
			}
		}
		// West
		if (here.second != 0) {
			// printf("Looking West: %c\n", g[1][c-1]);
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
			// printf("(%lld, %lld) [%c]\n", p.first, p.second, g[p.first][p.second]);

			const auto ns = getNbrs(p, pp, g);
			dist++;
			if (ns.size() == 1) { // only one route we can take
				pp = p;
				p = ns.front();
				
				if (p == to) {
					printf("Reached the end with distance %d\n", dist);
					return dist;
				}
			}
		
			if (ns.size() > 1) {
				// printf("Reached a fork in the road at (%lld, %lld)\n", p.first, p.second);
				vertices.insert(p);
				for (auto pn : ns) {
					// printf("\t(%lld, %lld) [%c]\n", r, c, g[r][c]);
					const auto d = CalcMaxPath(pn, to, p, g, dist, vertices);
					distances.push_back(d);
				}
				return *std::max_element(std::begin(distances), std::end(distances));
			}

			if (ns.size() == 0) {
				// printf("Reached a dead end at (%lld, %lld)\n", p.first, p.second);
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
		bool bailOut = false;
		std::vector<int> distances;

		while(true) {
			printf("(%lld, %lld) [%c]\n", p.first, p.second, g[p.first][p.second]);

			const auto ns = getNbrs2(p, pp, g);
			dist++;
			if (ns.size() == 1) { // only one route we can take
				pp = p;
				p = ns.front();
				
				if (p == to) {
					printf("Reached the target with distance %d\n", dist);
					return dist;
				}
			}
		
			if (ns.size() > 1) {
				printf("Reached a fork in the road at (%lld, %lld)\n", p.first, p.second);
				if (p != to) {
					break;
				}

				for (auto pn : ns) {
					// printf("\t(%lld, %lld) [%c]\n", r, c, g[r][c]);
					const auto d = CalcMinPath(pn, to, p, g, dist);
					distances.push_back(d);
				}
				return *std::min_element(std::begin(distances), std::end(distances));
			}

			if (ns.size() == 0) {
				// printf("Reached a dead end at (%lld, %lld)\n", p.first, p.second);
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
				printf("%d, %d\n", i, j);
				printf("Distance from (%d, %d) to (%d,%d)\n", vs[i].first, vs[i].second, vs[j].first, vs[j].second);
				adj[i][j] = CalcMinPath(vs[i], vs[j], {0,0}, g, 0);
				// adj[j][i] = adj[i][j];
				printf("----------------------------------------------\n");
			}
		}

		return;
	}

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		size_t start, end;
		const auto grid = parseInput(ls, start, end);

		printf("%lld, %lld\n", grid.size(), grid[0].size());
		printf("%lld, %lld\n", start, end);

		const auto nbrs = getNbrs({0, start}, {0, 0}, grid);
		for (auto [r, c] :  nbrs) {
			printf("[%lld, %lld]\n", r, c);
		}

		std::set<Pos> vertices;
		vertices.insert({0, start});
		vertices.insert({grid.size() - 1, end});
		const int part1 = CalcMaxPath({0, start}, {grid.size() - 1, end}, {0,0}, grid, 0, vertices);

		printf("Vertices:\n");
		for (auto [r,c] : vertices) {
			printf("\t[%d, %d]\n", r, c);
		}
		std::vector<Pos> vv;
		std::vector<std::vector<int>> adj;
		buildGraph(vertices, grid, vv, adj);
		printf("Adjacency:\n");
		for (auto r : adj) {
			for (auto v : r) {
				printf("%d\t", v);
			}
			printf("\n");
		}
		printf("\n");

		// std::set<Pos> hist;
		const int part2 = 0;//CalcPath2({0, start}, {grid.size() - 1, end}, {0,0}, grid, 0, hist);

		AH::PrintSoln(23, part1, part2);

		return 0;
	}

}
