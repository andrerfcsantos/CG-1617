#pragma once
#include <string>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


class DefsDesenho {
public:
	float red, green, blue;
	GLenum modoDesenho;
	DefsDesenho(float p_red = 1, float p_green =1, float p_blue =1, GLenum p_md = GL_LINE) :
		red(p_red), green(p_green), blue(p_blue),modoDesenho(p_md) {
	}
};
