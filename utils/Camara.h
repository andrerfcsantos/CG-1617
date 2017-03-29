#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "CoordsEsfericas.h"
#include "Coordenadas3D.h"

class Camara {

public:
	Coordenadas3D p;
	CoordsEsfericas q;
	Coordenadas3D v_d;

	Camara(Coordenadas3D centro = Coordenadas3D{ 0,0,0 }, float azimuth = M_PI / 4.0, float polar= M_PI / 4.0) :
		p(centro),q(centro,1.0, azimuth,polar){
		v_d = q.cCartesianas - p;
	}

	Camara& frente(float k) {
		p = p + v_d.times(k);
		q.centro = p;
		q.refreshCartesianas();
		return *this;
	}

	Camara& tras(float k) {
		p = p - v_d.times(k);
		q.centro = p;
		q.refreshCartesianas();
		return *this;
	}

	Camara& direita(float k) {
		Coordenadas3D v_up = { 0,1,0 };
		Coordenadas3D v_r =  v_d.crossproduct(v_up);
		p = p + v_r.times(k);
		q.centro = p;
		q.refreshCartesianas();
		return *this;
	}

	Camara& esquerda(float k) {
		Coordenadas3D v_up = { 0,1,0 };
		Coordenadas3D v_l = v_up.crossproduct(v_d);
		p = p + v_l.times(k);
		q.centro = p;
		q.refreshCartesianas();
		return *this;
	}

	Camara& cima(float k) {
		Coordenadas3D v_up = { 0,1,0 };
		p = p + v_up.times(k);
		q.centro = p;
		q.refreshCartesianas();
		return *this;
	}

	Camara& baixo(float k) {
		Coordenadas3D v_up = { 0,1,0 };
		p = p - v_up.times(k);
		q.centro = p;
		q.refreshCartesianas();
		return *this;
	}

	Camara& lookBaixo(float ang){
		q.paraBaixo(ang);
		v_d = q.cCartesianas - p;
		return *this;
	}

	Camara& lookCima(float ang) {
		q.paraCima(ang);
		v_d = q.cCartesianas - p;
		return *this;
	}

	Camara& lookDireita(float ang) {
		q.paraEsquerda(ang);
		v_d = q.cCartesianas - p;
		return *this;
	}

	Camara& lookEsquerda(float ang) {
		q.paraDireita(ang);
		v_d = q.cCartesianas - p;
		return *this;
	}

};

