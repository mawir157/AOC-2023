#ifdef DAY14

#include "AH.h"

namespace Day14
{

	void shiftNorth(std::vector<std::string> & grid)
	{
		bool moved = true;
		while (moved) {
			moved = false;
			for (int c = 0; c < (int)grid[0].size(); ++c) {
				for (int r = 0; r < (int)grid.size() - 1; ++r) {
					if ((grid[r].at(c) == '.') &&
					    (grid[r + 1].at(c) == 'O')) {
						std::swap(grid[r][c], grid[r+1][c]);
						moved = true;
					}
				}
			}
		}

		return;
	}

	void rotateNegative(std::vector<std::string> & grid)
	{
		const int N = grid.size();// grid is square
		for (int r = 0; r < N / 2; ++r) {
			for (int c = r; c < N - r - 1; ++c) {
				std::swap(grid[r][c],                 grid[N - 1 - c][r]);
				std::swap(grid[N - 1 - c][r],         grid[N - 1 - r][N - 1 - c]);
				std::swap(grid[N - 1 - r][N - 1 - c], grid[c][N - 1 - r]);
			}
		}

		return;
	}

	// north, then west, then south, then east
	void spinCycle(std::vector<std::string> & grid)
	{
		for (int i = 0; i < 4; ++i) {
			shiftNorth(grid);
			rotateNegative(grid);
		}

		return;
	}

	int calculateLoad(const std::vector<std::string> & grid)
	{
		int load = 0;
		for (int r = 0; r < (int)grid.size(); ++r) {
			const int n = std::count(grid[r].begin(), grid[r].end(), 'O');
			load += ((int)grid.size() - r) * n;
		}

		return load;
	}

	int determinePeriod(const std::vector<int> & vs)
	{
		for (int p = 1; p < (int)vs.size(); ++p) {
			bool good = true;
			for (int i = 0; i < p; ++i) {
				if (vs[i] != vs[i+p])
					good = false;
				break;
			}
			if (good) {
				return p;
			}
		}
		return -1;
	}

	int Run(const std::string& filename)
	{
		auto ls = AH::ReadTextFile(filename);

		shiftNorth(ls);
		int part1 = calculateLoad(ls);

		// re-read the grid
		ls = AH::ReadTextFile(filename);
		const int burnIn = 750; // needs to be large enough to fall into a cycle
		int i = 0;
		for (; i < burnIn; ++i) {
			spinCycle(ls);
		}
		std::map<int, int> cycleMap;
		std::vector<int> cycleVector;
		const int target = 1000000000;
		for (; i < burnIn + 100; ++i) { // i know the period is < 100
			const int load = calculateLoad(ls);
			cycleVector.push_back(load);
			cycleMap[i] = load;
			spinCycle(ls);
		}
		const int period = determinePeriod(cycleVector);
		// we go through (target - i) / period full cycles then
		// (target - i) % period final steps to reach target
		const int finalSteps = (target - burnIn) % period;
		int part2 = cycleMap[burnIn + finalSteps];

		AH::PrintSoln(14, part1, part2);

		return 0;
	}

}

#endif
