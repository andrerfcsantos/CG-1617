#pragma once

#include "Ponto3D.h"
#define _USE_MATH_DEFINES
#include <math.h>

class CoordsPolares {
public:
	//Centro a partir do qual se considera as coordenadas polares
	Ponto3D centro;
	//Coordenadas polares
	float raio, azimuth;
	//Coordenadas rectangulares correspondentes às coordenadas polares
	Ponto3D cCartesianas;

	CoordsPolares(Ponto3D c = Ponto3D{ 0.0,0.0,0.0 }, float r = 5.0f, float az = 0.0f) :
		raio(r), azimuth(az), centro(c) {
		refreshCartesianas();
	}

	Ponto3D toCartesianas() {
		return cCartesianas;
	}

private:

	void refreshCartesianas(){
		cCartesianas.x = centro.x + raio * sin(azimuth);
		cCartesianas.y = centro.y;
		cCartesianas.z = centro.z + raio * cos(azimuth);
	}
};