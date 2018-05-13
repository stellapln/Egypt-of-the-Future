#include "../include/game.h"


int main(int argc, char** argv){
	int level = 1;

	initializeSDL(1);
	gameLoop();

	return EXIT_SUCCESS;
}