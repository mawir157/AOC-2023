#include "AH.h"

namespace Day13
{

	std::set<int> colRef(const std::string row)
	{
		std::set<int> idxs;

		int dimn = (int)row.size();
		for (int r = 0; r < (dimn - 1); ++r) {
			bool good = true;
			for (int i = 0; i <= r; ++i) {
				if (((r - i) < 0) || ((r + i + 1) >= dimn)) {
					break;
				}
				if (row.at(r - i) != row.at(r + i + 1)) {
					good = false;
					break;
				}
			}

			if (!good) {
				idxs.insert(r + 1);
			}
		}

		return idxs;
	}

	int rowRef(const std::vector<std::string> rs, const int skip) {
		int dimn = (int)rs.size();
		for (int r = 0; r < dimn - 1; ++r) {
			if (r + 1 == skip) {
				continue;
			}

			bool good = true;

			for (int i = 0; i < dimn; ++i) {
				if (((r - i) < 0) || ((r + i + 1) >= dimn)) {
					break;
				}
				if (rs[r - i] != rs[r + i + 1]) {
					good = false;
					break;
				}
			}

			if (good) {
				return r + 1;
			}
		}

		return -1;
	}

	int findRefLines(const std::string grid, const int skip = -1)
	{
		const auto gd = AH::Split(grid, '|');

		const auto row = rowRef(gd, skip / 100);
		if (row > 0) {
			return 100 * row;
		}

		std::set<int> cols;
		for (int i = 1; i < (int)gd[0].size(); ++i) { cols.insert(i); }
		if (skip > 0) { cols.erase(skip % 100); }

		for (auto r : gd) {
			const auto colRefs = colRef(r);
			for (auto c : colRefs) {
				cols.erase(c);
			}
		}

		if (cols.size() > 0) {
			return *cols.begin();
		}
		return -1;
	}

	int findAfterSmudge(const std::string grid, const int bad)
	{
		for (int i = 0; i < (int)grid.size(); ++i) {
			const auto c = grid.at(i);
			if (grid.at(i) == '|') { continue; }

			auto copy_grid = grid;
			copy_grid[i] = (c == '.') ? '#' : '.';
			const auto t = findRefLines(copy_grid, bad);

			if (t > 0) {
				return t;
			}
		}

		return -1;
	}

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		const auto gs = AH::ParseLineGroups(ls, '|');

		int part1 = 0;
		int part2 = 0;

		for (auto g : gs) {
			const auto s = findRefLines(g);
			part1 += s;
			part2 += findAfterSmudge(g, s);
		}

		AH::PrintSoln(13, part1, part2);

		return 0;
	}

}
