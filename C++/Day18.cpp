#include "AH.h"

namespace Day18
{

	typedef std::pair<int64_t, int64_t> Pos;

	struct Instruction
	{
	   std::string d;
	   int64_t m;

	   Instruction(const std::string d, const int64_t m) : d(d), m(m) {};
	};

	Instruction parseInput(const std::string s, const bool part1=true)
	{
		const auto ss = AH::Split(s, ' ');
		if (part1) {
			return Instruction(ss[0], stoi(ss[1]));
		} else {
			const auto col = ss[2].substr(2,6);
			const int64_t i = int64_t(col[5] -'0');
			std::string dir = "0";
			switch (i)
			{
			case 0:
				dir = "R";
				break;
			case 1:
				dir = "D";
				break;
			case 2:
				dir = "L";
				break;
			case 3:
				dir = "U";
				break;
			default:
				break;
			}
			const auto hex = "0x000" + col.substr(0, 5);
			int64_t x = std::stoul(hex, nullptr, 16);
			return Instruction(dir, x);
		}
	}

	void digTrench(int64_t & boundaryLength, Pos & p, const Instruction in)
	{
		if (in.d == "L") {
			p.second -= in.m;
		} else if (in.d == "U") {
			p.first -= in.m;
		} else if (in.d == "R") {
			p.second += in.m;
		} else if (in.d == "D") {
			p.first += in.m;
		}
		boundaryLength += in.m;

		return;
	}

	int64_t simplePolygonArea(std::vector<Pos> vertices, const int64_t offset)
	{
		int64_t sum = offset;

		for (size_t i = 0; i < (vertices.size() - 1); ++i) {
			const auto p0 = vertices[i];
			const auto p1 = vertices[i+1];
			sum += (p1.first * p0.second) - (p0.first * p1.second);
		}

		return std::abs(sum) / 2;
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);

		int64_t trenchLength1 = 0;
		int64_t trenchLength2 = 0;
		std::vector<Pos> verts1;
		std::vector<Pos> verts2;
		Pos here{0,0};
		Pos here2{0,0};
		verts1.push_back(here);
		verts2.push_back(here2);
		for (auto l : ls) {
			digTrench(trenchLength1, here, parseInput(l));
			verts1.push_back(here);
			digTrench(trenchLength2, here2, parseInput(l, false));
			verts2.push_back(here2);
		}

		// mysterious additional offset of 1 required...
		const int64_t part1 = 1 + simplePolygonArea(verts1, trenchLength1);
		const int64_t part2 = 1 + simplePolygonArea(verts2, trenchLength2);

		AH::PrintSoln(18, part1, part2);

		return 0;
	}

}
