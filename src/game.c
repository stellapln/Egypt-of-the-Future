#include "../include/game.h"


ListeElements allocBg(float x, float y, float speed, GLuint text){
	ListeElements tmp = (Element*)malloc(sizeof(Element));

	tmp->pos.x = x;
	tmp->pos.y = y;
	tmp->speed = speed;
	tmp->textureID = text;
	tmp->next = NULL;

	return tmp;
}

ListeElements allocElements(){

}

void pushElements(World *world, int level){
	char line[100];
    FILE *file = NULL;
	if(level == 1)
		file = fopen("./img/ppm/new_ppm.ppm", "r"); // level 1
    else 
	   file = fopen("./img/ppm/new_ppm.ppm", "r");
	/* Read line per line the file */
	fgets(line, sizeof(line), file); /* Line P3 */
	fgets(line, sizeof(line), file); /* Line GIMP... */
	fgets(line, sizeof(line), file); /* Line Width & Height */
	sscanf(line," %d %d", &(PPM_WIDTH), &(PPM_HEIGHT));
	fgets(line, sizeof(line), file); /* 255 line */

	int x = 0;
	int y = PPM_HEIGHT;
	int r, g, b;
	char imgSrc[100];

	while(fgets(line, sizeof(line), file) != NULL){
		if(x == PPM_WIDTH){
			x = 0;
			y--;
		}
		fscanf(line," %d\n %d\n %d\n", &r, &g, &b);
		if(r == 255 && g == 0 && b == 0){
			// mobs
		}
		else if(r == 255 && g == 0 && b == 255){
			// ship
		}
		else if(r == 0 && g == 0 && b == 0){
			// obstacles
		}
		else if(r == 0 && g == 255 && b == 0){
			// bonus
		}
		else if(r == 0 && g == 0 && b == 255){
			// key
		}
		if(r == 255){
			if(b == 0){
				//mobs
			}
			else if(g == 0){
				//ship
			}
		}
		else {
			if(g == 255){
				//bonus
			}
			else if (b == 255){
				//key
			}
			//obs
		}
		x++;
	}

}


int initializeSDL(){

	 if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Window opening and OpenGL context creation */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    /* Name of the window */
    SDL_WM_SetCaption("Egypt of the future", NULL);

    resizeViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

    return EXIT_SUCCESS;

}



void initializeElements(World *world, int level){

	GLuint bg1, bg2;
	//createImgPNG(&bg1, )
	float bgx1 = 0;
	float bgy1 = 0;
	float bgx2 = 60;
	float bgy2 = 0;
	float bgSpeed = 0.1;
	Element *Bg1 = allocBg(bgx1,bgy1,bgSpeed,bg1); // 2 bg
	Element *Bg2 = allocBg(bgx2,bgy2,bgSpeed,bg2); // 2 bg
    Element *Ship;
    pushElements(&world, level);

}


int initializeGame(World world, int level){

    /* SDL initialization */
    initializeSDL();


    /* ELEMENTS initialization */
    initializeElements(&world, level);

    /* */

    return EXIT_SUCCESS;

}

void gameLoop(){
    int loop = 1;

    glClearColor(0, 0, 0, 1.0);

	glEnable(GL_BLEND);

    Uint32 startTime = SDL_GetTicks();


    /*********** LOOP **********/

    while(loop) {


        glClear(GL_COLOR_BUFFER_BIT);

	    glLoadIdentity();


        /* Draw functions*/



	    SDL_GL_SwapBuffers();

        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            switch(e.type) {

                case SDL_QUIT:
                    loop = 0;
                    break;

                 case SDL_VIDEORESIZE:
                    if(e.resize.w > WINDOW_WIDTH || WINDOW_HEIGHT < e.resize.h){
                        resizeViewport(WINDOW_WIDTH,WINDOW_HEIGHT);
                        glClear(GL_COLOR_BUFFER_BIT);
                        SDL_GL_SwapBuffers();
                    }
                    break;

                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                    printf("Keydown : %c\n", e.key.keysym.sym);
                        case SDLK_q:
                            loop = 0;
                            break;
                        case SDLK_ESCAPE:
                            /* ABANDON */
                            loop = 0;
                            break;
                        case SDLK_UP:
                            /* GO UP */
                            break;
                        case SDLK_DOWN:
                            /* GO DOWN */
                            break;
                        case SDLK_SPACE:
                            /* SHOOT */
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    if(e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP){
                        
                    }
                    break;

                default:
                    break;
                }
            }
        }

        SDL_GL_SwapBuffers();
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    

    /* Free memory 
    SDL_FreeSurface();
    glDeleteTextures(1, &texture3);*/

    SDL_Quit();

}