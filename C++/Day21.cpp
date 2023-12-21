#include "AH.h"

namespace Day21
{
	typedef std::vector<std::vector<int>> Grid;
	typedef std::vector<std::vector<std::vector<int>>> Grid3;

	struct Pos
	{
		int r, c;
		int rr, cc;
		Pos (const int r, const int c) : r(r), c(c), rr(0), cc(0) {};
		Pos (const int r, const int c, const int rr, const int cc) : 
		r(r), c(c), rr(rr), cc(cc) {};

		friend bool operator<(const Pos& lhs, const Pos& rhs)
		{
			if (lhs.r != rhs.r)
				return lhs.r < rhs.r;

			if (lhs.c != rhs.c)
				return lhs.c < rhs.c;

			if (lhs.rr != rhs.rr)
				return lhs.rr < rhs.rr;
			
			return (lhs.cc < rhs.cc);
		}
	};

	Grid parseInput(std::vector<std::string> ss, Pos & p)
	{
		std::vector<std::vector<int>> grid;
		
		int r = 0;
		for (auto l :  ss) {
			std::vector<int> row;
			row.reserve(ss.size());
			int c = 0;
			for (auto ch : l) {
				const int v = (ch == '#') ? 1 : 0;
				row.push_back(v);
				if (ch == 'S') {
					p.r = r; p.c = c;
				}
				++c;
			}
			grid.push_back(row);
			++r;
		}

		return grid;
	}

	std::vector<Pos> nbrs(const Pos p, const int rlim, const int clim)
	{
		std::vector<Pos> ns;
		if (p.r > 0) {
			ns.emplace_back(p.r-1, p.c);
		}
		if (p.c > 0) {
			ns.emplace_back(p.r, p.c-1);
		}
		if (p.r < rlim) {
			ns.emplace_back(p.r+1, p.c);
		}
		if (p.c < clim) {
			ns.emplace_back(p.r, p.c+1);
		}

		return ns;
	}

	std::map<Pos, int> spread(const std::map<Pos, int> cur, const Grid & grid)
	{
		const int rlim = grid.size() - 1;
		const int clim = grid[0].size() - 1;
		std::map<Pos, int> next;
		for (auto [p, v] : cur) {
			const auto ns = nbrs(p, rlim, clim);
			for (auto n :  ns) {
				if (grid[n.r][n.c] == 0) {
					next[n] += 1;
				}
			}
		}

		return next;
	}

	// loop back round
	std::vector<Pos> nbrs2(const Pos p, const int rlim, const int clim)
	{
		std::vector<Pos> ns;
		int drrl = (p.r == 0) ? -1 : 0;
		int drrr = (p.r == (rlim - 1)) ? 1 : 0;
		int dccl = (p.c == 0) ? -1 : 0;
		int dccr = (p.c == (clim - 1)) ? 1 : 0;
		ns.emplace_back((p.r - 1 + rlim) % rlim, p.c, p.rr + drrl, p.cc);
		ns.emplace_back(p.r, (p.c - 1 + clim) % clim, p.rr, p.cc + dccl);
		ns.emplace_back((p.r + 1) % rlim, p.c,        p.rr + drrr, p.cc);
		ns.emplace_back(p.r, (p.c + 1) % clim,        p.rr, p.cc + dccr);

		return ns;
	}

	std::map<Pos, int> spread2(const std::map<Pos, int> cur, const Grid & grid,
	const bool parity, std::map<std::pair<int, int>, bool> & mg, const int magic)
	{
		const int rlim = grid.size();
		const int clim = grid[0].size();
		std::map<std::pair<int, int>, int> mgsum;
		std::map<Pos, int> next;
		for (auto [p, v] : cur) {
			// printf("[%d, %d|%d, %d]\n", p.r, p.c, p.rr, p.cc);
			const auto ns = nbrs2(p, rlim, clim);
			for (auto n :  ns) {
				std::pair<int, int> mgindex = {n.rr, n.cc};
				// this meta grid index has settled into a cycle so don't count it any more
				if (mg.count(mgindex) != 0) { 
					continue;
				}
			// printf("\t[%d, %d|%d, %d]\n", n.r, n.c, n.rr, n.cc);
				if ((grid[n.r][n.c] == 0) && (next.count(n) == 0)) {
					next[n] = 1;
					mgsum[mgindex] += 1;
				}
			}
		}

		for (auto [k,v] : mgsum) {
			// printf("\t--->((%d, %d), %d)\n", k.first, k.second, v);
			if (v == magic) {
				mg[k] = parity;
			}
		}

		return next;
	}

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		Pos start(0,0);
		const auto grid = parseInput(ls, start);
		printf("Old Grid Dims = (%lld, %lld)\n", grid.size(), grid[0].size());
		std::map<Pos, int> cur = { {start, 1} };
		std::map<std::pair<int, int>, bool> metagrid;

		for (int i = 0; i < 64; ++i) {
			cur = spread(cur, grid);
		}

		// const auto grid2 = parseInput2(ls, start);
		// printf("New Grid Dims = (%lld, %lld)\n", grid2.size(), grid2[0].size());
		std::map<Pos, int> cur2 = { {start, 1} };

		bool parity = false;
		// const int magic1 = 42, magic2 = 39;
		const int magic1 = 7257, magic2 = 7226;
		for (int i = 0; i < 500; ++i) {
			cur2 = spread2(cur2, grid, parity, metagrid, magic1);
			int live = 0;
			// int live00 = 0;
			parity = !parity;
			for (auto [k,v] : cur2) {
				live += v;
				// if ((k.rr == 0) && (k.cc == 0)) {
				// 	live00 += 1;
				// }
			}
			if (i % 100 == 0) {
				printf("After %d steps there are %d live cells\n", (i+1), live);
				printf("After %d steps %ld mg cells are in a cycle\n", (i+1), metagrid.size());
			}
			// for (auto [n,v] : cur2) {
			// 	printf("\t[%d, %d|%d, %d]\n", n.r, n.c, n.rr, n.cc);
			// }
		}

		int64_t liveCells = cur2.size();
		printf("livecells = %lld\n", liveCells);
		printf("dead metacells = %lld\n", metagrid.size());
		for (auto [k, v] : metagrid) {
			// printf("\t--->((%d, %d), %s)\n", k.first, k.second, v ? "true" : "false");
			liveCells += v ? magic1 : magic2;
		}


		AH::PrintSoln(21, cur.size(), liveCells);

		return 0;
	}

}
