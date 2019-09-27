#include "neuralNetwork.h"

neuralNetwork::neuralNetwork(genome gen)
{
	for (nodeGene node : gen.getNodes()) {
		neuron neu = neuron();

		if (node.getTYPE() == TYPE::INPUT) {
			neu.increaseInputConnectionSize();
			input.push_back(node.getId());
		}
		else if (node.getTYPE() == TYPE::OUTPUT) {
			output.push_back(node.getId());
		}
		neurons.insert(make_pair(node.getId(), neu));
	}

	for (connectionGene con : gen.getConnections()) {
		if (!con.getExpressed())
			continue;

		neurons[con.getInNode()].addOutputConnection(con.getOutNode(), con.getWeight());
		neurons[con.getOutNode()].increaseInputConnectionSize();
	}
}

vector<float> neuralNetwork::calculate(vector<float> inputParameters)
{
	if (inputParameters.size() != input.size()) {
		throw exception("Number of inputs must match number of input neurons in genome");
	}

	// Iterate through the neurons map and reset them
	for (auto &neu : neurons)
		neu.second.reset();

	unprocessed.clear();
	// Insert the neurons into the unprocessed list but make sure to pass reference in
	for (auto neu : neurons)
		unprocessed[neu.first] = neu.second;
	

	// ready the inputs
	// loop through each input
	for (int i = 0; i < inputParameters.size(); i++) {
		neuron &inputNeuron = neurons[input[i]];
		// input neurons only have one input, so we know they're ready for calculation
		inputNeuron.feedInput(inputParameters[i]);
		inputNeuron.calculate();

		// loop through receivers of this input
		for (int k = 0; k < inputNeuron.getNoOfInputs(); k++) {
			neuron &receiver = neurons[inputNeuron.getOutputIds()[k]];
			// add the input directly to the next neuron, using the correct weight for the connection
			receiver.feedInput(inputNeuron.getOutput() * inputNeuron.getOutputWeights()[k]);
		}
		unprocessed.erase(input[i]);
	}

	int loops = 0;
	while (unprocessed.size() > 0) {
		loops++;
		if (loops > 10000) {
			throw exception("Can't solve network giving up.");
		}
		
		vector<int> toRemove;
		for (auto keyNeuron : unprocessed) {
			// Grab reference to the actual map we're working with
			neuron &neu = neurons[keyNeuron.first];
			// if neuron has all inputs, calculate the neuron
			if (neu.isReady()) {
				neu.calculate();
				for (int i = 0; i < neu.getOutputIds().size(); i++) {
					int receiverID = neu.getOutputIds()[i];
					float receiverValue = neu.getOutput() * neu.getOutputWeights()[i];
					neurons[receiverID].feedInput(receiverValue);
				}
				// Set it up for removal
				toRemove.push_back(keyNeuron.first);
			}
		}
		for (int i : toRemove) {
			unprocessed.erase(i);
		}
		toRemove.clear();
	}

	// copy output from output neurons, and copy it into array
	vector<float> outputs;
	for (int i = 0; i < output.size(); i++) {
		outputs.push_back(neurons[output[i]].getOutput());
	}

	return outputs;
}
