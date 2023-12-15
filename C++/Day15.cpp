#include "AH.h"

namespace Day15
{

    typedef std::pair<std::string, int> P;
    
    std::tuple<std::string, bool, int> parseInput(const std::string & s)
    {
        auto upto = s.find_first_of("-=");
        bool flag = (s.at(upto) == '=');
        int value = -1;

        if (flag) {
            value = stoi(s.substr(upto + 1));
        }

        return { s.substr(0, upto), flag, value };
    }

    int hash(const std::string & s)
    {
        int hash = 0;

        for (auto c : s) {
            hash += int(c);
            hash *= 17;
            hash %= 256;
        }

        return hash;
    }

    int scoreBoxes(const std::vector<std::list<P>> boxes)
    {
        int score = 0;
        int b = 1;
        for (auto box : boxes) {
            int i = 1;
            for (auto l :box) {
                score += b * i * l.second;
                i++;
            }
            b++;
        }

        return score;
    }

	int Run(const std::string& filename)
	{
		const std::vector<std::string> ls = AH::ReadTextFile(filename);
        const std::vector<std::string> inputs = AH::Split(ls[0], ',');

		int part1 = 0;
        std::vector<std::list<P>> boxes(256);

        for (auto i : inputs) {
            const auto [ label, add, value ] = parseInput(i);
            const auto box_id = hash(label);
            part1 += hash(i);

            if (add) { // add to this box
                const P contents{label, value};
                // scan through the box to find a lens with the same label
                bool replaced = false;
                for (auto & l : boxes[box_id]) {
                    if (label == l.first) {
                        l.second = value;
                        replaced = true;
                    }
                }
                if (!replaced) {
                    boxes[box_id].push_back(contents);
                }
            } else { // remove from this box
                boxes[box_id].remove_if([label](P p){ return p.first == label; });
            }
        }

		AH::PrintSoln(15, part1, scoreBoxes(boxes));

		return 0;
	}

}
