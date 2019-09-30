#pragma once
#include "genome.h"
#include <memory>
#include <map>
#include "species.h"

class evaluator
{
private:
	int populationSize = 100;

	vector<shared_ptr<genome>> genomes;
	map<shared_ptr<genome>, float> scoreAdjustedMap;
	map<shared_ptr<genome>, shared_ptr<Species>> speciesMap;
	vector<shared_ptr<Species>> species;
	vector<shared_ptr<genome>> nextGenerationGenome;

	shared_ptr<Species> getRandomSpeciesBiasedAdjustedFitness();
	shared_ptr<genome> getRandomGenomeBiasedAdjustedFitness(shared_ptr<Species> s);
	
	void removeWeakSpecies();

public:
	void evaluate1();
	void evaluate2();
	vector<shared_ptr<genome>> getGenomes() { return genomes; }
	void initPopulation(int inputs, int outputs);
	// virtual void evaluateGenome(genome gen) = 0;
};