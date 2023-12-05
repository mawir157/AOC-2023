#include "AH.h"

namespace Day04
{

	void parseLine(const std::string game, std::vector<int> & winning,
		std::vector<int> & scratchcard)
	{
		winning.clear();
		scratchcard.clear();

		const int space = game.find(":");
		std::string parsed = game.substr(space+1);
		const auto s = AH::Split(parsed, '|');

		const auto wins = AH::Split(AH::trim(s[0]), ' ');
		for (auto & win : wins) {
			if (win.size() == 0) { continue; }
			winning.push_back(stoi(AH::trim(win)));
		}
		const auto cards = AH::Split(AH::trim(s[1]), ' ');
		for (auto & card : cards) {
			if (card.size() == 0) { continue; }
			scratchcard.push_back(stoi(AH::trim(card)));
		}

		return;
	}

	int scoreCard(const std::vector<int> winning, const std::vector<int> scratchcard) {
		int score = 0;
		for (auto card : scratchcard) {
			if (std::find(winning.begin(), winning.end(), card) != winning.end()) {
			  score++;
			}
		}

		return score;
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> inputLines = AH::ReadTextFile(filename);
		std::vector<int> winning(0);
		std::vector<int> scratchcard(0);

		std::vector<int> copies(inputLines.size(), 1);
		int part1 = 0, part2 = 0;

		for (size_t i = 0; i < inputLines.size(); ++i) {
			const auto l = inputLines[i];

			parseLine(l, winning, scratchcard);
			int score = scoreCard(winning, scratchcard);
			if (score > 0) {
				for (int j = 0; j < score; ++j) {
					if (i + j + 1 < inputLines.size()) {
						copies[i + j + 1] += copies[i];
					}
				}
				part1 += (1 << (score - 1));
			}
		}
		for (auto c : copies) {
			part2 += c;
		}

		AH::PrintSoln(4, part1, part2);

		return 0;
	}

}
