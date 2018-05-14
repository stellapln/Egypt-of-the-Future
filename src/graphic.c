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

void createImgPNG(GLuint *textureID, SDL_Surface* img){
	glGenTextures(1,&(*textureID));
	glBindTexture(GL_TEXTURE_2D,*textureID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_BYTE,img->pixels);
	glBindTexture(GL_TEXTURE_2D,0);
}

void loadImgPNG(char imgSrc[], ListeElements elements){
	SDL_Surface *img = IMG_Load(imgSrc);
	if(img == NULL){
		printf("ERROR at loading image from %s\n", imgSrc);
		exit(1);
	}
	GLuint id;
	createImgPNG(&id, img);
	elements->textureID = id;
}

void drawList(ListeElements list){
	while(list!=NULL){
		drawElement(list);
		list=list->next;
	}
}

void drawElement(Element *e){
	e->pos.x -= e->speed;
    /* Transparency */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /* Brackground drawing */
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, e->textureID);
        glPushMatrix();
        	glTranslatef(e->pos.x, e->pos.y, 0);
            glScalef(1.0,1.0,1.0);
            glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex2f(-1,1);

            glTexCoord2f(1,0);
            glVertex2f(1,1);

            glTexCoord2f(1,1);
            glVertex2f(1,-1);

            glTexCoord2f(0,1);
            glVertex2f(-1,-1); // PAS SÛRE POUR TEXCOORD2F
            glEnd();
        glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void moveElement(Element *e){
	glPushMatrix();
    	glTranslatef(e->pos.x-e->speed, e->pos.y, 0);
        glScalef(1.0,1.0,1.0);
        glEnd();
    glPopMatrix();
}

void moveShip(Element *e, int move){
    /* Up */
    if (move == 1){
        if(e->pos.x + e->speed <= 40){
            e->pos.y += e->speed;
        }
    }
    /* Down */
    else if (move == -1){
        if(e->pos.x - e->speed >= 0){
            e->pos.y -= e->speed;
        }
    }
    /* Redrawing */
    glPushMatrix();
        glTranslatef(e->pos.x, e->pos.y, 0);
        if(move != 0){
            glRotatef(10.0*move, 0, 0, 1.0); /* rotate down or up */
        }
        drawElement(e);
    glPopMatrix();
}

void moveBackground(Element *e){
    if(e->pos.x < -1.98) e->pos.x = 1.99;
    else e->pos.x -= e->speed;
}