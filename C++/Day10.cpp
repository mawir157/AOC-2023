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

		const std::string above = "F7|", left = "FL-", down ="JL|", right = "7-";
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

	std::set<Pos> pipes(const std::vector<std::string> ps)
	{
		int ret = 0;
		std::set<Pos> boundary;

		// First move needs a bit of admin, can go in either direction
		const Pos start = findStart(ps);
		printf("start [%c] = (%ld, %ld)\n", ps[start.first][start.second], start.first, start.second);

		Pos next = start;
		Pos prev, prevprev;

		++ret;
		auto nbrs = moves(next, ps);
		prevprev = start;
		prev = start;
		next = nbrs[0]; // can pick either move here wlog
		boundary.insert(start);

		while (next != start) {
			boundary.insert(next);
			++ret;
			nbrs = moves(next, ps);
			prevprev = prev;
			prev = next;
			next = (nbrs[0] == prevprev) ? nbrs[1] : nbrs[0];
		}

		return boundary;
	}

	bool inBoundary(const size_t r, const size_t s, const std::set<Pos> bd)
	{
		const Pos p = { r, s };

		return bd.find(p) != bd.end();
	}

	int jordanCurveTheorem(const std::vector<std::string> grid, const std::set<Pos> bd)
	{
		int ret = 0;
		for (size_t r = 0; r < grid.size(); ++r) {
			int bd_count_lr = 0;
			char last = 'X';
			for (size_t c = 0; c < grid[r].size(); ++c) {
				if (inBoundary(r, c, bd)) {
					char t = grid[r][c];

					if (t == '-') {
						continue;
					}

					if (t == '|') {
						++bd_count_lr;
					}

					if (t == 'J') {
						if (last == 'F') {
							++bd_count_lr;
						}
						last = 'X';
					}

					if (t == '7') {
						if (last == 'L') {
							++bd_count_lr;
						}
						last = 'X';
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
		const std::vector<std::string> inputLines = AH::ReadTextFile(filename);

		const auto bd = pipes(inputLines);
		int part2 = jordanCurveTheorem(inputLines, bd);

		AH::PrintSoln(10, bd.size() / 2, part2);

		return 0;
	}

}
