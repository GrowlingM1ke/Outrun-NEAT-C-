#include "genome.h"
#include <list>


genome::genome(){
	fitness = 0;
}

void genome::addConnectionMutation() {
	// First Select 2 random nodes from our list
	nodeGene node1 = *nodes[rand() % nodes.size()];
	nodeGene node2 = *nodes[rand() % nodes.size()];
	float weight = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0f)));

	// Check if the node connects to itself
	while (node1.getId() == node2.getId()) {
		node2 = *nodes[rand() % (nodes.size() - 1)];
	}

	// See if the positions need to be reversed
	bool reversed = false;

	if (node1.getTYPE() == TYPE::HIDDEN && node2.getTYPE() == TYPE::INPUT)
		reversed = true;
	else if (node1.getTYPE() == TYPE::OUTPUT && node2.getTYPE() == TYPE::HIDDEN)
		reversed = true;
	else if (node1.getTYPE() == TYPE::OUTPUT && node2.getTYPE() == TYPE::INPUT)
		reversed = true;


	// Check if a connection exists already
	for (connectionGene *con : connections) {
		if (con->getInNode() == node1.getId() && con->getOutNode() == node2.getId()) return;
		// Maybe other way around
		else if (con->getInNode() == node2.getId() && con->getOutNode() == node1.getId())  return;
	}

	// Create connection
	connectionGene* cg = new connectionGene(reversed ? node2.getId() : node1.getId(), reversed ? node1.getId() : node2.getId(), weight, true, innovation++);
	connections.push_back(cg);

}

void genome::addNode() {
	if (nodes.empty())
		return;

	int id = rand() % nodes.size();
	connectionGene con = *connections[id];
	
	// Disable the gene
	connections[id]->setExpressed(true);

	// Create the new node and connections
	nodeGene* newNode = new nodeGene(TYPE::HIDDEN);
	connectionGene *intoNew = new connectionGene(con.getInNode(), newNode->getId(), 1.0f, true, innovation++);
	connectionGene *outNew = new connectionGene(newNode->getId(), con.getOutNode(), con.getWeight(), true, innovation++);

	nodes.push_back(newNode);
	connections.push_back(intoNew);
	connections.push_back(outNew);

}

genome *genome::crossover(genome parent1, genome parent2)
{
	// Make sure that genome1 has greater fitness
	if (parent1.getFitness() < parent2.getFitness()) {
		genome tempP = parent1;
		parent1 = parent2;
		parent2 = tempP;
	}

	genome *child = new genome();
	
	// Copy the node genes of the more fit parent into the child
	for (auto node : parent1.getNodes()) {
		child->addNode(node);
	}

	// Iterate through the connections of the more fit parent
	for (auto connection : parent1.getConnections()) {
		// Check if other parent has same innovation
		bool sameInnovation = false;
		for (auto connection2 : parent2.getConnections()) {
			if (connection->getInnovation() == connection2->getInnovation()) {
				sameInnovation = true;
				if (rand() % 2 == 0 || !connection2->getExpressed())
					child->addConnection(connection->connectionCopy());
				else
					child->addConnection(connection2->connectionCopy());
				break;
			}
		}

		if (!sameInnovation)
			child->addConnection(connection->connectionCopy());
	}

	return child;
}

