#pragma once
#include <string>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


class DefsDesenho {
public:
	float diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 0;
	float red, green, blue;
	GLenum modoPoligonos;
	GLenum modoDesenho;
	DefsDesenho(float p_red = 1, float p_green =1, float p_blue =1, GLenum p_md = GL_LINE) :
		red(p_red), green(p_green), blue(p_blue),modoPoligonos(p_md) {
	}
};
