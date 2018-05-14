/* For apple OS */
#ifdef __APPLE__
#include <openGL/gl.h>
#include <openGL/glu.h>
/* For Linus OS */
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
/* For all */
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/***** VARIABLES & STRUCTURES *****/

static unsigned int WINDOW_WIDTH = 900;
static unsigned int WINDOW_HEIGHT = 600;
static unsigned int PPM_WIDTH;
static unsigned int PPM_HEIGHT;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000/60;

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
	Coordinates pos;
	Coordinates pminX, pmaxX, pminY, pmaxY;
	int life;
	float speed;
    GLuint textureID;
	struct element *next;
} Element, *ListeElements;

typedef struct world{
	ListeElements ship;
	ListeElements backgrounds;
	ListeElements mobs;
	ListeElements obstacles;
	ListeElements arrows;
	ListeElements key;
	ListeElements bonus;
	int level;
} World;

/********** PROTOTYPES *********/

void resizeViewport(int w, int h);
void loadImgPNG(char imgSrc[], ListeElements elements);
void createImgPNG(GLuint *textureID, SDL_Surface* img);
void drawList(ListeElements list);
void drawElement(Element *e);
void moveElement(Element *e);
void moveShip(Element *e, int move);
void moveBackground(Element *e);