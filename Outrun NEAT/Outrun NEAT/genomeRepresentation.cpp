#include "SDL.h"
#undef main
#include <SDL_ttf.h>
#include "genome.h"
#include <map>

int countNodesByType(genome *gen, TYPE type) {
	int acc = 0;
	for (nodeGene *ng : gen->getNodes()) {
		if (ng->getTYPE() == type)
			acc++;
	}
	return acc;
}

int main() {
	// Create the genomes
	genome* parent1 = new genome();
	for (int i = 0; i < 3; i++) {
		nodeGene* node = new nodeGene(TYPE::INPUT);
		parent1->addNode(node);
	}

	parent1->addNode(new nodeGene(TYPE::OUTPUT));
	parent1->addNode(new nodeGene(TYPE::HIDDEN));

	// SDL Related variables
	int screenSize = 800;
	int nodeSize = screenSize / 25;
	map<int, int*> m;


	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Genome Representation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenSize, screenSize, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	
	// Stuff for writing a message
	TTF_Init();

	TTF_Font* font = TTF_OpenFont("arial.ttf", 24); //this opens a font style and sets a size

	SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "put your text here", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = 0;  //controls the rect's x coordinate 
	Message_rect.y = 0; // controls the rect's y coordinte
	Message_rect.w = nodeSize; // controls the width of the rect
	Message_rect.h = nodeSize; // controls the height of the rect

	//Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance

	//Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

	SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

	
	// Draw the nodes
	for (nodeGene* gene : parent1->getNodes()) {
		if (gene->getTYPE() == TYPE::INPUT) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			float x = ((float)gene->getId() / ((float)countNodesByType(parent1, TYPE::INPUT) + 1.0f)) * screenSize + nodeSize;
			float y = screenSize - nodeSize / 2;
			SDL_Rect *rect = new SDL_Rect();
			rect->x = x - nodeSize / 2;
			rect->y = y - nodeSize / 2;
			rect->w = nodeSize;
			rect->h = nodeSize;
			SDL_RenderFillRect(renderer, rect);

			int coordinates[2] = { (int)x, (int)y };
			m[gene->getId()] = coordinates;
		}
		else if (gene->getTYPE() == TYPE::HIDDEN) {
			int x = rand() % (screenSize - nodeSize * 2) + nodeSize;
			int y = rand() % (screenSize - nodeSize * 3) + nodeSize * 1.5f;
			SDL_Rect* rect = new SDL_Rect();
			rect->x = x - nodeSize / 2;
			rect->y = y - nodeSize / 2;
			rect->w = nodeSize;
			rect->h = nodeSize;
			SDL_RenderFillRect(renderer, rect);

			int coordinates[2] = { (int)x, (int)y };
			m[gene->getId()] = coordinates;
		}
		else if (gene->getTYPE() == TYPE::OUTPUT) {
			float x = ((float)gene->getId() / ((float)countNodesByType(parent1, TYPE::INPUT) + 1.0f)) * screenSize;
			float y = nodeSize / 2;
			SDL_Rect* rect = new SDL_Rect();
			rect->x = x - nodeSize / 2;
			rect->y = y - nodeSize / 2;
			rect->w = nodeSize;
			rect->h = nodeSize;
			SDL_RenderFillRect(renderer, rect);

			int coordinates[2] = { (int)x, (int)y };
			m[gene->getId()] = coordinates;
		}
	}


	//SDL_RenderDrawLine(renderer, 0, 0, screenSize, screenSize);

	SDL_RenderPresent(renderer);

	while (true)
	{
		// Get the next event
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				// Break out of the loop on quit
				TTF_Quit();
				TTF_CloseFont(font);
				SDL_DestroyTexture(Message);
				SDL_FreeSurface(surfaceMessage);
				break;
			}
		}
	}
}