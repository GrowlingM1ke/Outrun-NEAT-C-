#include "Evaluator.h"
#include <exception>
#define MUTATION_RATE 0.5f
#define ADD_CONNECTION_RATE 0.5f
#define ADD_NODE_RATE 0.5f

Species evaluator::getRandomSpeciesBiasedAdjustedFitness()
{
	float completeWeight = 0.0f;
	for (Species &s : species) {
		completeWeight += s.totalAdjustedFitness;
	}

	float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (completeWeight)));
	float countWeight = 0.0f;
	for (Species &s : species) {
		countWeight += s.totalAdjustedFitness;
		if (countWeight >= r) {
			return s;
		}
	}

	throw exception("Couldn't find a species.");
}

// Last thing to do before check
void evaluator::removeWeakSpecies() {
	for (auto sp : species) {

	}
}


genome evaluator::getRandomGenomeBiasedAdjustedFitness(Species s)
{
	float completeWeight = 0.0f;
	for (genome g : s.members) {
		completeWeight += g.getFitness();
	}

	float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (completeWeight)));
	float countWeight = 0.0f;
	for (genome g : s.members) {
		countWeight += g.getFitness();
		if (countWeight >= r) {
			return g;
		}
	}

	throw exception("Couldn't find a genome.");
}

void evaluator::evaluate()
{
	// Reset everything for the next generation
	for (Species &s : species) {
		s.reset();
	}
	scoreAdjustedMap.clear();
	speciesMap.clear();
	nextGenerationGenome.clear();

	// Insert genomes into species
	for (genome gen : genomes) {
		bool foundSpecies = false;
		// Pass the species by reference so the totaladjustedfitness isnt lost
		for (Species &s : species) {
			if (gen.compatibilityDistance(gen, s.mascot, c1, c2, c3) < threshold) {
				s.members.push_back(gen);
				speciesMap.insert(make_pair(gen, s));
				foundSpecies = true;
				break;
			}
		}

		if (!foundSpecies) {
			Species newSpecies = Species(gen);
			species.push_back(newSpecies);
			// Push a reference
			Species &speciesReference = species[species.size() - 1];
			speciesMap.insert(make_pair(gen, speciesReference));
		}
	}

	// Remove empty species
	auto it = species.begin();
	while (it != species.end()) {
		if (it->members.size() == 0)
			it = species.erase(it);
		else
			++it;
	}

	// Evaluate genomes and assign fitness
	for (genome &gen : genomes) {
		// WATCH OUT BECAUSE YOU ARE ADDING DATA TO THE WRONG VECTOR
		//float score = evaluateGenome(gen);
		//float adjustedScore = score / speciesMap[gen].members.size();
		// Adjusted score is used because the genome competes with other genomes in the species
		//gen.setFitness(adjustedScore);

		//speciesMap[gen].addAdjustedFitness(adjustedScore);
		//scoreAdjustedMap.insert(make_pair(gen, adjustedScore));
	}

	removeWeakSpecies();

	// Put best genome from each species into next generation
	for (Species s : species) {
		float bestFitness = 0.0f;
		genome bestGenome;
		for (genome g : s.members) {
			if (g.getFitness() > bestFitness) {
				bestGenome = g;
			}
		}

		nextGenerationGenome.push_back(bestGenome);
	}

	// Breed new children for the population
	while (nextGenerationGenome.size() < populationSize) {
		Species s = getRandomSpeciesBiasedAdjustedFitness();

		genome p1 = getRandomGenomeBiasedAdjustedFitness(s);
		genome p2 = getRandomGenomeBiasedAdjustedFitness(s);

		genome child;

		child = genome::crossover(p1, p2);

		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < MUTATION_RATE)
			child.mutation();
		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < ADD_CONNECTION_RATE)
			child.addConnectionMutation();
		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < ADD_NODE_RATE)
			child.addNode();

		nextGenerationGenome.push_back(child);
	}
}
