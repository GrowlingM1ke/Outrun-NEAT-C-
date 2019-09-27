#include "neuron.h"

void neuron::addOutputConnection(int outputID, float weight)
{
	outputIds.push_back(outputID);
	outputWeights.push_back(weight);
}

float neuron::calculate()
{
	float acc = 0.0f;
	for (float inc : inputs) {
		acc += inc;
	}
	output = sigmoidActivationFunction(acc);
	return output;
}

void neuron::feedInput(float input)
{
	if (inputs.size() < noOfInputs)
		inputs.push_back(input);
	else
		throw exception("Could not add input as it exceeded the maximum.");
}

void neuron::reset()
{
	inputs.clear();
	output = 0.0f;
}


