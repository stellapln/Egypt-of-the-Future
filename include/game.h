#include "../include/graphic.h"


ListeElements allocBg(float x, float y, float speed, GLuint text);
ListeElements allocElements();
void pushElements(ListeElement *lst, int level);
int initializeSDL();
void initializeElements(World *world, int level);
int initializeGame(int level);
void gameLoop();