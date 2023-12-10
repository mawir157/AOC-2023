#include "AH.h"

namespace Day10
{

	typedef std::pair<size_t, size_t> Pos;

	std::vector<Pos> moves(const Pos p,
		const std::vector<std::string> ps)
	{
		const auto c = ps[p.first][p.second];
		if (c == 'F') {
			return { {p.first + 1, p.second}, {p.first, p.second + 1} };
		} else if (c == 'L') {
			return { {p.first - 1, p.second}, {p.first, p.second + 1} };
		} else if (c == '7') {
			return { {p.first + 1, p.second}, {p.first, p.second - 1} };
		} else if (c == 'J') {
			return { {p.first - 1, p.second}, {p.first, p.second - 1} };
		} else if (c == '|') {
			return { {p.first - 1, p.second}, {p.first + 1, p.second} };
		} else if (c == '-') {
			return { {p.first, p.second - 1}, {p.first, p.second + 1} };
		} else if (c == '.') {
			return {};
		}

		// at this point c == S, we are at the start
		std::vector<Pos> startNbrs;

		const std::string above = "F7|", left = "FL-", down ="JL|", right = "J7-";
		if (above.find(ps[p.first-1][p.second]) != std::string::npos) { // valid above chars
			startNbrs.emplace_back(p.first-1, p.second);
		}
		if (left.find(ps[p.first][p.second-1]) != std::string::npos) { // valid left chars
			startNbrs.emplace_back(p.first, p.second-1);
		}
		if (down.find(ps[p.first+1][p.second]) != std::string::npos) { // valid down chars
			startNbrs.emplace_back(p.first+1, p.second);
		}
		if (right.find(ps[p.first][p.second+1]) != std::string::npos){ // valid right chars
			startNbrs.emplace_back(p.first, p.second+1);
		}


		return startNbrs;
	}

	Pos findStart(const std::vector<std::string> ps) {
		for (size_t r = 0; r < ps.size(); ++r) {
			for (size_t c = 0; c < ps[r].size(); ++c) {
				if (ps[r][c] == 'S') {
					return { r, c };
					break;
				}
			}
		}
		return { 0, 0 };
	}

	int pipes(const std::vector<std::string> ps,
		std::vector<std::vector<int>> & bound)
	{
		int ret = 0;

		// First move needs a bit of admin, can go in either direction
		const Pos start = findStart(ps);
		Pos cur = start;
		Pos prev;

		bound[cur.first][cur.second] = 1;
		++ret;

		auto nbrs = moves(cur, ps);
		prev = cur;
		cur = nbrs[0]; // can pick either move here wlog

		while (cur != start) {
			bound[cur.first][cur.second] = 1;
			++ret;

			nbrs = moves(cur, ps);
			// prevprev = prev;
			const bool use1 = (nbrs[0] == prev);
			prev = cur;
			cur = (use1) ? nbrs[1] : nbrs[0];
		}

		return ret / 2;
	}

	int jordanCurveTheorem(const std::vector<std::string> grid,
		const std::vector<std::vector<int>> bound)
	{
		int ret = 0;
		for (size_t r = 0; r < grid.size(); ++r) {
			int bd_count_lr = 0;
			char last = 'X';
			for (size_t c = 0; c < grid[r].size(); ++c) {
				if (bound[r][c] != 0) {
					char t = grid[r][c];

					if (
						(t == '|') ||
						((t == 'J') && (last == 'F')) ||
						((t == '7') && (last == 'L'))
						) {
						++bd_count_lr;
					}

					if ((t == 'F') || (t == 'L')) {
						last = t;
					}
				} else {
					if ((bd_count_lr % 2) == 1) {
						++ret;
					}
				}
			}
		}

		return ret;
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);
		std::vector<std::vector<int>> bound(ls.size(), std::vector<int>(ls[0].size(), 0));

		const auto part1 = pipes(ls, bound);
		int part2 = jordanCurveTheorem(ls, bound);

		AH::PrintSoln(10, part1, part2);

		return 0;
	}

}
