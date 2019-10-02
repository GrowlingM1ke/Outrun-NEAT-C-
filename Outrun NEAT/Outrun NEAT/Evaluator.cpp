#include "Evaluator.h"
#include <exception>
#include "genomeSaveLoad.h"
#define MUTATION_CHANCE 0.5f
#define ADD_CONNECTION_CHANCE 0.05f
#define ADD_NODE_CHANCE 0.03f
#define CROSSOVER_CHANCE 0.75f
#define POPULATION_SIZE 50
#define C1 1.0f
#define C2 1.0f
#define C3 0.4f
#define THRESHOLD 3.0f

shared_ptr<Species> evaluator::getRandomSpeciesBiasedAdjustedFitness()
{
	float completeWeight = 0.0f;
	for (shared_ptr<Species> s : species) {
		completeWeight += s->totalAdjustedFitness;
	}

	float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (completeWeight)));
	float countWeight = 0.0f;
	for (shared_ptr<Species> s : species) {
		countWeight += s->totalAdjustedFitness;
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


shared_ptr<genome> evaluator::getRandomGenomeBiasedAdjustedFitness(shared_ptr<Species> s)
{
	float completeWeight = 0.0f;
	for (auto g : s->members) {
		completeWeight += g->getFitness();
	}

	float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (completeWeight)));
	float countWeight = 0.0f;
	for (auto g : s->members) {
		countWeight += g->getFitness();
		if (countWeight >= r) {
			return g;
		}
	}

	throw exception("Couldn't find a genome.");
}

void evaluator::evaluate1()
{
	// Reset everything for the next generation
	for (auto s : species) {
		s.reset();
	}
	scoreAdjustedMap.clear();
	speciesMap.clear();

	// Insert genomes into species
	for (auto gen : genomes) {
		bool foundSpecies = false;
		// Pass the species by reference so the totaladjustedfitness isnt lost
		for (auto s : species) {
			if (gen->compatibilityDistance(*gen, *s->mascot, C1, C2, C3) < THRESHOLD) {
				s->members.push_back(gen);
				speciesMap.insert(make_pair(gen, s));
				foundSpecies = true;
				break;
			}
		}

		if (!foundSpecies) {
			shared_ptr<Species> newSpecies = make_shared<Species>(gen);
			species.push_back(newSpecies);
			speciesMap.insert(make_pair(gen, newSpecies));
		}
	}

	// Remove empty species
	auto it = species.begin();
	while (it != species.end()) {
		if ((**it).members.size() == 0)
			it = species.erase(it);
		else
			++it;
	}
}

void evaluator::evaluate2() {
	// Evaluate genomes and assign fitness
	for (auto gen : genomes) {
		float adjustedScore = gen->getFitness() / speciesMap[gen]->members.size();
		// Adjusted score is used because the genome competes with other genomes in the species

		speciesMap[gen]->addAdjustedFitness(adjustedScore);
		// scoreAdjustedMap.insert(make_pair(gen, adjustedScore));
	}

	// removeWeakSpecies();

	// Put best genome from each species into next generation
	for (auto s : species) {
		float bestFitness = 0.0f;
		shared_ptr<genome> bestGenome;
		for (auto g : s->members) {
			if (g->getFitness() > bestFitness) {
				bestGenome = g;
			}
		}

		nextGenerationGenome.push_back(bestGenome);
	}

	// Breed new children for the population
	while (nextGenerationGenome.size() < POPULATION_SIZE) {

		shared_ptr<Species> s = getRandomSpeciesBiasedAdjustedFitness();
		shared_ptr<genome> child;

		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < CROSSOVER_CHANCE) {
			shared_ptr<genome> p1 = getRandomGenomeBiasedAdjustedFitness(s);
			shared_ptr<genome> p2 = getRandomGenomeBiasedAdjustedFitness(s);

			child = make_shared<genome>(genome::crossover(*p1, *p2));
		}
		else {
			child = getRandomGenomeBiasedAdjustedFitness(s);
		}


		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < MUTATION_CHANCE)
			child->mutation();
		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < ADD_CONNECTION_CHANCE)
			child->addConnectionMutation();
		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < ADD_NODE_CHANCE)
			child->addNode();

		nextGenerationGenome.push_back(child);
	}

	// Save current generation before moving on
	genomesSaveLoad::saveGenomes(genomes, currentGeneration++);

	genomes.clear();
	for (auto genome : nextGenerationGenome) {
		genomes.push_back(genome);
	}
	nextGenerationGenome.clear();
}

void evaluator::initPopulation(int inputs, int outputs, bool load) {
	if (load) {
		genomes = genomesSaveLoad::loadGenomes();
	}
	else {
		while (genomes.size() < 5) {
			shared_ptr<genome> firstMembers = make_shared<genome>();
			for (int i = 0; i < inputs; i++) {
				nodeGene node = nodeGene(TYPE::INPUTER);
				firstMembers->addNode(node);
			}

			for (int i = 0; i < outputs; i++) {
				nodeGene node = nodeGene(TYPE::OUTPUT);
				firstMembers->addNode(node);
			}

			firstMembers->addConnectionMutation();
			firstMembers->addNode();
			firstMembers->mutation();
			genomes.push_back(firstMembers);
		}
	}
}
