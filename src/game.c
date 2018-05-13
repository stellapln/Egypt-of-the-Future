#include "../include/game.h"

int initializeSDL(int level){

    /* SDL initialization */
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

void gameLoop(){
    int loop = 1;

    glClearColor(0, 0, 0, 1.0);

	glEnable(GL_BLEND);

    Uint32 startTime = SDL_GetTicks();

    ListeElements Background = allocElement(0,0,0.1,0,); // 2 bg
    Element Ship;
    ListeElements Arrow = NULL;
    ListeElements Mobs = NULL;
    ListeElements Obstacles = NULL;
    ListeElements Bonus = NULL;
    ListeElements Keys = NULL;

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