/* For apple OS */
#ifdef __APPLE__
#include <openGL/gl.h>
#include <openGL/glu.h>
/* For Linus OS */
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
/* For all */
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/******** VARIABLES & STRUCTURES ********/

static float WINDOW_WIDTH = 900.0;
static float WINDOW_HEIGHT = 600.0;
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
* 2 points (with 2 coordinates) for bounding boxs
* a velocity
* a texture
* will be in a simple list
*/
typedef struct element{
	Coordinates pos;
	Coordinates pmin, pmax;
	int life;
	float speed;
    GLuint textureID;
	struct element *next;
	char type;
} Element, *ListeElements;

typedef struct world{
	//Ship
	ListeElements ship;
	
	//Elts
	ListeElements mobs;
	ListeElements obstacles;
	ListeElements arrows;
	ListeElements key;
	ListeElements bonus;
	ListeElements hearts;
	ListeElements pyramid;

	//Screen
	ListeElements backgrounds;
	ListeElements menu;
	ListeElements gameover;
	ListeElements win;
	ListeElements miss;
	ListeElements end;

	int level;
	float speed;

} World;



/********** PROTOTYPES *********/

/* Will resize de window in needed */
void resizeViewport(int w, int h);

/* Create a texture from an image */
void createImgPNG(GLuint *textureID, SDL_Surface* img);
/* Affects the texture passed in parameters to the elements, load the PNG image and calls the createImgPNG function */
void loadImgPNG(char imgSrc[], ListeElements elements);
/* Load number images by calling the createImgPNG function */
void loadImgNb(char imgSrc[], GLuint texture[], int index);

/* Calls the drawElement function until the list in parameters is null */
void drawList(ListeElements list,float,float);
/* Draw all the elements from the list sent by drawList */
void drawElement(Element *e,float width,float height);
/* Draw the ship when it's moving and colors it if collision */
void drawShipInMove(Element *e, float width, float height, int collision);
/* nb = 1 to reduce the scale -for the number-, else it's the "score" image or the "best" image */
void drawScore(GLuint texture, int posX, int nb);

/* Moves the elements from the ppm execpt the ship with the same speed gave in parameters */
void moveElements(ListeElements l);
/* Moves the BG with its own speed to create a parallax effect */
void moveBackground(Element *e);
/* Decrements or increments the ship y position depends on the move value -1 or 1 */
void moveShip(Element *e, int move);

void moveElementsVertical(World w);

/* SDL mix audio */
int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);

/* DEBUG */
void drawBB(ListeElements l);
void drawLandmark();
