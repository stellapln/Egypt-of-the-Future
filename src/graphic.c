#include "../include/graphic.h"

void resizeViewport(int w, int h) {
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW); // PROJECTION?
    glLoadIdentity();
    gluOrtho2D(0., 60., 0.,40.);
}

void drawElement(){

}

void createImgPNG(GLuint *textureID, SDL_Surface* img){
	glGenTextures(1,&(*textureID));
	glBindTexture(GL_TEXTURE_2D,*textureID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_BYTE,img->pixels);
	glBindTexture(GL_TEXTURE_2D,0);
}

void loadImgPNG(char imgSrc[], ListeElement *elements){
	SDL_Surface *img = IMG_Load(&imgSrc);
	if(img == NULL){
		printf("ERROR at loading image\n");
		exit(1);
	}
	GLuint id;
	createImgPNG(&id, img);
	elements->textureID = id;
}