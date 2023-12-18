#include "AH.h"

namespace Day18
{

	typedef std::pair<int, int> Pos;

	struct Instruction
	{
	   std::string d;
	   int m;

	   Instruction(const std::string d, const int m) : d(d), m(m) {};
	};

	Instruction parseInput(const std::string s, const bool part1=true)
	{
		const auto ss = AH::Split(s, ' ');
		if (part1) {
			const int i = stoi(ss[1]);

			return Instruction(ss[0], i);
		} else {
			const auto col = ss[2].substr(2,6);
			const int i = int(col[5] -'0');
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
			int x = std::stoul(hex, nullptr, 16);
			printf("%s -> %d\n", hex.c_str(), x);
			return Instruction(dir, x);
		}
	}

	Pos digTrench(std::map<Pos, bool> & trench, const Pos p, 
	const Instruction in)
	{
		Pos pp = p;
		if (in.d == "L") {
			for (int i = 0; i < in.m; ++i) {
				--pp.second;
				trench[pp] = true;
			}
		} else if (in.d == "U") {
			for (int i = 0; i < in.m; ++i) {
				--pp.first;
				trench[pp] = true;
			}
		} else if (in.d == "R") {
			for (int i = 0; i < in.m; ++i) {
				++pp.second;
				trench[pp] = true;
			}		
		} else if (in.d == "D") {
			for (int i = 0; i < in.m; ++i) {
				++pp.first;
				trench[pp] = true;
			}
		}

		return pp;
	}

	void floodFill(std::map<Pos, bool> & trench, const Pos p)
	{
		if (trench.count(p) != 0) { // sanity to check to find valid start
			printf("ALREADY IN BOUNDARY KILL\n");
			return;
		}
		
		Pos p0 = p;
		std::queue<Pos> todo;
		trench[p0] = "FILLED";
		todo.push(p0);

		while(todo.size() > 0) {
			auto p0 = todo.front();
			todo.pop();

			std::vector<Pos> nbrs = {
				{p0.first, p0.second-1},
				{p0.first-1, p0.second},
				{p0.first, p0.second+1},
				{p0.first+1, p0.second}
				}; 

			for (auto n : nbrs) {
				if (trench.count(n) == 0) {
					trench[n] = true;
					todo.push(n);
				}				
			}
		}

		return;
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);

		std::map<Pos, bool> trench;
		std::map<Pos, bool> trench2;
		Pos here{0,0};
		Pos here2{0,0};
		for (auto l : ls) {
			const auto ins = parseInput(l);
			here = digTrench(trench, here, ins);
			// const auto ins2 = parseInput(l, false);
			// here2 = digTrench(trench2, here2, ins2);
		}

		// start found with a bit of trial and error
		floodFill(trench, Pos{-1, 1});
		// floodFill(trench2, Pos{-1, 1}); 

		AH::PrintSoln(18, trench.size(), trench2.size());

		return 0;
	}

}
