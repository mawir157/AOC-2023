#include "AH.h"

namespace Day21
{
	typedef std::vector<std::vector<int>> Grid;

	struct Pos
	{
		int r, c;
		Pos (const int r, const int c) : r(r), c(c) {};

		friend bool operator<(const Pos& lhs, const Pos& rhs)
		{
			if (lhs.r != rhs.r)
				return lhs.r < rhs.r;
			
			return (lhs.c < rhs.c);
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

	std::set<Pos> spread(const std::set<Pos> cur, const Grid & grid)
	{
		const int rlim = grid.size() - 1;
		const int clim = grid[0].size() - 1;
		std::set<Pos> next;
		for (auto p : cur) {
			const auto ns = nbrs(p, rlim, clim);
			for (auto n :  ns) {
				if (grid[n.r][n.c] == 0) {
					next.insert(n);
				}
			}
		}

		return next;
	}

	// Eventually the grid flips between two stable state
	// magic[0]/[1] are the number of live cells in the whole grid in these states
	// magic[2]/[3] are the number of live cells in the CORNERS we will use this 
	// later.
	std::vector<int> getMagicNumbers(const Grid & grid, std::set<Pos> & cur)
	{
		// std::map<Pos, int> cur = { {start, 1} };
		// do enough iterations to get to a stable state 
		// we've lready done 64 so another 66 is enough (130=64+66 and even)
		for (int i = 0; i < 66; ++i) {
			cur = spread(cur, grid);
		}

		std::vector<int> stable{0, 0, 0, 0};
		for (int r = 0; r <  (int)grid.size(); ++r) {
			for (int c = 0; c < (int)grid[r].size(); ++c) {
				Pos p(r, c);
				if (cur.count(p) != 0) {
					const int absV = std::abs(r-65) + std::abs(c-65);
					if ((absV % 2) == 0) {
						stable[0]++;
						if (absV > 65) {
							stable[2]++;
						}
					}			
				}
			}
		}

		cur = spread(cur, grid);
		for (int r = 0; r < (int)grid.size(); ++r) {
			for (int c = 0; c < (int)grid[r].size(); ++c) {
				Pos p(r, c);
				if (cur.count(p) != 0) {
					const int absV = std::abs(r-65) + std::abs(c-65);
					if ((absV % 2) == 1) {
						stable[1]++;
						if (absV > 65) {
							stable[3]++;
						}
					}				
				}
			}
		}

		return stable;
	}

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		Pos start(0,0);
		const auto grid = parseInput(ls, start);
		std::set<Pos> cur = { start };

		for (int i = 0; i < 64; ++i) {
			cur = spread(cur, grid);
		}
		const int part1 = cur.size();

		const auto magic = getMagicNumbers(grid, cur);
		const auto bigN = 26501365; // = INTEGER * grid dimn + 1/2 grid dimn
		// How many full grids have walked? This is exactly an integer.
		const int64_t n = ((bigN - (grid.size() / 2)) / grid.size());
		// draw a picture we have (n+1)^2 Odd parity stable states + n^2 Even parity
		// states then need to remove n+1 Odd Corners and add n Even Corners
		// The grid has very specific properties that means this works - the row and
		// column containing the start are empty and there is an empty region around
		// the corner diagonals.
		// THIS WILL NOT WORK FOR A GENERIC GRID OR A NON INTEGER MULTIPLE FOR
		// NUMBER OF STEPS
		const int64_t part2 = ((n+1)*(n+1)*magic[1]) + (n*n*magic[0]) - ((n+1)*magic[3]) + (n*magic[2]);

		AH::PrintSoln(21, part1, part2);

		return 0;
	}

}
