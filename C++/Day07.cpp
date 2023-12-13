#include "AH.h"

namespace Day07
{

	const std::string CARDS  = "AKQJT98765432";
	const std::string CARDSJ = "AKQT98765432J";

	std::pair<std::string, int> parseInput(std::string s)
	{
		const auto ss = AH::Split(s, ' ');
		const int bet = stoi(ss[1]);

		return std::make_pair(ss[0], bet);
	}

	int handRank(const std::string hand)
	{
		std::map<char, int> cardCount;
		for (auto c :  hand) {
			cardCount[c] += 1;
		}

		if (cardCount.size() == 1) { // all cards the same, must be Five of a kind
			return 6;
		}

		if (cardCount.size() == 5) { // all cards different, must be High card
			return 0;
		}

		int maxCard = 0;
		for (auto const& [key, val] : cardCount) {
			maxCard = std::max(maxCard, val);
		}

		if (maxCard == 4) { // most common card seen 4 times, must be Four of a Kind
			return 5;
		}

		if (maxCard == 3) { // either Full house or Three of a kind
			if (cardCount.size() == 2) {
				return 4; // Full House
			} else {
				return 3; // Three of kind
			}
		}

		if (maxCard == 2) { // either Two pair or One pair
			if (cardCount.size() == 3) {
				return 2; // Two pair
			} else {
				return 1; // One Pair
			}
		}

		// should never be hit, only here to stop compiler warning
		return 0;
	}

	// It is always optimal to promote jokers to the same card
	int handRankWithJokers(const std::string hand)
	{
		int best = 0;
		for (auto c : CARDS) {
			if (c == 'J') continue;
			auto s = hand;
			std::replace( s.begin(), s.end(), 'J', c);
			best = std::max(best, handRank(s));
		}

		return best;
	}

	bool handLT(std::string a, std::string b, const bool part2=false)
	{
		int a_score = 0, b_score = 0;
		if (!part2) {
			a_score = handRank(a);
			b_score = handRank(b);			
		} else {
			a_score = handRankWithJokers(a);
			b_score = handRankWithJokers(b);
		}

		if (a_score != b_score) {
			return a_score < b_score;
		}

		for (size_t i = 0; i < 5; ++i) {
			int a_i = 0, b_i = 0;
			if (!part2) {
				a_i = CARDS.find(a.at(i));
				b_i = CARDS.find(b.at(i));			
			} else {
				a_i = CARDSJ.find(a.at(i));
				b_i = CARDSJ.find(b.at(i));	
			}

			if (a_i != b_i) {
				return a_i > b_i;
			}
		}

		// should never be hit, only here to stop compiler warning
		return false;
	}

	int Run(const std::string& filename)
	{
		const std::vector<std::string> inputLines = AH::ReadTextFile(filename);
		std::vector<std::pair<std::string, int>>hands;
		for (auto & l : inputLines) {
			hands.push_back(parseInput(l));
		}

		int part1 = 0, part2 = 0;

		sort( hands.begin( ), hands.end( ), [ ]( const auto& lhs, const auto& rhs )
		{
		   return handLT(lhs.first, rhs.first);
		});

		int idx = 1;
		for (auto [h, i] : hands) {
			part1 += idx * i;
			++idx;
		}

		sort( hands.begin( ), hands.end( ), [ ]( const auto& lhs, const auto& rhs )
		{
		   return handLT(lhs.first, rhs.first, true);
		});

		idx = 1;
		for (auto [h, i] : hands) {
			part2 += idx * i;
			++idx;
		}

		AH::PrintSoln(7, part1, part2);

		return 0;
	}

}
