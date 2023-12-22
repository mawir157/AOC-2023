#include "AH.h"

namespace Day22
{
	struct Block
	{
		int xlo, xhi;
		int ylo, yhi;
		int zlo, zhi;
		bool hasFallen;

		Block() : xlo(0), xhi(0), ylo(0), yhi(0), zlo(0), zhi(0), hasFallen(false) {}
		Block(const int xlo, const int xhi,
			    const int ylo, const int yhi,
			    const int zlo, const int zhi) :
			xlo(xlo), xhi(xhi), ylo(ylo), yhi(yhi), zlo(zlo), zhi(zhi), hasFallen(false) {}

		bool Contains(const int x, const int y, const int z) const;
		bool CanFall(const std::vector<Block> & bs) const;
		void Fall(bool part2=false) { zlo--; zhi--; if (part2) { hasFallen = true; }}

		friend bool operator==(const Block& l, const Block& r)
		{
			return ( (l.xlo == r.xlo) && (l.xhi == r.xhi) &&
				       (l.ylo == r.ylo) && (l.yhi == r.yhi) &&
				       (l.zlo == r.zlo) && (l.zhi == r.zhi) );
		}
	};

	bool Block::Contains(const int x, const int y, const int z) const
	{
		const bool inx = ((xlo <= x) && (x <= xhi));
		const bool iny = ((ylo <= y) && (y <= yhi));
		const bool inz = ((zlo <= z) && (z <= zhi));

		return inx && iny && inz;
	}

	bool Block::CanFall(const std::vector<Block> & bs) const
	{
		const int base = std::min(zlo, zhi);
		if (base <= 1) {
			return false;
		}

		for (auto b : bs) {
			if (*this == b) {
				continue;
			}
			for (int x = xlo; x <= xhi; ++x) {
				for (int y = ylo; y <= yhi; ++y) {
					if (b.Contains(x, y, base -1)) {
						return false;
					}
				}
			}
		}

		return true;
	}

	Block parseInput(const std::string s)
	{
		const auto ll = AH::SplitOnString(s, "~");
		const auto lows = AH::SplitOnString(ll[0], ",");
		const auto highs = AH::SplitOnString(ll[1], ",");

		Block b(stoi(lows[0]), stoi(highs[0]),
			      stoi(lows[1]), stoi(highs[1]),
			      stoi(lows[2]), stoi(highs[2]));

		return b;
	}

	std::pair<int,int> getXYBounds(const std::vector<Block> & bs)
	{
		int xmax = 0, ymax = 0;
		for (auto b : bs) {
			xmax = std::max(xmax, b.xhi);
			ymax = std::max(ymax, b.yhi);
		}

		return { xmax, ymax };
	}

	int part1(const std::vector<Block> & bs)
	{
		int ret = 0;
		for (size_t index = 0; index < bs.size(); ++index) {
			auto copy_bs = bs;
			copy_bs.erase(copy_bs.begin() + index);
			// std::cout << "[" << index << "] "<< copy_bs.size() << "\n";
			bool canFall = false;
			for (auto b : copy_bs) {
				// std::cout << "\t" << b.CanFall(copy_bs) << "\n"; 
				canFall |= b.CanFall(copy_bs);
			}
			if (!canFall) {
				ret++;
			} 
		}

		return ret;
	}

	int part2(const std::vector<Block> & bs)
	{
		int ret = 0;
		for (size_t index = 0; index < bs.size(); ++index) {
			auto copy_bs = bs;
			copy_bs.erase(copy_bs.begin() + index);
			bool falling = true;
			while(falling) {
				falling = false;
				for (auto & b : copy_bs) {
					if (b.CanFall(copy_bs)) {
						falling = true;
						b.Fall(true);
					}
				}
			}

			for (auto b : copy_bs) {
				if (b.hasFallen) {
					ret++;
				}
			}
		}

		return ret;
	}

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		std::vector<Block> bs;
		for (auto l : ls) {
			bs.push_back(parseInput(l));
		}

		const auto [xx, yy] = getXYBounds(bs);

		bool falling = true;
		while(falling) {
			falling = false;
			for (auto & b : bs) {
				if (b.CanFall(bs)) {
					falling = true;
					b.Fall();
				}
			}
		}

		AH::PrintSoln(22, part1(bs), part2(bs));

		return 0;
	}

}
