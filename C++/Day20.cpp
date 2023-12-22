#include "AH.h"

namespace Day20
{
	enum Mode   { Source, Flip, Conj, Sink };

	struct GraphNode
	{
		std::string name;
		Mode mode;
		bool onOf;
		std::map<std::string, bool> inputs;

		GraphNode(const std::string name, const Mode mode) : 
		  name(name), mode(mode), onOf(false), inputs({}) {}
		GraphNode() : name("0"), mode(Mode::Source), onOf(false), inputs({}) {}

		bool allHigh() const;
		bool anyHigh() const;
		void PrintNode() const;
		
	};

	bool GraphNode::allHigh() const {
		for (auto [k,v] : inputs) {
			if (!v)
				return false;
		}

		return true;
	}

	bool GraphNode::anyHigh() const {
		for (auto [k,v] : inputs) {
			if (v)
				return true;
		}

		return false;
	}

	struct Graph
	{
		std::vector<std::vector<int>> adj;
		std::vector<GraphNode> gns;

		void PrintState() const;
		GraphNode getNode(const std::string name) const;
	};

	GraphNode Graph::getNode(const std::string name) const
	{
		for (auto n : gns) {
			if (n.name == name) {
				return n;
			}
		}

		std::cout << "Failed to find\n";
		return gns[0];
	}
	
	Graph parseInput(const std::vector<std::string> ls)
	{
		const auto n = ls.size() + 1;
		std::vector<std::vector<int>> adj(n, std::vector<int>(n));
		std::vector<GraphNode> gns(n);

		// populate list of names
		int i = 0;
		for (auto l : ls) {
			const auto ps = AH::SplitOnString(l, " -> ");
			std::string name = ps[0];

			auto mode = Source;
			if (ps[0] != "broadcaster") {
				if (name.at(0) == '&') {
					mode = Conj;
				} else {
					mode = Flip;
				}
				name = name.substr(1);
			}
			const GraphNode gn(name, mode);
			gns[i] = gn;
			++i;
		}

		// sneaky hack because i'm lazy. This is the final node it has no children
		gns[n - 1] = GraphNode("rx", Sink);

		// build adjacency matrix and inputs list
		for (size_t i = 0; i < ls.size(); ++i) {
			const auto ps = AH::SplitOnString(ls[i], " -> ");
			std::string name = ps[0];

			if (ps[0] != "broadcaster") {
				name = name.substr(1);
			}
			// vector of nodes this node mapsto
			const auto to = AH::SplitOnString(ps[1], ", ");

			for (size_t j = 0; j < gns.size(); ++j) {
				const auto gnName = gns[j].name;
				if (std::find(to.begin(), to.end(), gnName) != to.end()) {
					adj[i][j] = 1;
					gns[j].inputs[name] = false;
				}
			}
		}

		Graph g;
		g.adj = adj;
		g.gns = gns;

		return g;
	}

	typedef std::vector<std::pair<std::string, bool>> Pulses;

	Pulses sendPulse(Graph & g, const Pulses & toProcess, int & low, int & high)
	{	
		// Node <name> has just sent a Pulse <signal> 
		std::vector<std::pair<std::string, bool>> newSignals;
		for (auto [name, signal] : toProcess) {
			// find this node in the graph
			for (size_t i = 0; i < g.gns.size(); ++i) {
				auto & node = g.gns[i];
				if (node.name != name) {
					continue;
				}
				// where is signal being sent to?
				const auto adj = g.adj[i];
				for (size_t j = 0; j < adj.size(); ++j) {
					if (adj[j] == 0) {
						continue;
					}

					if (signal) {
						high++;
					} else {
						low++;
					}

					// we are transmitting to this node
					auto & child = g.gns[j];
					auto childName = child.name;
					auto childPulse = false; // false = low
					// figure out what sort of Pulse it has to send
					if (child.mode == Flip) {
						if (!signal) {
							child.onOf = !child.onOf;
							childPulse = child.onOf;
							newSignals.emplace_back(childName, childPulse);
						}
					}
					if (child.mode == Conj) {
						child.inputs[name] = signal;
						if (child.allHigh()) {
							childPulse = false;
						} else {
							childPulse = true;
						}
						newSignals.emplace_back(childName, childPulse);
					}
					
				}				
			}
		}

		return newSignals;
	}

	int Run(const std::string& filename)
	{
		const auto ls = AH::ReadTextFile(filename);
		auto graph = parseInput(ls);
		
		int lows = 0, highs = 0;
		for (int i = 0; i < 1000; ++i) {
			lows++; // send low signal to broadcaster
			Pulses input = { {"broadcaster", false} };
			while(input.size() > 0) {
				input = sendPulse(graph, input, lows, highs);
			}
		}
		const int part1 = lows * highs;

		// hacky nonsense. the only way we can send a low to rx is if all the
		// inputs to cl are set to true so we are going to find cycles on each
		// of these then lcm them. If there was more than one input to rx we 
		// would have to do something more complicated. But there isn't. LUCKY!
		const auto n = graph.getNode("cl");
		std::map<std::string, std::pair<int, int>> cycles;
		for (auto [k,v] : n.inputs) {
			cycles[k] = { 0,0 };
		}

		for (int i = 0; i < 10000; ++i) {
			Pulses input = { {"broadcaster", false} };
			while(input.size() > 0) {
				input = sendPulse(graph, input, lows, highs);

				// find state cl
				const auto n = graph.getNode("cl");
				if (n.anyHigh()) {
					// one of the inputs to cl is high
					for (auto [name,v] : n.inputs) {
						if (v) {
							const auto [x,y] = cycles.at(name);
							if (i == y) break;
							cycles[name] = {y, i};
							break;
						}
					}
				}
			}
		}

		int64_t part2 = 1;
		for (auto [k, v] : cycles) {
			part2 = std::lcm(part2, v.second - v.first);
		}
	
		AH::PrintSoln(20, part1, part2);

		return 0;
	}

}