#pragma once
#include "genome.h"
#include <memory>
#include <map>
#include "species.h"

class evaluator
{
private:

	int currentGeneration = 1;
	vector<shared_ptr<genome>> genomes;
	map<shared_ptr<genome>, float> scoreAdjustedMap;
	map<shared_ptr<genome>, shared_ptr<Species>> speciesMap;
	vector<shared_ptr<Species>> species;
	vector<shared_ptr<genome>> nextGenerationGenome;

	shared_ptr<Species> getRandomSpeciesBiasedAdjustedFitness();
	genome getRandomGenomeBiasedAdjustedFitness(shared_ptr<Species> s);

public:
	void evaluate1();
	void evaluate2();
	vector<shared_ptr<genome>> getGenomes() { return genomes; }
	void initPopulation(int inputs, int outputs, bool load);
	int getCurrentGeneration() { return currentGeneration; }
	float getTopFitness();
	int getNumberOfSpecies() { return species.size(); }
};