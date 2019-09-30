//#include "SDL.h"
//#undef main
//#include <SDL_ttf.h>
//#include "neuralNetwork.h"
//#include "genome.h"
//#include <map>
//#include <string>
//#include <sstream>
//#include <iomanip>
//#include <iostream>
//#include "genomeRepresentation.h"
//
//using namespace std;
//
//int countNodesByType(genome gen, TYPE type) {
//	int acc = 0;
//	for (nodeGene ng : gen.getNodes()) {
//		if (ng.getTYPE() == type)
//			acc++;
//	}
//	return acc;
//}
//
//void drawNodes(genome gen, SDL_Renderer* renderer, int screenSize){
//	// SDL Related variables
//	int nodeSize = screenSize / 25;
//	int connectionSizeBulb = nodeSize / 2;
//	map<int, vector<int>> m;
//
//	TTF_Font* font = TTF_OpenFont("arial.ttf", 24); //this opens a font style and sets a size
//
//	SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
//
//	SDL_Color Black = { 0, 0, 0 };
//
//	SDL_Color Red = { 255, 0, 0 };
//
//	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "2", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
//
//	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
//
//	SDL_Rect Message_rect; //create a rect
//	Message_rect.x = 0;  //controls the rect's x coordinate 
//	Message_rect.y = 0; // controls the rect's y coordinte
//	Message_rect.w = nodeSize; // controls the width of the rect
//	Message_rect.h = nodeSize; // controls the height of the rect
//
//	// Draw the nodes
//	for (nodeGene gene : gen.getNodes()) {
//		if (gene.getTYPE() == TYPE::INPUTER) {
//			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
//			float x = ((float)(gene.getId()) / ((float)countNodesByType(gen, TYPE::INPUTER))) * screenSize - nodeSize;
//			float y = screenSize - nodeSize / 2;
//			SDL_Rect* rect = new SDL_Rect();
//			rect->x = x - nodeSize / 2;
//			rect->y = y - nodeSize / 2;
//			rect->w = nodeSize;
//			rect->h = nodeSize;
//			SDL_RenderFillRect(renderer, rect);
//
//			m[gene.getId()].push_back((int)x);
//			m[gene.getId()].push_back((int)y);
//		}
//		else if (gene.getTYPE() == TYPE::HIDDEN) {
//			int x = rand() % (screenSize - nodeSize * 2) + nodeSize;
//			int y = rand() % (screenSize - nodeSize * 3) + nodeSize * 1.5f;
//			SDL_Rect* rect = new SDL_Rect();
//			rect->x = x - nodeSize / 2;
//			rect->y = y - nodeSize / 2;
//			rect->w = nodeSize;
//			rect->h = nodeSize;
//			SDL_RenderFillRect(renderer, rect);
//
//			m[gene.getId()].push_back((int)x);
//			m[gene.getId()].push_back((int)y);
//		}
//		else if (gene.getTYPE() == TYPE::OUTPUT) {
//			float x = ((float)(gene.getId()- (float)countNodesByType(gen, TYPE::INPUTER))) / ((float)countNodesByType(gen, TYPE::OUTPUT)) * screenSize;
//			float y = nodeSize / 2;
//			SDL_Rect* rect = new SDL_Rect();
//			rect->x = x - nodeSize / 2;
//			rect->y = y - nodeSize / 2;
//			rect->w = nodeSize;
//			rect->h = nodeSize;
//			SDL_RenderFillRect(renderer, rect);
//
//			m[gene.getId()].push_back((int)x);
//			m[gene.getId()].push_back((int)y);
//		}
//	}
//
//	for (connectionGene connection : gen.getConnections()) {
//		if (!connection.getExpressed()) {
//			continue;
//		}
//		vector<int> inNode = m[connection.getInNode()];
//		vector<int> outNode = m[connection.getOutNode()];
//
//		int x = (outNode.at(0) - inNode.at(0));
//		int y = (outNode.at(1) - inNode.at(1));
//		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//		SDL_RenderDrawLine(renderer, inNode.at(0), inNode.at(1), inNode.at(0) + x, inNode.at(1) + y);
//
//		// Indicate direction
//		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//		SDL_Rect* rect = new SDL_Rect();
//		rect->x = inNode.at(0) + x * 0.8f - connectionSizeBulb / 2;
//		rect->y = inNode.at(1) + y * 0.8f - connectionSizeBulb / 2;
//		rect->w = connectionSizeBulb;
//		rect->h = connectionSizeBulb;
//		SDL_RenderFillRect(renderer, rect);
//
//		// Write down weight
//		std::stringstream stream;
//		stream << std::fixed << std::setprecision(2) << connection.getWeight();
//		string id = stream.str();
//		surfaceMessage = TTF_RenderText_Solid(font, id.c_str(), White);
//		Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
//		Message_rect.x = inNode.at(0) + x * 0.5f - connectionSizeBulb / 2 * 3;
//		Message_rect.y = inNode.at(1) + y * 0.5f - connectionSizeBulb;
//		Message_rect.w = connectionSizeBulb * 3;
//		Message_rect.h = connectionSizeBulb * 2;
//		SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
//	}
//
//	// Render text
//	for (auto const& x : m)
//	{
//		string id = to_string(x.first);
//		surfaceMessage = TTF_RenderText_Solid(font, id.c_str(), Black);
//		Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
//		Message_rect.x = x.second.at(0) - nodeSize / 2;
//		Message_rect.y = x.second.at(1) - nodeSize / 2;
//		Message_rect.w = nodeSize;
//		Message_rect.h = nodeSize;
//		SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
//	}
//
//	SDL_RenderPresent(renderer);
//}
//
//int main() {
//
//	int screenSize = 500;
//
//	// Parent 1
//	genome parent1 = genome();
//	for (int i = 0; i < 3; i++) {
//		nodeGene node = nodeGene(TYPE::INPUTER, i + 1);
//		parent1.addNode(node);
//	}
//
//	parent1.addNode(nodeGene(TYPE::OUTPUT, 4));
//	parent1.addNode(nodeGene(TYPE::OUTPUT, 6));
//	parent1.addNode(nodeGene(TYPE::HIDDEN, 5));
//
//	parent1.addConnection(connectionGene(1, 4, 1.0f, true, 1));
//	parent1.addConnection(connectionGene(2, 4, 1.0f, false, 2));
//	parent1.addConnection(connectionGene(3, 4, 1.0f, true, 3));
//	parent1.addConnection(connectionGene(2, 5, 1.0f, true, 4));
//	parent1.addConnection(connectionGene(5, 4, 1.0f, true, 9));
//	parent1.addConnection(connectionGene(1, 5, 1.0f, true, 8));
//	//parent1.addConnection(connectionGene(1, 6, 1.0f, true, 12));
//
//	srand(time(0));
//	// parent1.addNode();
//	// Make sure a connection between input and input doesn't exist
//	// Make sure there is a check if everything is already taken
//	// parent1.addConnectionMutation();
//	// parent1.mutation();
//	neuralNetwork neuralN = neuralNetwork(parent1);
//	vector<float> inputs;
//	inputs.push_back(-1.0);
//	inputs.push_back(-1.0);
//	inputs.push_back(-1.0);
//	vector<float> outputs = neuralN.calculate(inputs);
//	for (auto output : outputs)
//		cout << output << endl;
//	// Parent 2
//
//	genome parent2 = genome();
//	for (int i = 0; i < 3; i++) {
//		nodeGene node = nodeGene(TYPE::INPUTER, i + 1);
//		parent2.addNode(node);
//	}
//	parent2.addNode(nodeGene(TYPE::OUTPUT, 4));
//	parent2.addNode(nodeGene(TYPE::HIDDEN, 5));
//	parent2.addNode(nodeGene(TYPE::HIDDEN, 6));
//
//	parent2.addConnection(connectionGene(1, 4, 1.0f, true, 1));
//	parent2.addConnection(connectionGene(2, 4, 1.0f, false, 2));
//	parent2.addConnection(connectionGene(3, 4, 1.0f, true, 3));
//	parent2.addConnection(connectionGene(2, 5, 1.0f, true, 4));
//	parent2.addConnection(connectionGene(5, 4, 1.0f, false, 5));
//	parent2.addConnection(connectionGene(5, 6, 1.0f, true, 6));
//	parent2.addConnection(connectionGene(6, 4, 1.0f, true, 7));
//	parent2.addConnection(connectionGene(3, 5, 1.0f, true, 9));
//	parent2.addConnection(connectionGene(1, 6, 1.0f, true, 10));
//
//	// cout << genome::compatibilityDistance(parent1, parent2, 1.0f, 1.0f, 1.0f) << endl;
//
//	// Child
//	genome child = genome::crossover(parent2, parent1);
//	SDL_Init(SDL_INIT_EVERYTHING);
//	SDL_Window* window = SDL_CreateWindow("Genome Representation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenSize, screenSize, SDL_WINDOW_SHOWN);
//	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
//
//
//	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//
//	SDL_RenderClear(renderer);
//
//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
//	
//	// Stuff for writing a message
//	TTF_Init();
//
//	drawNodes(parent1, renderer, screenSize);
//	
//
//	while (true)
//	{
//		// Get the next event
//		SDL_Event event;
//		if (SDL_PollEvent(&event))
//		{
//			if (event.type == SDL_QUIT)
//			{
//				// Break out of the loop on quit
//				/*TTF_Quit();
//				TTF_CloseFont(font);
//				SDL_DestroyTexture(Message);
//				SDL_FreeSurface(surfaceMessage);*/
//				break;
//			}
//		}
//	}
//}