#include "AH.h"

namespace Day16
{

	struct BeamState {
		int r,c,d;

		BeamState(const int r, const int c, const int d) :
		r(r), c(c), d(d) {};
	};

	bool operator<(const BeamState & lhs, const BeamState & rhs)
	{
		if (lhs.r != rhs.r) {
			return (lhs.r < rhs.r);
		}

		if (lhs.c != rhs.c) {
			return (lhs.c < rhs.c);
		}

		if (lhs.d != rhs.d) {
			return (lhs.d < rhs.d);
		}

		return false;
	}

	BeamState updateBeam(const BeamState beam, const char loc)
	{
		auto b = beam;
		// beam splitters are handled in the main loop
		if (loc == '/') {
			b.d += (b.d % 2) ? -1 : 1;	
		} else if (loc == '\\') {
			b.d = (3 - b.d);
		}
		
		switch (b.d)
		{
		case 0:
			b.c++; break;
		case 1:
			b.r--; break;
		case 2:
			b.c--; break;
		case 3:
			b.r++; break;
		default:
			break;
		}

		return b;
	}

	void fireTheLaser(const std::vector<std::string> & ms,
	  std::vector<std::vector<int>> & grid, const BeamState beam,
	  std::set<BeamState> & path, int depth=0)
	{
		auto b = beam;

		while(true) {
			// We have been here before so bail out to avoid infinite loop
			if (path.find(b) != path.end()) {
				break;
			}
			
			// if out of bounds bail out
			if ((b.r < 0) || (b.c < 0) ||
			    (b.c >= (int)ms[b.r].size()) || (b.r >= (int)ms.size())) {
				return;
			}

			grid[b.r][b.c] = 1;
			// remember that we've been here
			path.insert(b);

			const auto c = ms[b.r][b.c];
			// beam splitting!
			if ((c == '|') && (b.d % 2 == 0)) {
				auto b_N = b;
				b_N.d = 1;
				b_N.r -= 1;
				fireTheLaser(ms, grid, b_N, path, depth+1);
				auto b_S = b;
				b_S.d = 3;
				b_S.r += 1;
				fireTheLaser(ms, grid, b_S, path, depth+1);
				break;
			} else if ((c == '-') && (b.d % 2 == 1)) {
				auto b_W = b;
				b_W.d = 0;
				b_W.c += 1;
				fireTheLaser(ms, grid, b_W, path, depth+1);
				auto b_E = b;
				b_E.d = 2;
				b_E.c -= 1;
				fireTheLaser(ms, grid, b_E, path, depth+1);
				break;
			} else {
				b = updateBeam(b, c);
			}
		}

		return;
	}

	int sumGrid(const std::vector<std::vector<int>> & grid)
	{
		int sum = 0;
		for (auto r : grid) {
			for (auto c : r) {
				sum += c;
			}
		}

		return sum;
	}

	int part2Ignorance(const std::vector<std::string> & ls)
	{
		int best = 0;
		// top row
		for (int c = 0; c < ls[0].size(); ++c) {
			// top heading south
			std::vector<std::vector<int>> grid(
				ls.size(), std::vector<int> (ls[0].size(), 0));
			std::set<BeamState> path;
			BeamState initial(0, c, 3);

			fireTheLaser(ls, grid, initial, path);
			best = std::max(best, sumGrid(grid));
		}

		for (int c = 0; c < ls[0].size(); ++c) {
			// bottom heading north
			std::vector<std::vector<int>> grid(
				ls.size(), std::vector<int> (ls[0].size(), 0));
			std::set<BeamState> path;
			BeamState initial(ls.size() - 1, c, 1);

			fireTheLaser(ls, grid, initial, path);
			best = std::max(best, sumGrid(grid));
		}

		for (int r = 0; r < ls.size(); ++r) {
			// left heading east
			std::vector<std::vector<int>> grid(
				ls.size(), std::vector<int> (ls[0].size(), 0));
			std::set<BeamState> path;
			BeamState initial(r, 0, 2);

			fireTheLaser(ls, grid, initial, path);
			best = std::max(best, sumGrid(grid));
		}

		for (int r = 0; r < ls[0].size(); ++r) {
			// right heading west
			std::vector<std::vector<int>> grid(
				ls.size(), std::vector<int> (ls[0].size(), 0));
			std::set<BeamState> path;
			BeamState initial(r, ls[0].size() - 1, 0);

			fireTheLaser(ls, grid, initial, path);
			best = std::max(best, sumGrid(grid));
		}

		return best;
	}

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		BeamState initial(0, 0, 0);
		std::set<BeamState> path;
		std::vector<std::vector<int>> grid(
			ls.size(), std::vector<int> (ls[0].size(), 0));

		fireTheLaser(ls, grid, initial, path);
		int part1 = sumGrid(grid);
		int part2 = part2Ignorance(ls);

		AH::PrintSoln(1, part1, part2);

		return 0;
	}

}
