#pragma once

#include "Coordenadas3D.h"
#define _USE_MATH_DEFINES
#include <math.h>

class CoordsPolares {
public:
	//Centro a partir do qual se considera as coordenadas polares
	Coordenadas3D centro;
	//Coordenadas polares
	float raio, azimuth;
	//Coordenadas rectangulares correspondentes às coordenadas polares
	Coordenadas3D cCartesianas;

	CoordsPolares(Coordenadas3D c = Coordenadas3D{ 0.0,0.0,0.0 }, float r = 5.0f, float az = 0.0f) :
		raio(r), azimuth(az), centro(c) {
		refreshCartesianas();
	}

	Coordenadas3D toCartesianas() {
		return cCartesianas;
	}

private:

	void refreshCartesianas(){
		cCartesianas.x = centro.x + raio * sin(azimuth);
		cCartesianas.y = centro.y;
		cCartesianas.z = centro.z + raio * cos(azimuth);
	}
};