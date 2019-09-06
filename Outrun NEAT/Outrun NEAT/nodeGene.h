#pragma once


static int nodeId = 0;

enum TYPE {
	INPUT,
	HIDDEN,
	OUTPUT
};

class nodeGene {

private:
	TYPE type;
	int id;

public:
	nodeGene(TYPE type) { this->type = type; this->id = nodeId++; }
	TYPE getTYPE() { return type; }
	int getId() { return id; }
};
