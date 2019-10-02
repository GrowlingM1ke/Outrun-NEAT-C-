#include "genomeSaveLoad.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

int genomesSaveLoad::returnHighest(int first, int second) {
	if (first >= second)
		return first;
	else
		return second;
}

void genomesSaveLoad::saveGenomes(vector<shared_ptr<genome>> genomes, int generation) {
	// file pointer 
	fstream fout;
	string fileName = "genomes\\genome" + to_string(generation) + ".csv";

	// opens an existing csv file or creates a new file. 
	fout.open(fileName, ios::out);
	
	if (fout) {
		// Iterate through the genomes
		for (auto genome : genomes) {
			// First get the id then the fitness
			fout << genome->getId() << ",";
			std::ostringstream ss;
			ss << genome->getFitness();
			std::string s(ss.str());
			cout << s << endl;
			fout << s << ",";
			// Now iterate through the connections
			for (auto connection : genome->getConnections()) {
				// First let the reader know that it is a connection
				fout << "c" << ",";
				fout << connection.getInNode() << ",";
				fout << connection.getOutNode() << ",";
				std::ostringstream ss;
				ss << connection.getWeight();
				std::string s(ss.str());
				fout << s << ",";
				fout << connection.getExpressed() << ",";
				fout << connection.getInnovation() << ",";
			}
			// Now iterate through nodes
			for (auto node : genome->getNodes()) {
				// First let the reader know that it is a connection
				fout << "n" << ",";
				fout << node.getTYPE() << ",";
				fout << node.getId() << ",";
			}
			// Now that the genome is finished put an endline character
			fout << "\n";
		}
	}

	fout.close();
}

vector<shared_ptr<genome>> genomesSaveLoad::loadGenomes()
{
	vector<shared_ptr<genome>> genomes;
	ifstream file("genomes\\genome.csv");
	string genero;
	int highestGenome = 0;
	int highestConnection = 0;
	int highestNode = 0;
	while (getline(file, genero)) {
		vector<string> values;
		string genero2;
		stringstream ss(genero);
		while (getline(ss, genero2, ',')) {
			values.push_back(genero2);
		}
		shared_ptr<genome> gen = make_shared<genome>(stoi(values[0]), stof(values[1]));
		int i = 2;
		while (i < values.size()) {
			highestGenome = returnHighest(highestGenome, stoi(values[0]));
			if (values[i] == "c") {
				connectionGene con = connectionGene(stoi(values[i + 1]), stoi(values[i + 2]), stof(values[i + 3]), stoi(values[i + 4]), stoi(values[i + 5]));
				gen->addConnection(con);
				highestConnection = returnHighest(highestConnection, stoi(values[i + 5]));
				i += 6;
			}
			else if (values[i] == "n") {
				nodeGene node = nodeGene(stoi(values[i + 1]), stoi(values[i + 2]));
				gen->addNode(node);
				highestNode = returnHighest(highestNode, stoi(values[i + 2]));
				i += 3;
			}
		}
		genomes.push_back(gen);
	}

	genome gen2 = genome();
	gen2.setGlobalId(highestGenome);
	connectionGene con2 = connectionGene(1, 4, 1.0f, true);
	con2.setInnovationGlobal(highestConnection);
	nodeGene node2 = nodeGene(TYPE::OUTPUT, 4);
	node2.setIdNodeGlobal(highestNode);
	file.close();

	return genomes;
}
