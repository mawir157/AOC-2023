#pragma once

#include "includes.h"

namespace AH
{
	// I/0
	template<typename T, typename W>
	void PrintSoln(const int day, const T soln1, const W soln2)
	{
		std::cout << "Day "       << day   << std::endl;
		std::cout << "  Part 1: " << soln1 << std::endl;
		std::cout << "  Part 2: " << soln2 << std::endl;

		return;
	}

	// void PrintSoln(const int day, const std::string soln1, const std::string soln2);
	void PrintSolnFinal(const int day, const uint64_t soln1);
	std::vector<std::string> ReadTextFile(const std::string& filename);
	std::vector<std::string> ParseLineGroups(const std::vector<std::string>& ss,
		                                       const char sep=' ');
	std::vector<std::string> Split(const std::string &s, char delim);
	std::vector<std::string> SplitOnString(const std::string &s, 
		                                     const std::string delim);

	std::string trim(const std::string & str);
	uint64_t stoui64(const std::string s);
	// Maths
	uint64_t IntPow(const uint64_t x, const uint64_t p);
	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

}
