#pragma once
#include <vector>
#include "nodeGene.h"
#include "connectionGene.h"

using namespace std;

class genome {
private:

	vector<connectionGene *> connections;
	vector<nodeGene *> nodes;
	float fitness;


public:
	genome();
	void addConnectionMutation();
	void addNode();
	genome *crossover(genome parent1, genome parent2);

	float getFitness() { return fitness; }
	vector<nodeGene*> getNodes() { return nodes; }
	void addNode(nodeGene* node) { nodes.push_back(node); }

	vector<connectionGene*> getConnections() { return connections; }
	void addConnection(connectionGene* connection) { connections.push_back(connection); }
};