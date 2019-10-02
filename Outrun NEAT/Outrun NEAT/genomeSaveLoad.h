#pragma once
#include "genome.h"
#include <memory>

class genomesSaveLoad
{
private:
	static int returnHighest(int first, int second);

public:
	static void saveGenomes(vector<shared_ptr<genome>> genomes, int generation);
	static vector<shared_ptr<genome>> loadGenomes();
};

