#pragma once


static int idNodeGlobal = 0;

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
	nodeGene(TYPE type, int id) { this->type = type; this->id = id; }
	nodeGene(TYPE type) { this->type = type; this->id = idNodeGlobal++; }
	TYPE getTYPE() { return type; }
	int getId() { return id; }
};
