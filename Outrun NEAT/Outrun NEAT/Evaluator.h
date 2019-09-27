#pragma once
#include "genome.h"
#include <memory>
#include <map>
#include "species.h"

class evaluator
{
private:
	int populationSize;
	float c1 = 0.0f;
	float c2 = 0.0f;
	float c3 = 0.0f;
	float threshold = 0.0f;

	vector<genome> genomes;
	map<genome, float> scoreAdjustedMap;
	map<genome, Species> speciesMap;
	vector<Species> species;
	vector<genome> nextGenerationGenome;

	Species getRandomSpeciesBiasedAdjustedFitness();
	genome getRandomGenomeBiasedAdjustedFitness(Species s);
	
	void removeWeakSpecies();

public:
	void evaluate();
	// virtual void evaluateGenome(genome gen) = 0;
};