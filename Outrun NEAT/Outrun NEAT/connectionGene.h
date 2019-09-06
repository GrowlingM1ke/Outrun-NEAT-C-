#pragma once

using namespace std;

static int innovation = 0;

class connectionGene {
private:

	int inNode;
	int outNode;
	float weight;
	bool expressed;
	int innovation;

public:
	connectionGene(int inNode, int outNode, float weight, bool expressed, int innovation) {
		this->inNode = inNode;
		this->outNode = outNode;
		this->weight = weight;
		this->expressed = expressed;
		this->innovation = innovation;
	}

	int getInNode() { return inNode; }
	int getOutNode() { return outNode; }
	float getWeight() { return weight; }
	bool getExpressed() { return expressed; }
	int getInnovation() { return innovation; }

	void setExpressed(bool expressed) { this->expressed = expressed; }
	connectionGene *connectionCopy() { return new connectionGene(inNode, outNode, weight, expressed, innovation); }
};