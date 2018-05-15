#include "../include/graphic.h"

static float ELEMENTS_SPEED = 0.3; // Except for the ship
static float BACKGROUND_SPEED = 0.01;
static const int MOBS_LIFE = 1;
static const int BONUS_LIFE = 1;
static const int KEY_LIFE = 1;
static const int OBSTACLES_LIFE = 1;
static const int SHIP_LIFE = 3;

ListeElements initBg(float x, float y, float speed);
ListeElements initElement(float x, float y, int life, float speed);
void addElementToList(ListeElements *lst, float x, float y, int life, float speed);
void pushElements(World *world, int level);
int initializeSDL();
void initializeElements(World *world, int level);
int initializeGame(World *world, int level);
void gameLoop(World *world);
