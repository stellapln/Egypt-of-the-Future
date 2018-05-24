#include "../include/game.h"


ListeElements initBg(float x, float y, float speed, char type){
	ListeElements tmp = (Element*)malloc(sizeof(Element));

	tmp->pos.x = x;
	tmp->pos.y = y;
	tmp->speed = speed;
	tmp->next = NULL;
    tmp->type = type;

	return tmp;
}

ListeElements initElement(float x, float y, int life, float speed, char type){
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
    tmp->type = type;
    /* Bounding Box */
    tmp->pmax.x = tmp->pos.x + 0.25*PPM_WIDTH/60.0;
    tmp->pmax.y = tmp->pos.y + 1*PPM_HEIGHT/40.0;
    tmp->pmin.x = tmp->pos.x - 0.25*PPM_WIDTH/60.0;
    tmp->pmin.y = tmp->pos.y - 1*PPM_HEIGHT/40.0;

	return tmp;
}

ListeElements initShip(float x, float y, int life, float speed, char type){
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
    tmp->type = type;
    /* Bounding Box */
    tmp->pmax.x = tmp->pos.x + (0.5*(WINDOW_WIDTH/WINDOW_HEIGHT)*PPM_WIDTH)/60.0 - 0.5;
    tmp->pmax.y = tmp->pos.y + (WINDOW_WIDTH/WINDOW_HEIGHT)*PPM_HEIGHT/40.0;
    tmp->pmin.x = tmp->pos.x - (0.5*(WINDOW_WIDTH/WINDOW_HEIGHT)*PPM_WIDTH)/60.0 + 0.5;
    tmp->pmin.y = tmp->pos.y - (WINDOW_WIDTH/WINDOW_HEIGHT)*PPM_HEIGHT/40.0;

    return tmp;
}

void addElementToList(ListeElements *lst, float x, float y, int life, float speed, char type){
	Element *tmp = initElement(x, y, life, speed, type);
	if (*lst != NULL){
		tmp->next = *lst;
    }
    *lst = tmp;
}

void pushElements(World *world, int level){
  FILE *file = NULL;

	if(level == 1)
		file = fopen("./img/ppm/lvl1.ppm", "r"); // level 1
    else if (level == 2)
	   file = fopen("./img/ppm/lvl2.ppm", "r");
    else if (level == 3)
       file = fopen("./img/ppm/lvl3.ppm", "r");
	char m[10]; // bin array
	int i; // bin integer
	int r,g,b; // R G B value
	fscanf(file, "%s\n%s %s %s %s %s %s %s\n",m,m,m,m,m,m,m,m); // Erase first ppm lines
	fscanf(file, "%d %d\n", &(PPM_WIDTH), &(PPM_HEIGHT));
	fscanf(file, "%d\n", &i);
	int x = 0;
	int y = PPM_HEIGHT;

	while(fscanf(file, "%d\n%d\n%d\n", &r,&g,&b) != EOF){
		if(x ==PPM_WIDTH){
			x = 0;
			y--;
		}
        if (g == 255 && r == 255 && b == 255){ //Blanc
        }
        else if (g == 0 && r == 0 && b == 0){ //Noir
            addElementToList(&world->obstacles, x, y, OBSTACLES_LIFE, world->speed, 'o');
            loadImgPNG("./img/elts/wall.png", world->obstacles);
        }
        else if (g == 0 && r == 255 && b == 0){ //Red
            addElementToList(&world->mobs, x, y, MOBS_LIFE, world->speed, 'm');
            loadImgPNG("./img/elts/mummy.png", world->mobs);
        }
        else if (g == 255 && r == 0 && b == 0){ //Green
            addElementToList(&world->bonus, x, y, BONUS_LIFE, world->speed, 'b');
            loadImgPNG("./img/elts/eye.png", world->bonus);
        }
        else if (g == 255 && r == 255 && b == 0){ //Jaune
            addElementToList(&world->mobs, x, y, MOBS_LIFE + 1, world->speed, 'c');
            loadImgPNG("./img/elts/cat.png", world->mobs);
        }
        else if (g == 200 && r == 200 && b == 200){ //Grey
            addElementToList(&world->mobs, x, y, MOBS_LIFE + 3, world->speed, 'd');
            loadImgPNG("./img/elts/dromedary.png", world->mobs);
        }
        else if (g == 255 && r == 0 && b == 255){ //Cyan
            addElementToList(&world->pyramid, x, y, MOBS_LIFE, world->speed, 'p');
            loadImgPNG("./img/elts/pyramid.png", world->pyramid);
        }
        else if (g == 0 && r == 0 && b == 255){ //Blue
            addElementToList(&world->key, x, y, KEY_LIFE, world->speed, 'k');
            loadImgPNG("./img/elts/key.png", world->key);
        }
        else{ //Violet
            world->ship = initShip( x, y, SHIP_LIFE, 0, 's');
            loadImgPNG("./img/elts/ship.png", world->ship);
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
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) ==-1){
       // printf("%s\n", Mix_Error());
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}

void initializeElements(World *world, int level){
	/* Backgrounds stuff */
	float bgx1 = 30.0;
	float bgy1 = 20.0;
	float bgx2 = 90.0;
	float bgy2 = 20.0;
    world->backgrounds = initBg(bgx1,bgy1,0.005, 'g'); // 1st bg
    world->backgrounds->next = initBg(bgx2,bgy2,0.005, 'g'); // 2nd bg
    if(level == 1){
        loadImgPNG("./img/fds/bg.png", world->backgrounds);
        loadImgPNG("./img/fds/gb.png", world->backgrounds->next);
    }
    else if(level == 2){
        loadImgPNG("./img/fds/bg2.png", world->backgrounds);
        loadImgPNG("./img/fds/gb2.png", world->backgrounds->next);
    }
    else if(level == 3){
        loadImgPNG("./img/fds/bg3.png", world->backgrounds);
        loadImgPNG("./img/fds/gb3.png", world->backgrounds->next);
    }
    world->menu = NULL;
    world->gameover = NULL;
    world->win = NULL;
    world->miss = NULL;
    world->end = NULL;
    addElementToList(&world->menu, 30.0, 20.0, 1, 0, 'n');
    loadImgPNG("./img/MENU/MENU.png", world->menu);
    addElementToList(&world->gameover, 30, 20, 1, 0, 'n');
    loadImgPNG("./img/MENU/gameover.png", world->gameover);
    addElementToList(&world->win, 30, 20, 1, 0, 'n');
    loadImgPNG("./img/MENU/win.png", world->win);
    addElementToList(&world->miss, 30, 20, 1, 0, 'n');
    loadImgPNG("./img/MENU/miss.png", world->miss);
    addElementToList(&world->end, 30, 20, 1, 0, 'n');
    loadImgPNG("./img/MENU/end.png", world->end);
	/* Other lists world initialization */
    world->ship = NULL;
    world->mobs = NULL;
    world->obstacles = NULL;
    world->arrows = NULL;
    world->key = NULL;
    world->bonus = NULL;
    world->pyramid = NULL;
    world->hearts = NULL;
    world->level = level;
    world->speed = WORLD_SPEED+level/100.0;
    pushElements(world, level);

    /*init hearts*/
    addElementToList(&world->hearts, 0.5, 39.0, 1, 0, 'h');
    loadImgPNG("./img/elts/heart.png", world->hearts);      
}

int initializeGame(World *world, int level){
    /* SDL initialization */
    initializeSDL();

    /* ELEMENTS initialization */
    initializeElements(world, level);

    return EXIT_SUCCESS;
}

int collisionShip(World world){
    int c_obstacles;
    int c_bonus;
    int c_key;
    int c_mobs;
    c_obstacles = collisionElement(world.obstacles, world.ship);
    if(c_obstacles){
        return -1;
    }
    c_bonus = collisionElement(world.bonus, world.ship);
    if(c_bonus){
        return 1;
    }
    c_key = collisionElement(world.key, world.ship);
    if(c_key){
        return 2;
    }
    c_mobs = collisionElement(world.mobs, world.ship);
    if(c_mobs){
        return -2;
    }
    return 0;
}

int collisionArrow(World world, ListeElements larrows){
    int c_obstacles;
    int c_mobs;

    if (world.arrows == NULL){
        return 0;
    }

    c_obstacles = collisionElement(world.obstacles, larrows);
    if(c_obstacles){
        return -1;
    }
    c_mobs = collisionElement(world.mobs, larrows);
    if(c_mobs){
        return 1;
    }
    return 0;
}

int collisionElement(ListeElements bad, ListeElements good){
    while(bad != NULL){
        if(good->pmax.x >= bad->pmin.x && good->pmin.x <= bad->pmax.x){
            if(good->pmax.y >= bad->pmin.y && good->pmin.y <= bad->pmax.y){
                if(bad->type != 'b'){
                    good->life--;
                }
                bad->life--;
                return 1;
            }
        }
        bad = bad->next;
    }
    return 0;
}

ListeElements cleanList(ListeElements l, int entire){
    if(l == NULL){
        return NULL;
    }
    if(entire == 0){
        if(l->life == 0){
            ListeElements tmp = l->next;
            glDeleteTextures(1, &l->textureID);
            free(l);
            tmp = cleanList(tmp, 0);
            return tmp;
        }
        else {
            l->next = cleanList(l->next, 0);
            return l;
        }
    }
    ListeElements tmp = l->next;
    glDeleteTextures(1, &l->textureID);
    free(l);
    tmp = cleanList(tmp, 0);
    return tmp;
}

void sendWeapon(World *w){
    ListeElements tmp = w->mobs;
    while(tmp!=NULL){
        if(tmp->type == 'c'){
            if(((tmp->pos.x) >= 40.0 && (tmp->pos.x) <= 40.06) || 
                ((tmp->pos.x) >= 50.0 && (tmp->pos.x) <= 50.06) ||
                ((tmp->pos.x) >= 30.0 && (tmp->pos.x) <= 30.06)){
                addElementToList(&w->mobs, tmp->pos.x-0.5, tmp->pos.y, 1, 0.15, 'w');
                loadImgPNG("./img/elts/weapon.png", w->mobs);
                break;
            }
        }
      tmp = tmp->next;
    }
}

void freeWorld(World *world, int entire){
    world->obstacles = cleanList(world->obstacles, 1);
    world->arrows = cleanList(world->arrows, 1);
    world->ship = cleanList(world->ship, 1);
    world->mobs = cleanList(world->mobs, 1);
    world->backgrounds = cleanList(world->backgrounds, 1);
    world->key = cleanList(world->key, 1);
    world->bonus = cleanList(world->menu, 1);

    if(entire){
        world->speed = 0;
        world->level = 0;
        world->gameover = NULL;
        world->menu = NULL;
        world->win = NULL;
        world->miss = NULL;
        free(world);
    }
}

void scoreToTab(int* tab, int points){
    /* unité */
    if(points >= 0){
        tab[0] = points;
        /* décimal */
        if(points >= 10){
            tab[1] = points/10;
            tab[0] = points-tab[1]*10;
            /* centimal */
            if(points >= 100){
                tab[2] = points/100;
                tab[1] = tab[1]-tab[2]*10;
                /* millimal */
                if(points >= 1000){
                    tab[3] = points/1000;
                    tab[2] = tab[2]-tab[3]*10;
                    /* dix-millimal */
                    if(points >= 10000){
                        tab[4] = points/10000;
                        tab[3] = tab[3]-tab[4]*10;
                    }
                    else{
                        tab[4] =0;
                    }
                }
                else{
                    tab[3] = 0;
                    tab[4] = 0;
                }
            }
            else{
                tab[2] = 0;
                tab[3] = 0;
                tab[4] = 0;
            }
        }
        else{
            tab[1] = 0;
            tab[2] = 0;
            tab[3] = 0;
            tab[4] = 0;
        }
    }
}

int readBestScore(){
    FILE *file;
    int bestscore;
    file = fopen("./score.txt", "r");
    fscanf(file, "%d", &bestscore);
    fclose(file);
    return bestscore;
}

void writeBestScore(int score){
    if(score > readBestScore()){
        FILE *file;
        file = fopen("./score.txt", "w");
        fprintf(file, "%d", score);
        fclose(file);
    }
}

void gameLoop(World *world){
    int loop = 1;
    int move = 0;
    int collShip = 0;
    int collArrow = 0;
    int points = 0;
    int tab_points[5];
    int tab_best_points[5];
    int play = 0;
    int countLife = 0;
    int music = 0;
    int i;

    /**********************/

    /* Music initialisation */
    Mix_Music *mmenu = Mix_LoadMUS("./son/menu.mp3");
    Mix_Music *mlvl1 = Mix_LoadMUS("./son/musiclvl1.mp3");
    Mix_Music *mlvl2 = Mix_LoadMUS("./son/musiclvl2.mp3");
    Mix_Music *mlvl3 = Mix_LoadMUS("./son/musiclvl3.mp3");
    Mix_Music *mend =Mix_LoadMUS("./son/end.mp3");
    /* Sound initialisation */
    Mix_AllocateChannels(2);
    Mix_Chunk *sarrow = Mix_LoadWAV("./son/arrow.wav");
    Mix_VolumeChunk(sarrow, MIX_MAX_VOLUME/3);
    Mix_Chunk *swin = Mix_LoadWAV("./son/win.wav");
    Mix_Chunk *spenguin = Mix_LoadWAV("./son/penguin.wav");
    /**********************/

    /* initialize numbers*/
    GLuint tab_text[12];
    char imgName[50];
    for(i = 0; i < 10; i++){
        sprintf(imgName, "./img/nb/%d.png", i);
        loadImgNb(imgName, tab_text, i); 
    }
    loadImgNb("./img/nb/score.png", tab_text, 10);
    loadImgNb("./img/nb/best.png", tab_text, 11);
    /***********************/

    glClearColor(0, 0, 0, 1.0);

    Uint32 startTime = SDL_GetTicks();

	glEnable(GL_BLEND);

    resizeViewport(WINDOW_WIDTH,WINDOW_HEIGHT);


    /*********** LOOP **********/

    while(loop){

        glClear(GL_COLOR_BUFFER_BIT);

        /************************* MENU SCREEN *************************/
        if(play == 0){
            /* Music stuff */
            if(music == 0){
                Mix_PlayMusic(mmenu, -1);
                music = 1;
            }
            drawList(world->menu, WINDOW_WIDTH/30.0,WINDOW_HEIGHT/30.0);
            scoreToTab(tab_best_points, readBestScore());
            for(i = 0; i < 5 ; i++){
                drawScore(tab_text[tab_best_points[i]], 5-i, 1);
            }
            drawScore(tab_text[10], -2, 0);
            drawScore(tab_text[11], -5, 2);
            SDL_Event menu;
            while(SDL_PollEvent(&menu)) {
                switch(menu.type){
                    case SDL_QUIT:
                        loop = 0;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        if(music)Mix_HaltMusic();
                        music = 0;
                        play = 1;
                        break;
                    case SDL_KEYDOWN:
                        switch(menu.key.keysym.sym){
                            case SDLK_q:
                                loop = 0;
                                break;
                            default:
                            break;
                        }
                    default:
                        play = 0;
                        break;
                }
            }        
            SDL_GL_SwapBuffers();
        }

        /************************* PLAY SCREEN *************************/
        else if(play == 1){

            if (music == 0){
                /* Music stuff */
                if(world->level == 1){
                    Mix_PlayMusic(mlvl1,-1);
                }
                else if(world->level == 2){
                    Mix_PlayMusic(mlvl2, -1);
                }
                else if(world->level == 3){
                    Mix_PlayMusic(mlvl3, -1);
                }
                music = 1;
            }

            /******* Move functions ******/
            /* Backgrounds */
            moveBackground(world->backgrounds);
            moveBackground(world->backgrounds->next);
            /* Obstacles */
            moveElements(world->obstacles);
            /* Mobs */
            moveElements(world->mobs);
            moveElementsVertical(*world);
            /* Key */
            moveElements(world->key);
            /* Bonus */
            moveElements(world->bonus);

            /******** Draw functions *******/
            /* Backgrounds */
            drawList(world->backgrounds, WINDOW_WIDTH/30.0, WINDOW_HEIGHT/30.0);
            /* Pyramid */
            if(world->pyramid){
                moveElements(world->pyramid);
                drawList(world->pyramid, 10, 10);
            }
            sendWeapon(world);

            /* Obstacles */
            drawList(world->obstacles, 1, 1);
            /* Mobs */
            drawList(world->mobs, 1, 1);
            /* Key */
            drawList(world->key, 1, 1);
            /* Bonus */
            drawList(world->bonus, 1, 1);
            /* Hearts */
            for (countLife = 1; countLife <= world->ship->life; countLife++) {
                drawElement(world->hearts, 0.5, 0.5);
                world->hearts->pos.x = countLife*1.4;
            }

            /******* Collision functions *******/
            /* Ship */
            switch(collShip = collisionShip(*world)){
                case 0 : 
                        break;
                case 1 : // Bonus
                        points += 100;
                        break; 
                case 2 : // Key end of level
                        points += 500;
                        if(world->level < 3){
                            world->level += 1;
                            Mix_HaltMusic();
                            music = 0;
                            play = 2;
                        }
                        else{
                            Mix_HaltMusic();
                            music = 0;
                            play = 3;
                        }
                        freeWorld(world, 0);
                        initializeElements(world, world->level);

                        break;
                case -1 : // Wall
                        points -= 5;
                        break;
                case -2 : // Mummy
                        points -= -25;
                        break;
                default:
                        break;
            }
            /* Arrow */
            ListeElements tmpA = world->arrows;
            while(world->arrows != NULL){
                switch(collArrow = collisionArrow(*world,world->arrows)){
                    case 0 : 
                            break;
                    case -1 : // Wall
                            points += 10;
                            break;
                    case 1 : // Mummy
                            points += 50;
                            break;
                    default:
                            break;
                }
                world->arrows = world->arrows->next;
            }
            world->arrows = tmpA;

            /* SCORE */
            scoreToTab(tab_points, points);
            for(i = 0; i < 5 ; i++){
                drawScore(tab_text[tab_points[i]], 5-i, 1);
            }
            drawScore(tab_text[10], -2, 0);



            /***** DRAWING IN MOVING *****/
            /* Ship move and drawing */
            if(move != 0){
                moveShip(world->ship, move);
                /* Redrawing */
                glPushMatrix();
                    glTranslatef(world->ship->pos.x, world->ship->pos.y, 0);
                    glRotatef(10.0*move, 0, 0, 1.0); /* rotate down or up */
                    drawShipInMove(world->ship, WINDOW_WIDTH/350.0,WINDOW_HEIGHT/350.0, collShip);
                glPopMatrix();  
            }
            else{
                drawList(world->ship, WINDOW_WIDTH/350.0, WINDOW_HEIGHT/350.0);
            }

            glColor3ub(255, 255, 255);

            /* Arrows */
            if(world->arrows != NULL){
                drawList(world->arrows, 1, 0.3);
                moveElements(world->arrows);
            }

            /***** CONDITIONS *****/
            /* DEATH OF THE SHIP */
            if(world->ship->life == 0){
                Mix_HaltMusic();
                points = 0;
                move = 0;
                music = 0;
                play = -1;
            }
            /* MISS THE PENGOUIN */
            if(world->ship->pmin.x > world->key->pmax.x){
                Mix_HaltMusic();
                points = 0;
                move = 0;
                music = 0;
                play = -2;
            }

            /******* EVENTS LOOP *******/

            SDL_Event e;
            while(SDL_PollEvent(&e)){
                switch(e.type){
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
                        switch(e.key.keysym.sym){
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
                                Mix_PlayChannel(-1,sarrow,0);
                                addElementToList(&world->arrows, world->ship->pos.x-0.6, world->ship->pos.y+0.6, 1, -0.8, 'a');
                                loadImgPNG("./img/elts/arrow.png", world->arrows);
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
                        move = 0;
                    break;
                }
            }
            SDL_GL_SwapBuffers();
        }


        /************************* GAME OVER / MISS SCREEN *************************/
        else if(play == -1 || play == -2){


            if(play == -1){
                drawList(world->gameover, WINDOW_WIDTH/30.0,WINDOW_HEIGHT/30.0);
            }
            else{
                drawList(world->miss, WINDOW_WIDTH/30.0,WINDOW_HEIGHT/30.0);
            }
            if(music == 0){
                Mix_PlayChannel(1,spenguin,0);
                music = 1;
            }
            SDL_Event over;
            while(SDL_PollEvent(&over)){
                switch(over.key.keysym.sym){
                    case SDLK_q:
                        loop = 0;
                        music = 0;
                        break;
                    case SDLK_c:
                        //if(music)Mix_HaltChannels(1);
                        play = 0;
                        music = 0;
                        move = 0;
                        freeWorld(world, 0);
                        initializeElements(world, 1);
                        break;
                    default:
                        break;
                }
            }
            SDL_GL_SwapBuffers();
        }


        /************************* WIN SCREEN *************************/
        else if(play == 2){
            if(music == 0){
                Mix_PlayChannel(1,swin, 0);
                music = 1;
            }

            drawList(world->win, WINDOW_WIDTH/30.0,WINDOW_HEIGHT/30.0);
            SDL_Event win;
            while(SDL_PollEvent(&win)){
                switch(win.type){
                    case SDL_QUIT:
                        loop = 0;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        //if(music)Mix_HaltChannels(1);
                        music = 0;
                        move = 0;
                        play = 1;
                        break;
                    default:
                        break;
                }
            }
            SDL_GL_SwapBuffers();
        }


        /************************* END SCREEN *************************/
        else if(play == 3){
            if(music == 0){
                Mix_PlayMusic(mend, 0);
                music = 1;
            }
            writeBestScore(points);
            drawList(world->end, WINDOW_WIDTH/30.0,WINDOW_HEIGHT/30.0);
            SDL_Event end;
            while(SDL_PollEvent(&end)){
                switch(end.type){
                    case SDL_QUIT:
                        loop = 0;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        //if(music)Mix_HaltMusic();
                        music = 0;
                        loop = 0;
                        break;
                    default:
                        break;
                }
            }
            SDL_GL_SwapBuffers();
        }

        /* Free memory if needed (collision) */
        world->obstacles = cleanList(world->obstacles, 0);
        world->bonus = cleanList(world->bonus, 0);
        world->arrows = cleanList(world->arrows, 0);
        world->mobs = cleanList(world->mobs, 0);
        world->hearts = cleanList(world->hearts, 0);


        /******************* END BIG LOOP ***********************/
    }

    SDL_GL_SwapBuffers();

    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
        SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }

    /* FREE MEMORY */
    freeWorld(world, 1);

    /* FREE MUSIC & SONG */
    Mix_FreeChunk(spenguin);
    Mix_FreeChunk(swin);
    Mix_FreeChunk(sarrow);

    Mix_FreeMusic(mmenu);
    Mix_FreeMusic(mlvl1);
    Mix_FreeMusic(mlvl2);
    Mix_FreeMusic(mlvl3);
    Mix_FreeMusic(mend);

    Mix_CloseAudio();
    SDL_Quit();

}

/********** DEBUG ***********/
void displayList(ListeElements e){
    if (e == NULL) return;
    printf("%f %f %c\n", e->pos.x, e->pos.y, e->type);
    displayList(e->next);
}