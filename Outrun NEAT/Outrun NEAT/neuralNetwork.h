#pragma once
#include <map>
#include "neuron.h"
#include "genome.h"

class neuralNetwork
{
private:
	// All neurons ID
	map<int, neuron> neurons;
	vector<int> input;
	vector<int> output;

	map<int, neuron> unprocessed;

public:
	neuralNetwork(genome gen);
	vector<float> calculate(vector<float> inputParameters);
};

