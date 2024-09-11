#ifdef DAY17

#include "AH.h"

namespace Day17
{
	typedef std::vector<std::vector<int>> Grid;
	typedef std::vector<std::vector<std::vector<int>>> Grid3;

	struct Pos
	{
		int r, c, d;
		Pos(const int r, const int c, const int d) : r(r), c(c), d(d) {};
	};
	

	Grid parseInput(std::vector<std::string> ss)
	{
		std::vector<std::vector<int>> grid;
		for (auto l :  ss) {
			std::vector<int> row;
			row.reserve(ss.size());
			for (auto c : l) {
				row.push_back(int(c-'0'));
			}
			grid.push_back(row);
		}

		return grid;
	}

	int hashPos(const int i, const int j, const int d)
	{
		int ret = i;
		ret *= 1000;
		ret += j;
		ret *= 10;
		ret += d;
		return ret;
	}

	int hashPos(const Pos p)
	{
		return hashPos(p.r, p.c, p.d);
	}

	Pos dehashPos(int i)
	{
		int d = i % 10;
		i /= 10;
		int c = i % 1000;
		i /= 1000;
		int r = i % 1000;
		return Pos(r, c, d);
	}

	std::vector<Pos> nbrsOfPos(const Pos p, const int rows, const int cols,
	const bool ultra=false)
	{
		std::vector<Pos> ret;

		if (ultra) {
			if (p.d % 2 == 1) { // we arrived N-S so leave E-W
				for (int c = 0; c < cols; ++c) {
					if ((std::abs(p.c - c) < 4) || (std::abs(p.c - c) > 10)) {
						continue;
					} else {
						int d = (c < p.c) ? 2 : 0;
						ret.emplace_back(p.r,c,d);
					}
				}
			} else { // we arrived E-W so leave N-S
				for (int r = 0; r < rows; ++r) {
					if ((std::abs(p.r - r) < 4) || (std::abs(p.r - r) > 10)) {
						continue;
					} else {
						int d = (r < p.r) ? 1 : 3;
						ret.emplace_back(r,p.c,d);
					}
				}
			}
		} else {
			int row_lo = p.r, row_hi = p.r + 1, col_lo = p.c, col_hi = p.c + 1;
			if (p.d % 2 == 0) { // west-east
				row_lo = std::max(p.r - 3 , 0);
				row_hi = std::min(p.r + 3, rows - 1) + 1;
			} else {
				col_lo = std::max(p.c - 3 , 0);
				col_hi = std::min(p.c + 3, cols - 1) + 1;
			}

			for (int r = row_lo; r < row_hi; ++r) {
				for (int c = col_lo; c < col_hi; ++c) {
					if ((r == p.r) && (c == p.c)) continue;
					int d = 0;
					if (r > p.r) {
						d = 3;
					} else if (c < p.c) {
						d = 2;
					} else if (r < p.r) {
						d = 1;
					}
					ret.emplace_back(r,c,d);
				}
			}
		}

		return ret;
	}

	Pos findSmallest(const std::set<int> Q, const Grid3 & dist, const int minSeen=0)
	{
		Pos ret(-1, -1, -1);
		int smallest = 1e6 + 1;
		for (auto k : Q) {
			const auto p = dehashPos(k);
			const auto d = dist[p.r][p.c][p.d % 2];
			if (d < smallest) {
				smallest = d;
				ret = p;
				if (smallest == minSeen) {
					break;
				}
			}

		}
		return ret;
	}

	int cost(const Pos start, const Pos end, const Grid & grid)
	{
		int cost = 0;
		int row_lo = std::min(start.r, end.r);
		int row_hi = std::max(start.r, end.r) + 1;
		int col_lo = std::min(start.c, end.c);
		int col_hi = std::max(start.c, end.c) + 1;
		for (int r = row_lo; r < row_hi; ++r) {
			for (int c = col_lo; c < col_hi; ++c) {
				cost += grid[r][c];
			}
		}

		return cost - grid[start.r][start.c];
	}

	int shortestPath1(const Grid & grid, const Pos start, const Pos target,
		const bool ultra=false)
	{
		Grid3 dist =
		  std::vector<std::vector<std::vector<int>>>(grid.size(),
		    std::vector<std::vector<int>>(grid[0].size(),
		      std::vector<int>(2)));
		std::set<int> V;
		const int HUGE = 1e6;

		for (int i = 0; i < (int)grid.size(); ++i) {
			for (int j = 0; j < (int)grid[i].size(); ++j) {
				for (int d = 0; d < 2; ++d) {
					dist[i][j][d] = HUGE;
				}
			}
		}

		dist[start.r][start.c][start.d % 2] = grid[start.r][start.c % 2];
		V.insert(hashPos(start));
		int minSeen = dist[start.r][start.c][start.d];

		while (V.size() > 0) {
			const auto u = findSmallest(V, dist, minSeen);
			minSeen = dist[u.r][u.c][u.d % 2];
			V.erase(hashPos(u));

			const auto nbrs = nbrsOfPos(u, grid.size(), grid[0].size(), ultra);
			for (auto n : nbrs) {
				const auto l = minSeen + cost(u, n, grid);
				if (l < dist[n.r][n.c][n.d % 2]) {
					dist[n.r][n.c][n.d % 2] = l;
					V.insert(hashPos(n));
				}
			}
		}

		int best = HUGE;
		for (int i = 0; i < 2; ++i) {
			best = std::min(best, dist[target.r][target.c][i]);
		}

		return best - grid[start.r][start.c];
	}

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		const auto grid = parseInput(ls);

		const auto part1W = shortestPath1(grid, Pos(0,0,0), Pos(grid.size() - 1,grid[0].size() - 1, 0));
		const auto part1S = shortestPath1(grid, Pos(0,0,3), Pos(grid.size() - 1, grid[0].size() - 1, 0));

		const auto part2W = shortestPath1(grid, Pos(0,0,0), Pos(grid.size() - 1, grid[0].size() - 1, 0), true);
		const auto part2S = shortestPath1(grid, Pos(0,0,3), Pos(grid.size() - 1, grid[0].size() - 1, 0), true);

		AH::PrintSoln(17, std::min(part1W, part1S), std::min(part2W, part2S));

		return 0;
	}

}

#endif
