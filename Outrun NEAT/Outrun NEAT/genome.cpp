#include "genome.h"
#include <list>
#include <algorithm>

using namespace std;


genome::genome(){
	fitness = 0;
	id = globalID++;
}

// Possible mutation for the weights withon a gene
void genome::mutation() {
	for (connectionGene &con : connections) {
		// Perturbing chance is 90% for little changes (max 0.1) else make a new weight
		if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= 0.9f)
			con.setWeight(con.getWeight() + (-0.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.1f - (-0.1f))))));
		else
			con.setWeight(-2.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0f - (-2.0f)))));
	}
}

vector<int> genome::getInnovationsSorted()
{
	vector<int> innovations;
	for (connectionGene con : connections) {
		innovations.push_back(con.getInnovation());
	}

	sort(innovations.begin(), innovations.end());

	return innovations;
}

vector<int> genome::getNodeIdSorted()
{
	vector<int> ids;
	for (nodeGene ng : nodes) {
		ids.push_back(ng.getId());
	}

	sort(ids.begin(), ids.end());

	return ids;
}

float genome::getWeightByIndex(int innovation)
{
	for (connectionGene con : connections) {
		if (con.getInnovation() == innovation)
			return con.getWeight();
	}
}

void genome::addConnectionMutation() {
	// High chance at the beginning that the connection won't be valid
	int attempts = 0;
	while (attempts < 100) {
		// First Select 2 random nodes from our list
		nodeGene node1 = nodes[rand() % nodes.size()];
		// if nodes size is too big make the other be none input
		nodeGene node2 = nodes[0];
		if (nodes.size() > 100)
			node2 = nodes[rand() % (nodes.size() - 3100) + 3100];
		else
			node2 = nodes[rand() % nodes.size()];
		float weight = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0f)));

		// Check if the node connects to itself
		while (node1.getId() == node2.getId()) {
			node2 = nodes[rand() % (nodes.size() - 1)];
		}

		// See if the positions need to be reversed
		bool reversed = false;

		if (node1.getTYPE() == TYPE::HIDDEN && node2.getTYPE() == TYPE::INPUTER)
			reversed = true;
		else if (node1.getTYPE() == TYPE::OUTPUT && node2.getTYPE() == TYPE::HIDDEN)
			reversed = true;
		else if (node1.getTYPE() == TYPE::OUTPUT && node2.getTYPE() == TYPE::INPUTER)
			reversed = true;

		// Check if they are both output or both Input
		if (node1.getTYPE() == TYPE::INPUTER && node2.getTYPE() == TYPE::INPUTER) {
			attempts++;
			continue;
		}
		if (node1.getTYPE() == TYPE::OUTPUT && node2.getTYPE() == TYPE::OUTPUT) {
			attempts++;
			continue;
		}
			

		bool connectionExists = false;
		// Check if a connection exists already
		for (connectionGene con : connections) {
			if (con.getInNode() == node1.getId() && con.getOutNode() == node2.getId()) {
				attempts++;
				connectionExists = true;
				break;
			}
			// Maybe other way around
			else if (con.getInNode() == node2.getId() && con.getOutNode() == node1.getId()) {
				attempts++;
				connectionExists = true;
				break;
			}
		}

		if (connectionExists)
			continue;

		// Create connection
		connectionGene cg = connectionGene(reversed ? node2.getId() : node1.getId(), reversed ? node1.getId() : node2.getId(), weight, true);
		connections.push_back(cg);
		return;
	}
	

}

void genome::addNode() {
	if (nodes.empty())
		return;

	int id = rand() % connections.size();
	connectionGene con = connections[id];
	
	// Disable the connection
	connections[id].setExpressed(false);

	// Create the new node and connections
	nodeGene newNode = nodeGene(TYPE::HIDDEN);
	connectionGene intoNew = connectionGene(con.getInNode(), newNode.getId(), 1.0f, true);
	connectionGene outNew = connectionGene(newNode.getId(), con.getOutNode(), con.getWeight(), true);

	nodes.push_back(newNode);
	connections.push_back(intoNew);
	connections.push_back(outNew);

}

genome genome::crossover(genome parent1, genome parent2)
{
	bool equal = false;
	// Make sure that genome1 has greater fitness
	if (parent1.getFitness() < parent2.getFitness()) {
		genome tempP = parent1;
		parent1 = parent2;
		parent2 = tempP;
	}
	else if (parent1.getFitness() == parent2.getFitness()) {
		equal = true;
	}

	genome child = genome();
	
	// Copy the node genes of the more fit parent into the child
	for (auto node : parent1.getNodes()) {
		child.addNode(node);
	}

	// Iterate through the connections of the more fit parent
	for (auto connection : parent1.getConnections()) {
		// Check if other parent has same innovation
		bool sameInnovation = false;
		for (auto connection2 : parent2.getConnections()) {
			if (connection.getInnovation() == connection2.getInnovation() || !equal) {
				sameInnovation = true;
				if (rand() % 2 == 0 || !connection2.getExpressed())
					child.addConnection(connection.connectionCopy());
				else
					child.addConnection(connection2.connectionCopy());
				break;
			}
		}

		if (!sameInnovation)
			child.addConnection(connection.connectionCopy());
	}

	return child;
}

float genome::compatibilityDistance(genome genome1, genome genome2, float c1, float c2, float c3, bool removeNodes)
{
	int excessGenes = 0;
	int disjointGenes = 0;
	float avgWeightDiff = 0.0f;
	float weightDiff = 0.0f;
	int matchingGenes = 0;

	vector<int> nodeKeys1 = genome1.getNodeIdSorted();
	vector<int> nodeKeys2 = genome2.getNodeIdSorted();

	int highestId1 = nodeKeys1[nodeKeys1.size() - 1];
	int highestId2 = nodeKeys2[nodeKeys2.size() - 1];
	int indices = max(highestId1, highestId2);

	for (int i = 0; i <= indices; i++) {
		if (contains(nodeKeys1, i) && !contains(nodeKeys2, i)) {
			if (highestId2 < i) {
				excessGenes++;
			}
			else {
				disjointGenes++;
			}
		}
		else if (!contains(nodeKeys1, i) && contains(nodeKeys2, i)) {
			if (highestId1 < i) {
				excessGenes++;
			}
			else {
				disjointGenes++;
			}
		}
	}

	// Connections
	vector<int> conKeys1 = genome1.getInnovationsSorted();
	vector<int> conKeys2 = genome2.getInnovationsSorted();
	int highestInnovation1 = conKeys1[conKeys1.size() - 1];
	int highestInnovation2 = conKeys2[conKeys2.size() - 1];
	indices = max(highestInnovation1, highestInnovation2);

	for (int i = 0; i <= indices; i++) {
		if (contains(conKeys1, i)) {
			if (contains(conKeys2, i)) {
				matchingGenes++;
				weightDiff += abs(genome1.getWeightByIndex(i) - genome2.getWeightByIndex(i));
			}
			else if (highestInnovation2 < i) {
				excessGenes++;
			}
			else {
				disjointGenes++;
			}
		}
		else if (contains(conKeys2, i)) {
			if (highestInnovation1 < i) {
				excessGenes++;
			}
			else {
				disjointGenes++;
			}
		}

	}

	// Final formula
	if (matchingGenes == 0)
		avgWeightDiff = 0;
	else
		avgWeightDiff = weightDiff / matchingGenes;



	int n = max(genome1.getNodes().size(), genome2.getNodes().size());

	//if (removeNodes);
	//	n -= 3100;

	if (n < 20) {
		n = 1;
	}

	return (excessGenes * c1) / n + (disjointGenes * c2) / n + avgWeightDiff * c3;

}

bool genome::contains(vector<int> list1, int index)
{
	if ((std::find(list1.begin(), list1.end(), index) != list1.end()))
		return true;

	return false;
}



