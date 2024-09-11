#ifdef DAY22

#include "AH.h"

namespace Day22
{
	struct Block
	{
		int xlo, xhi;
		int ylo, yhi;
		int zlo, zhi;
		bool hasFallen;
		size_t idx;

		Block() : xlo(0), xhi(0), ylo(0), yhi(0), zlo(0), zhi(0), hasFallen(false), idx(0) {}
		Block(const int xlo, const int xhi,
			    const int ylo, const int yhi,
			    const int zlo, const int zhi,
				const int index) :
			xlo(xlo), xhi(xhi), ylo(ylo), yhi(yhi), zlo(zlo), zhi(zhi), hasFallen(false), idx(index) {}

		int CanFallBy(const std::vector<Block> & bs, size_t skip) const;
		void Fall(const int by, const bool part2=false) { 
			zlo -= by;
			zhi -= by;
			if (part2) {
				hasFallen = true;
			}
		}
	};

	int Block::CanFallBy(const std::vector<Block> & bs, size_t skip=1e6) const
	{
		const int base = std::min(zlo, zhi);
		if (base <= 1) {
			return 0;
		}

		int fallBy = base - 1; // the furthest we can fall
		for (size_t i = 0; i < bs.size(); ++i) {
			if ( (i == skip) || (i == idx) ) {
				continue;
			}
			const auto b = bs[i];

			// do the x-y shadows overlap?
			if ( (b.xlo <= xhi) && (xlo <= b.xhi) &&
				   (b.ylo <= yhi) && (ylo <= b.yhi) ) {
				// if so is the top of this block below the bases
				const int top = std::max(b.zlo, b.zhi);
				if (base > top) {
					fallBy = std::min(fallBy, base - top - 1);
				}
			}
		}	
		return fallBy;
	}

	Block parseInput(const std::string s, const int index)
	{
		const auto ll = AH::SplitOnString(s, "~");
		const auto lows = AH::SplitOnString(ll[0], ",");
		const auto highs = AH::SplitOnString(ll[1], ",");

		Block b(stoi(lows[0]), stoi(highs[0]),
			      stoi(lows[1]), stoi(highs[1]),
			      stoi(lows[2]), stoi(highs[2]),
				  index);

		return b;
	}

	int part1(const std::vector<Block> & bs)
	{
		int ret = 0;
		for (size_t index = 0; index < bs.size(); ++index) {
			for (size_t i = 0; i < bs.size(); ++i) {
				if (i == index) {
					continue;
				}
				if (bs[i].CanFallBy(bs, index) != 0) {
					ret++;
					break;
				}
			}
		}

		return bs.size() - ret;
	}

	int part2(const std::vector<Block> & bs)
	{
		int ret = 0;
		for (size_t index = 0; index < bs.size(); ++index) {
			auto bss = bs;
			bool falling = true;
			while(falling) {
				falling = false;
				for (size_t i = 0; i < bss.size(); ++i) {
					if (i == index) {
						continue;
					}
					auto & b = bss[i];

					const int by = b.CanFallBy(bss, index);
					if (by > 0) {
						falling = true;
						b.Fall(by, true);
					}
				}
			}

			for (auto b : bss) {
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
		size_t index = 0;
		for (auto l : ls) {
			bs.push_back(parseInput(l, index));
			++index;
		}

		bool falling = true;
		while (falling) {
			falling = false;
			for (auto & b : bs) {
				const int by = b.CanFallBy(bs);
				if (by > 0) {
					falling = true;
					b.Fall(by);
				}
			}
		}

		AH::PrintSoln(22, part1(bs), part2(bs));

		return 0;
	}

}

#endif
