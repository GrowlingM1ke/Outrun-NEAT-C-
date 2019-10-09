#pragma once
#include "genome.h"

class Species
{
public:
	shared_ptr<genome> mascot;
	vector<shared_ptr<genome>> members;
	float totalAdjustedFitness = 0.0f;
	float topFitness = 0.0f;
	int staleness = 0;


	Species(shared_ptr<genome> mascot) {
		this->mascot = mascot;
		members.push_back(mascot);
	}

	void addAdjustedFitness(float adjustedFitness) {
		this->totalAdjustedFitness += adjustedFitness;
	}

	void resetSpecies() {
		this->mascot = bestMember();
		members.clear();
		totalAdjustedFitness = 0.0f;
	}

	void incrementStaleness() { staleness++; }
	void resetStaleness() { staleness = 0; }
	shared_ptr<genome> bestMember() {
		shared_ptr<genome> best = members[0];
		for (auto m : members) {
			if (m->getFitness() > best->getFitness())
				best = m;
		}
		return best;
	}

	bool operator < (const Species& sp) const
	{
		return (totalAdjustedFitness < sp.totalAdjustedFitness);
	}
};

