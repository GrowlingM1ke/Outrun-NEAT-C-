#include "Evaluator.h"
#include <exception>
#include "genomeSaveLoad.h"
#include <iostream>
#include <algorithm>
// Taken from https://pastebin.com/ZZmSNaHX
#define MUTATION_CHANCE 0.25f
#define ADD_CONNECTION_CHANCE 2.0f
#define ADD_NODE_CHANCE 0.5f
#define CROSSOVER_CHANCE 0.75f
#define POPULATION_SIZE 300
#define C1 1.0f
#define C2 1.0f
#define C3 0.4f
#define THRESHOLD 1.0f
#define STALE_SPECIES 10
#define STARTING_TEMPERATURE 100.0f
#define LOSS_OF_TEMPERATURE 0.85f

bool simAnneal = false;


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




genome evaluator::getRandomGenomeBiasedAdjustedFitness(shared_ptr<Species> s)
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
			return *g;
		}
	}

	throw exception("Couldn't find a genome.");
}

void evaluator::evaluate1()
{
	// Reset everything for the next generation
	for (auto s : species) {
		s->resetSpecies();
	}
	scoreAdjustedMap.clear();
	speciesMap.clear();

	// Insert genomes into species
	for (auto gen : genomes) {
		bool foundSpecies = false;
		// Pass the species by reference so the totaladjustedfitness isnt lost
		for (auto s : species) {
			float distance = gen->compatibilityDistance(*gen, *s->mascot, C1, C2, C3, false);
			if (distance < THRESHOLD) {
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



	// Identify stale species
	for (auto s : species) {
		if (s->mascot->getFitness() == s->staleness && s->mascot->getFitness() != getTopFitness()) {
			s->incrementStaleness();
		}
		else {
			s->staleness = s->mascot->getFitness();
			s->incrementStaleness();
		}
	}

	// Remove empty and stale species
	auto it = species.begin();
	while (it != species.end()) {
		if ((**it).members.size() == 0 || (**it).staleness == STALE_SPECIES)
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

	// Order the species from lowest average fitness
	sort(species.begin(), species.end());
	// In order to not have too many species at a time remove the weak ones
	while (species.size() > 20) {
		species.erase(species.begin());
	}

	// Put best genome from each species into next generation
	// Add mascot to check
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

		// Make have a look at this in literature review speciaition niching
		shared_ptr<Species> s = getRandomSpeciesBiasedAdjustedFitness();
		shared_ptr<Species> s2 = getRandomSpeciesBiasedAdjustedFitness();
		genome child;

		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < CROSSOVER_CHANCE) {
			genome p1 = getRandomGenomeBiasedAdjustedFitness(s);
			genome p2 = getRandomGenomeBiasedAdjustedFitness(s2);
			// if Simulated Annealing type then pick another species
			float prob = exp(-p1.compatibilityDistance(p1, p2, C1, C2, C3, false) / (STARTING_TEMPERATURE * powf(LOSS_OF_TEMPERATURE, currentGeneration)));
			if (!((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < prob) || !simAnneal)
				p2 = getRandomGenomeBiasedAdjustedFitness(s);


			child = (genome::crossover(p1, p2));
		}
		else {
			child = getRandomGenomeBiasedAdjustedFitness(s);
		}


		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < MUTATION_CHANCE)
			child.mutation();
		// Add many links
		float acc = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		while (acc < ADD_CONNECTION_CHANCE) {
			acc += (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
			child.addConnectionMutation();
		}
		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < ADD_NODE_CHANCE)
			child.addNode();

		nextGenerationGenome.push_back(make_shared<genome>(child));
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
		genome firstMember = genome();
		for (int i = 0; i < inputs; i++) {
			nodeGene node = nodeGene(TYPE::INPUTER);
			firstMember.addNode(node);
		}

		for (int i = 0; i < outputs; i++) {
			nodeGene node = nodeGene(TYPE::OUTPUT);
			firstMember.addNode(node);
		}
		while (genomes.size() < POPULATION_SIZE) {
			genome gen = firstMember;
			shared_ptr<genome> firstMembers = make_shared<genome>(gen);

			firstMembers->addConnectionMutation();
			firstMembers->addConnectionMutation();
			if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < 0.5f)
				firstMembers->addNode();
			firstMembers->mutation();
			genomes.push_back(firstMembers);
		}
	}
}

float evaluator::getTopFitness()
{
	float top = 0.0f;
	for (auto s : species) {
		if (s->mascot->getFitness() > top)
			top = s->mascot->getFitness();
	}

	return top;

}
