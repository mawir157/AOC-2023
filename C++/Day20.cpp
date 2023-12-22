#include "AH.h"

namespace Day20
{
	enum Mode   { Source, Flip, Conj, Sink };
	enum Signal { Low, High };

	struct GraphNode
	{
		std::string name;
		Mode mode;
		bool onOf; // if this node is a flip flop what state is it in?
		std::map<std::string, bool> inputs;

		GraphNode(const std::string name, const Mode mode) : 
		  name(name), mode(mode), onOf(false), inputs({}) {}
		GraphNode() : name("0"), mode(Mode::Source), onOf(false), inputs({}) {}

		bool allHigh() const;
	};

	bool GraphNode::allHigh() const {
		for (auto [k,v] : inputs) {
			if (!v)
				return false;
		}

		return true;
	}

	struct Graph
	{
		std::vector<std::vector<int>> adj;
		std::vector<GraphNode> gns;
	};
	
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

			auto mode = Mode::Source;
			if (ps[0] != "broadcaster") {
				if (name.at(0) == '&') {
					mode = Mode::Conj;
				} else {
					mode = Mode::Flip;
				}
				name = name.substr(1);
			}
			const GraphNode gn(name, mode);
			gns[i] = gn;
			++i;
		}

		// sneaky hack because i'm lazy fix later, This is the final node
		// gns[n - 1] = GraphNode("output", Mode::Sink);
		gns[n - 1] = GraphNode("rx", Mode::Sink);

		// build adjacency matrix and inputs list
		for (auto i = 0; i < ls.size(); ++i) {
			const auto ps = AH::SplitOnString(ls[i], " -> ");
			std::string name = ps[0];

			if (ps[0] != "broadcaster") {
				name = name.substr(1);
			}
			// vector of nodes this node mapsto
			const auto to = AH::SplitOnString(ps[1], ", ");

			for (auto j = 0; j < gns.size(); ++j) {
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
			for (auto i = 0; i < g.gns.size(); ++i) {
				auto & node = g.gns[i];
				if (node.name != name) {
					continue;
				}
				// where is signal being sent to?
				const auto adj = g.adj[i];
				for (auto j = 0; j < adj.size(); ++j) {
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
					if (child.mode == Mode::Flip) {
						if (!signal) {
							child.onOf = !child.onOf;
							childPulse = child.onOf;
							newSignals.emplace_back(childName, childPulse);
						}
					}
					if (child.mode == Mode::Conj) {
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
			std::vector<std::pair<std:: string, bool>> input = { {"broadcaster", false} };
			while(input.size() > 0) {
				input = sendPulse(graph, input, lows, highs);
			}
		}
	
		AH::PrintSoln(20, lows * highs, 0);

		return 0;
	}

}
