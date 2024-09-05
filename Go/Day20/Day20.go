//go:build d20
// +build d20

package Day20

import (
	"strings"

	AH "AoC/adventhelper"
)

type Mode int
type Pair struct {
	fst, snd int
}

const (
	Source Mode = iota
	Flip
	Conj
	Sink
)

type GraphNode struct {
	name   string
	mode   Mode
	onOff  bool
	inputs map[string]bool
}

func (gn GraphNode) allHigh() bool {
	for _, v := range gn.inputs {
		if !v {
			return false
		}
	}
	return true
}

func (gn GraphNode) anyHigh() bool {
	for _, v := range gn.inputs {
		if v {
			return true
		}
	}
	return false
}

type Graph struct {
	adj [][]int
	gns []GraphNode
}

func (g Graph) getNode(name string) GraphNode {
	for _, gn := range g.gns {
		if gn.name == name {
			return gn
		}
	}

	return g.gns[0] // should never be hit
}

func parseInput(ss []string) Graph {
	n := len(ss) + 1
	adj := make([][]int, n)
	for i := range adj {
		adj[i] = make([]int, n)
	}
	gns := make([]GraphNode, n)

	for i, s := range ss {
		ps := strings.Split(s, " -> ")
		name := ps[0]

		mode := Source
		if name != "broadcaster" {
			if AH.RuneAt(name, 0) == '&' {
				mode = Conj
			} else {
				mode = Flip
			}
			name = AH.TrimFirstRune(name)
		}
		mm := make(map[string]bool)
		gn := GraphNode{name: name, mode: mode, onOff: false, inputs: mm}
		gns[i] = gn
	}

	mn := make(map[string]bool)
	gns[n-1] = GraphNode{"rx", Sink, false, mn}

	for i := range ss {
		ps := strings.Split(ss[i], " -> ")
		name := ps[0]
		if ps[0] != "broadcaster" {
			name = AH.TrimFirstRune(name)
		}
		to := strings.Split(ps[1], ", ")
		for j := range gns {
			gnName := gns[j].name
			if AH.ContainsStr(to, gnName) {
				adj[i][j] = 1
				gns[j].inputs[name] = false
			}
		}
	}

	g := Graph{adj, gns}

	return g
}

type Pulse struct {
	name  string
	onOff bool
}

func sendPulse(g Graph, toProcess []Pulse, low *int, high *int) []Pulse {
	newSignals := make([]Pulse, 0)
	for _, s := range toProcess {
		name := s.name
		signal := s.onOff

		for i := range g.gns {
			node := g.gns[i]
			if node.name != name {
				continue
			}

			adj := g.adj[i]
			for j := range adj {
				if adj[j] == 0 {
					continue
				}

				if signal {
					*high++
				} else {
					*low++
				}

				childName := g.gns[j].name
				childPulse := false

				if g.gns[j].mode == Flip {
					if !signal {
						g.gns[j].onOff = !g.gns[j].onOff
						childPulse = g.gns[j].onOff
						newSignals = append(newSignals, Pulse{childName, childPulse})
					}
				} else if g.gns[j].mode == Conj {
					g.gns[j].inputs[name] = signal
					if g.gns[j].allHigh() {
						childPulse = false
					} else {
						childPulse = true
					}
					newSignals = append(newSignals, Pulse{childName, childPulse})
				}
			}
		}
	}

	return newSignals
}

func Run() {
	ls, _ := AH.ReadStrFile("../input/input20.txt")
	graph := parseInput(ls)

	lows, highs := 0, 0
	for i := 0; i < 1000; i++ {
		lows++
		input := []Pulse{{"broadcaster", false}}
		for len(input) > 0 {
			input = sendPulse(graph, input, &lows, &highs)
		}
	}

	part1 := lows * highs

	n := graph.getNode("cl")
	cycles := make(map[string]Pair)
	for k, _ := range n.inputs {
		cycles[k] = Pair{0, 0}
	}

	for i := 0; i < 10000; i++ {
		input := []Pulse{Pulse{"broadcaster", false}}

		for len(input) > 0 {
			input = sendPulse(graph, input, &lows, &highs)

			n := graph.getNode("cl")
			if n.anyHigh() {
				for name, v := range n.inputs {
					if v {
						p := cycles[name]
						if i == p.snd {
							break
						}
						cycles[name] = Pair{p.snd, i}
						break
					}
				}
			}
		}
	}

	part2 := 1
	for _, v := range cycles {
		part2 = AH.LCM(part2, v.snd-v.fst)
	}

	AH.PrintSoln(20, part1, part2)

	return
}
