#include "../include/game.h"


int main(int argc, char** argv){
	int level = 1;
	World *world = (World*)malloc(sizeof(World));

	initializeGame(world, level);
	gameLoop(world);

	return EXIT_SUCCESS;
}