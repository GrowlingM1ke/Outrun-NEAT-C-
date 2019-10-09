#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <windows.h>
#include <iterator>


typedef std::vector<std::string> stringvec;

using namespace std;

void read_directory(const std::string& name, stringvec& v)
{
	std::string pattern(name);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			v.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}


int main()
{
	// Get a list of all files within the directory
	stringvec v;
	read_directory(".", v);
	std::copy(v.begin(), v.end(),
		std::ostream_iterator<std::string>(std::cout, "\n"));

	// Iterate through the vector and make sure we're only using genomes
	auto it = v.begin();
	while (it != v.end())
	{
		// remove odd numbers
		if (it->find("genome") == std::string::npos) {
			// erase() invalidates the iterator, use returned iterator
			it = v.erase(it);
		}
		// Notice that iterator is incremented only on the else part (why?)
		else {
			++it;
		}
	}

	// Iterate through all the fitness in the generation
	vector<vector<float>> fitnessBig;
	for (auto genomes : v) {
		ifstream file(genomes);
		string genero;
		vector<float> fitnessSmall;

		while (getline(file, genero)) {
			vector<string> values;
			string genero2;
			stringstream ss(genero);
			while (getline(ss, genero2, ',')) {
				values.push_back(genero2);
			}

			fitnessSmall.push_back(stof(values[1]));
		}

		fitnessBig.push_back(fitnessSmall);
	}

	// Now write to a file the results

	fstream fout;
	string fileName = "Results.csv";

	// opens an existing csv file or creates a new file. 
	fout.open(fileName, ios::out);
	fout << "Generation,Highest Fitness,Average Fitness\n";

	for (int i = 0; i < fitnessBig.size(); i++) {
		float biggestFitness = 0.0f;
		float averageFitness = 0.0f;

		for (float fitness : fitnessBig[i]) {
			if (fitness > biggestFitness)
				biggestFitness = fitness;
			averageFitness += fitness;
		}

		averageFitness = averageFitness / fitnessBig[i].size();

		v[i] = v[i].substr(6);
		v[i] = v[i].substr(0, v[i].size() - 4);
		fout << v[i] << ", " << biggestFitness << ", " << averageFitness << "\n";
	}



	return 0;
}