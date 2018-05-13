#include "../include/system.h"
#include "../include/graphic.h"

/* Coord structure will simplify the implementation for position and bouding box */
typedef struct coord{
	float x;
	float y;
} Coordinates;

/* Each element will have:
* position x & y from the coordinates structure
* 4 coordinates for the bouding box
* a velocity
* a texture
* will be in a simple list
*/
typedef struct element{
	Coordinates x, y;
	Coordinates pminX, pmaxX, pminY, pmaxY;
	int life;
	float speed;
    GLuint textureID;
	struct element *suiv;
} Element, *ListeElements;



int initializeSDL(int level);
void gameLoop();