#include "../include/game.h"


ListeElements initBg(float x, float y, float speed){
	ListeElements tmp = (Element*)malloc(sizeof(Element));

	tmp->pos.x = x;
	tmp->pos.y = y;
	tmp->speed = speed;
	tmp->next = NULL;

	return tmp;
}


ListeElements initElement(float x, float y, int life, float speed){
	Element *tmp = (Element*)malloc(sizeof(Element));
    if(tmp == NULL){
        printf("ERROR AT MALLOC\n");
        return NULL;
    }
	tmp->pos.x = x;
	tmp->pos.y = y;
	tmp->life = life;
	tmp->speed = speed;
	tmp->next = NULL;
    /* Bounding Box */
    tmp->pmax.x = tmp->pos.x + 0.5*PPM_WIDTH/60.0;
    tmp->pmax.y = tmp->pos.y + 0.5*PPM_HEIGHT/40.0;
    tmp->pmin.x = tmp->pos.x - 0.5*PPM_WIDTH/60.0;
    tmp->pmin.y = tmp->pos.y - 0.5*PPM_HEIGHT/40.0;
    printf("%f %f %f %f\n", tmp->pmax.x, tmp->pmax.y,tmp->pmin.x,tmp->pmin.y);

	return tmp;
}

void addElementToList(ListeElements *lst, float x, float y, int life, float speed){
	Element *tmp = initElement(x, y, life, speed);
	if (*lst != NULL){
		tmp->next = *lst;
    }
    *lst = tmp;
}

void pushElements(World *world, int level){
  FILE *file = NULL;

	if(level == 1)
		file = fopen("./img/ppm/new_ppm.ppm", "r"); // level 1
    else
	   file = fopen("./img/ppm/new_ppm.ppm", "r");

	char m[10]; // bin array
	int i; // bin integer
	int r,g,b;
	fscanf(file, "%s\n%s %s %s %s %s %s %s\n",m,m,m,m,m,m,m,m); // Erease first ppm lines
	fscanf(file, "%d %d\n", &(PPM_WIDTH), &(PPM_HEIGHT));
	fscanf(file, "%d\n", &i);
	int x = 0;
	int y = PPM_HEIGHT;

	while(fscanf(file, "%d\n%d\n%d\n", &r,&g,&b) != EOF){
		if(x ==PPM_WIDTH){
			x = 0;
			y--;
		}
	    if(r == 255){
	      	if(g == 0){
	        	if (b == 0) {
	         		//printf("rouge\n");
					addElementToList(&world->mobs, x, y, MOBS_LIFE, ELEMENTS_SPEED);
					loadImgPNG("./img/elts/mummy.png", world->mobs);
	        	}
	        	else{
	          		//printf("violet\n");
					addElementToList(&world->ship, x, y, SHIP_LIFE, 0.1);
					loadImgPNG("./img/elts/ship.png", world->ship);
	       		}
	      	}
	    }
	    else {
		    if(g == 255){
		  	    //printf("vert\n");	  	   
				addElementToList(&world->bonus, x, y, BONUS_LIFE, ELEMENTS_SPEED);
				loadImgPNG("./img/elts/eye.png", world->bonus);
		    }
		    else if (b == 255){
		        //printf("bleu\n");
				addElementToList(&world->key, x, y, KEY_LIFE, ELEMENTS_SPEED);
				loadImgPNG("./img/elts/key.png", world->key);
		    }
		    else {
		        //printf("obs\n");
				addElementToList(&world->obstacles, x, y, OBSTACLES_LIFE, ELEMENTS_SPEED);
				loadImgPNG("./img/elts/wall.png", world->obstacles);
		    }
	    }
	    x++;
	}
	fclose(file);
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
	/* Backgrounds stuff*/
	float bgx1 = 30.0;
	float bgy1 = 20.0;
	float bgx2 = 90.0;
	float bgy2 = 20.0;
	world->backgrounds = initBg(bgx1,bgy1,BACKGROUND_SPEED); // 2 bg
	world->backgrounds->next = initBg(bgx2,bgy2,BACKGROUND_SPEED); // 2 bg
	loadImgPNG("./img/fds/bg.png", world->backgrounds);
	loadImgPNG("./img/fds/gb.png", world->backgrounds->next);
	/* Other lists world initialization */
    world->ship = NULL;
    world->mobs = NULL;
    world->obstacles = NULL;
    world->arrows = NULL;
    world->key = NULL;
    world->bonus = NULL;

    pushElements(world, level);
    //displayList(world->obstacles);

}


int initializeGame(World *world, int level){

    /* SDL initialization */
    initializeSDL();

    /* ELEMENTS initialization */
    initializeElements(world, level);

    /* */
    return EXIT_SUCCESS;

}

void displayList(ListeElements e){
	if (e == NULL) return;
	printf("%f %f\n", e->pos.x, e->pos.y);
	displayList(e->next);
}

void gameLoop(World *world){
    int loop = 1;
    int move = 0;

    glClearColor(0, 0, 0, 1.0);

    Uint32 startTime = SDL_GetTicks();

	glEnable(GL_BLEND);


    resizeViewport(WINDOW_WIDTH,WINDOW_HEIGHT);


    /*********** LOOP **********/

    while(loop) {

        glClear(GL_COLOR_BUFFER_BIT);

	    //glLoadIdentity();


	    /* Move functions */
        /* Backgrounds */
	    moveBackground(world->backgrounds);
	    moveBackground(world->backgrounds->next);
        /* Obstacles */
        moveElements(world->obstacles, ELEMENTS_SPEED);
        /* Mobs */
        moveElements(world->mobs, ELEMENTS_SPEED);
        /* Key */
        moveElements(world->key, ELEMENTS_SPEED);
        /* Bonus */
        moveElements(world->bonus, ELEMENTS_SPEED);

        /* Draw functions*/
        /* Backgrounds */
	    drawList(world->backgrounds, WINDOW_WIDTH/30.0, WINDOW_HEIGHT/30.0);
        /* Obstacles */
        drawList(world->obstacles, 1, 1);
        /* Mobs */
        drawList(world->mobs, 1, 1);
        /* Key */
        drawList(world->key, 1, 1);
        /* Bonus */
        drawList(world->bonus, 1, 1);

        drawBB(world->obstacles);
        drawBB(world->mobs);
        drawBB(world->bonus);
        drawBB(world->key);
		drawLandmark();

        /* Ship move and drawing */
        if(move != 0){
        	moveShip(world->ship, move);
            /* Redrawing */
            glPushMatrix();
                glTranslatef(world->ship->pos.x, world->ship->pos.y, 0);
                glRotatef(10.0*move, 0, 0, 1.0); /* rotate down or up */
                drawShipInMove(world->ship, WINDOW_WIDTH/350.0,WINDOW_HEIGHT/350.0);
            glPopMatrix();  
        }
        else{
            drawList(world->ship, WINDOW_WIDTH/350.0, WINDOW_HEIGHT/350.0);
        }

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
                        case SDLK_q:
                            loop = 0;
                            break;
                        case SDLK_ESCAPE:
                            /* ABANDON */
                            loop = 0;
                            break;
                        case SDLK_UP:
                            /* GO UP */
                        	move = 1;
                            break;
                        case SDLK_DOWN:
                            /* GO DOWN */
                        	move = -1;
                            break;
                        case SDLK_SPACE:
                            /* SHOOT */
                            break;
                        default:
                       		move = 0;
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    if(e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP){
                        move = 0;
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