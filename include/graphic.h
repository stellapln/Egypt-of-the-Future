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
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000/60;


/********** PROTOTYPES *********/

void resizeViewport(int w, int h);
void drawElement();