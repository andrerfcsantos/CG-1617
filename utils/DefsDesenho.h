#pragma once
#include <string>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


class DefsDesenho {
public:
	float diffuse[4] = {0.0, 0.0, 0.0, 1.0};
	float ambient[4] = { 0.0, 0.0, 0.0, 1.0 };
	float specular[4] = { 1.0, 1.0, 1.0, 1.0 };
	float emission[4] = { 0.0, 0.0, 0.0, 1.0 };
	float ambient_and_diffuse[4] = { 0.0, 0.0, 0.0, 1.0 };
	float shininess = 0.5;
	bool has_diffuse = false, has_ambient = false, has_specular = false, has_emission = false, has_ambient_and_diffuse = false;
	bool has_shininess=false;
	float red, green, blue;
	GLenum modoPoligonos;
	GLenum modoDesenho;
	DefsDesenho(float p_red = 1, float p_green =1, float p_blue =1, GLenum p_md = GL_LINE) :
		red(p_red), green(p_green), blue(p_blue),modoPoligonos(p_md) {
	}
};
