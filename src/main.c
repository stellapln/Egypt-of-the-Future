#include "../include/game.h"


int main(int argc, char** argv){

	if (argc != 2){
		printf("Veuillez préciser le niveau (./main [123])\n");
		return 0;
	}
	if (!(strcmp(argv[1],"3")) && !(strcmp(argv[1], "1")) && !(strcmp(argv[1],"2"))) {
		printf("ici\n");
		printf("Veuillez choisir un niveau (1, 2 ou 3)\n");
		return 0;
	}

	int level = (int)argv[1];
	World world;

	initializeGame(&world, level);
	gameLoop(&world);

	return EXIT_SUCCESS;
}