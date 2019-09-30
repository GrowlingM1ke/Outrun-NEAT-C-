#pragma once
#include "genome.h"

class Species
{
public:
	shared_ptr<genome> mascot;
	vector<shared_ptr<genome>> members;
	float totalAdjustedFitness = 0.0f;


	Species(shared_ptr<genome> mascot) {
		this->mascot = mascot;
		members.push_back(mascot);
	}

	void addAdjustedFitness(float adjustedFitness) {
		this->totalAdjustedFitness += adjustedFitness;
	}

	void reset() {
		this->mascot = members[rand() % members.size()];
		members.clear();
		totalAdjustedFitness = 0.0f;
	}
};

