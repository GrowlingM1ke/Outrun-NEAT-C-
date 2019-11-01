#pragma once
#include <vector>
#include "nodeGene.h"
#include "connectionGene.h"

using namespace std;
static int globalID = 0;

class genome {
private:

	vector<connectionGene> connections;
	vector<nodeGene> nodes;
	static bool contains(vector<int> list1, int index);
	float fitness = 0.0f;
	int id = 0;


public:
	// for the comparator
	bool operator <(const genome& rhs) const
	{
		return id < rhs.id;
	}

	genome();
	genome(int id, float fitness) { this->id = id; this->fitness = fitness; }
	void addConnectionMutation();
	void addNode();
	static genome crossover(genome parent1, genome parent2);

	vector<nodeGene> getNodes() { return nodes; }
	void addNode(nodeGene node) { nodes.push_back(node); }

	vector<connectionGene> getConnections() { return connections; }
	void addConnection(connectionGene connection) { connections.push_back(connection); }
	void mutation();
	vector<int> getInnovationsSorted();
	vector<int> getNodeIdSorted();

	float getWeightByIndex(int innovation);
	static float compatibilityDistance(genome genome1, genome genome2, float c1, float c2, float c3, bool removeNodes);

	float getFitness() { return fitness; }
	void setFitness(float fitness) { this->fitness = fitness; }

	int getId() { return id; }
	void setGlobalId(int value) { globalID = value; }
};