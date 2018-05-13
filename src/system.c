#include "../include/system.h"


Element allocElement(float x, float y, float speed, float life, GLuint texture){
	Element tmp = (Element*)malloc(sizeof(Element));

	tmp.x = x;
	tmp.y = y;
	tmp.speed = speed;
	tmp.life = life;
	tmp.textureID = texture;

	return tmp;
}