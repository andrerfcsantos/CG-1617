#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "Coordenadas3D.h"


class CoordsEsfericas {
public:
	// Coordenadas Esfericas
	float raio, azimuth_ang, polar_ang;
	// Coordenadas cartesianas
	Coordenadas3D cCartesianas;
	Coordenadas3D centro;

	CoordsEsfericas(Coordenadas3D p_centro = {0,0,0}, float r = 5.0, float az = M_PI / 4.0, float polar = M_PI / 4.0) :
		centro(p_centro), raio(r), azimuth_ang(az), polar_ang(polar) {
		refreshCartesianas();
	};

	CoordsEsfericas(float r = 5.0, float az = M_PI / 4.0, float polar = M_PI / 4.0) :
		centro(Coordenadas3D{0,0,0}), raio(r), azimuth_ang(az), polar_ang(polar) {
		refreshCartesianas();
	};

	CoordsEsfericas& paraCima(float incremento) {
		float check = polar_ang - incremento;

		if (check > 0.0) polar_ang = check;
		else polar_ang = 0.00001f;

		refreshCartesianas();
		return *this;
	}

	CoordsEsfericas& paraBaixo(float incremento) {
		float check = polar_ang + incremento;
		if (check < M_PI) polar_ang = check;
		else polar_ang = M_PI- 0.00001f;

		refreshCartesianas();
		return *this;
	}

	CoordsEsfericas& paraEsquerda(float incremento) {
		azimuth_ang -= incremento;
		refreshCartesianas();
		return *this;
	}

	CoordsEsfericas& paraDireita(float incremento) {
		azimuth_ang += incremento;
		refreshCartesianas();
		return *this;
	}

	CoordsEsfericas& aproximar(float incremento) {
		float check = raio - incremento;
		if (check <= 0) raio = 0;
		else raio = check;
		refreshCartesianas();
		return *this;
	}

	CoordsEsfericas& afastar(float incremento) {
		raio += incremento;
		refreshCartesianas();
		return *this;
	}

	Coordenadas3D toCartesianas() {
		return cCartesianas;
	}

	void refreshCartesianas() {
		cCartesianas.z = (raio * sin(polar_ang) * cos(azimuth_ang)) + centro.z;
		cCartesianas.x = (raio * sin(polar_ang) * sin(azimuth_ang)) + centro.x;
		cCartesianas.y = (raio * cos(polar_ang)) + centro.y;
	}

	void refreshEsfericas() {
		raio = sqrt(pow(cCartesianas.x, 2) + pow(cCartesianas.y, 2) + pow(cCartesianas.z, 2));
		polar_ang = acos(cCartesianas.y / raio);
		azimuth_ang = atan2(cCartesianas.x, cCartesianas.z);
	}


};


