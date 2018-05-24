#include "../include/graphic.h"

/********** VARIABLES **********/

static const int MOBS_LIFE = 1;
static const int BONUS_LIFE = 1;
static const int KEY_LIFE = 1;
static const int OBSTACLES_LIFE = 1;
static const int SHIP_LIFE = 20;
static const float WORLD_SPEED = 0.08;



/********** PROTOTYPES *********/

/* Contains all essentials settings and the audio mixer */
int initializeSDL();
/* Calls ushElements, initBG, LoadIMGPNG for the BG images and initialize all world element to NULL */
void initializeElements(World *world, int level);
/* Calls those 2 previous functions of initilisation (SDL & Elements) */
int initializeGame(World *world, int level);

/* Malloc and initialize the BG */
ListeElements initBg(float x, float y, float speed, char type);
/* Malloc and initialize all the world elements exept the ship */
ListeElements initElement(float x, float y, int life, float speed, char type);
/* Malloc and initialize the ship */
ListeElements initShip(float x, float y, int life, float speed, char type);

/* Calls initElement */
void addElementToList(ListeElements *lst, float x, float y, int life, float speed, char type);
/* Read the ppm image and malloc the elements (call addElementToList and LoadImgPNG in draw.c) */
void pushElements(World *world, int level);

/* Those 3 next functions manage collision events between ship and elements or arrow and elements */
int collisionArrow(World world, ListeElements);
/* bonus argu added: our ship would have lost a life if not */
int collisionElement(ListeElements bad, ListeElements good);
int collisionShip(World world);

/* Contains the loop game with all drawing/collision/gameplay functions -> assemblage */
void gameLoop(World *world);

/* Free memory functions */
void freeWorld(World *world, int entire);
/* Free memory only to dead element if entire = 0
or free an entire list to free it if entire = 1 
+ delete the texture of each element */
ListeElements cleanList(ListeElements l, int entire);

/* The weapon of the cats sent between 3 differents positions in the screen */
void sendWeapon(World *w);

/* Read best score from the score.txt file */
int readBestScore();
/* If a new best score is made, it's written in the score.txt file to save it */
void writeBestScore(int score);