#pragma once


static int idNodeGlobal = 1;

enum TYPE {
	INPUTER,
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
	nodeGene(int type, int id) { this->type = (TYPE)type; this->id = id; }
	TYPE getTYPE() { return type; }
	int getId() { return id; }
	void setIdNodeGlobal(int value) { idNodeGlobal = value; }
};
