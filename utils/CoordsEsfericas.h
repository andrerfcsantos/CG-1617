#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "Ponto3D.h"


class CoordsEsfericas {
public:
	// Coordenadas Esfericas
	float raio, azimuth_ang, polar_ang;
	// Coordenadas cartesianas
	Ponto3D cCartesianas;

	CoordsEsfericas(float r = 5.0, float az = M_PI / 4.0, float polar = M_PI / 4.0) :
		raio(r), azimuth_ang(az), polar_ang(polar) {
		refreshCartesianas();
	};

	CoordsEsfericas(Ponto3D pto) :
		cCartesianas(pto) {
		refreshEsfericas();
	};

	CoordsEsfericas& deslocaX(float dx) {
		cCartesianas.x += dx;
		refreshEsfericas();
		return *this;
	}

	CoordsEsfericas& deslocaY(float dy) {
		cCartesianas.y += dy;
		refreshEsfericas();
		return *this;
	}

	CoordsEsfericas& deslocaZ(float dz) {
		cCartesianas.z += dz;
		refreshEsfericas();
		return *this;
	}

	CoordsEsfericas& paraCima(float incremento) {
		float check = polar_ang - incremento;

		if (check >= 0.0) polar_ang = check;
		else polar_ang = 0;

		refreshCartesianas();
		return *this;
	}

	CoordsEsfericas& paraBaixo(float incremento) {
		float check = polar_ang + incremento;
		if (check < M_PI) polar_ang = check;
		else polar_ang = M_PI;

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

	Ponto3D toCartesianas() {
		return cCartesianas;
	}

private:
	void refreshCartesianas() {
		cCartesianas.z = raio * sin(polar_ang) * cos(azimuth_ang);
		cCartesianas.x = raio * sin(polar_ang) * sin(azimuth_ang);
		cCartesianas.y = raio * cos(polar_ang);
	}

	void refreshEsfericas() {
		raio = sqrt(pow(cCartesianas.x, 2) + pow(cCartesianas.y, 2) + pow(cCartesianas.z, 2));
		polar_ang = acos(cCartesianas.y / raio);
		azimuth_ang = atan2(cCartesianas.x, cCartesianas.z);
	}


};


