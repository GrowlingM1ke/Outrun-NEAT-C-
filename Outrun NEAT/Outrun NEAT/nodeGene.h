#pragma once


static int idGlobal = 0;

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
	nodeGene(TYPE type) { this->type = type; this->id = idGlobal++; }
	TYPE getTYPE() { return type; }
	int getId() { return id; }
};
