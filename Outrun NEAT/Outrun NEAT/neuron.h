#pragma once
#include <vector>
#include <math.h>

using namespace std;

class neuron
{
private:
	float output;
	vector<float> inputs;
	int noOfInputs;
	vector<int> outputIds;
	vector<float> outputWeights;

public:
	neuron() { output = 0.0f; noOfInputs = 0; }

	void addOutputConnection(int outputID, float weight);
	void increaseInputConnectionSize() { noOfInputs++; }
	float calculate();
	bool isReady() {
		if (noOfInputs == inputs.size())
			return true;
		else
			return false;
	}
	void feedInput(float input);
	float getOutput() { return output; }
	int getNoOfInputs() { return noOfInputs; }
	vector<int> getOutputIds() { return outputIds; }
	vector<float> getOutputWeights() { return outputWeights; }
	void reset();
	// To ensure the values stay small it takes a float and returns a value between -1 and 1
	float sigmoidActivationFunction(float input) {
		return ((2.0f / (1.0f + expf(-4.9f * input))) - 1);
	}
};

