#pragma once
#include "Coordenadas3D.h"
#define _USE_MATH_DEFINES
#include <cmath>

void multMatrixFP(float m[4][4], Coordenadas3D P[4][4], Coordenadas3D mr[4][4]) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Coordenadas3D soma = {0.0f,0.0f,0.0f};
			for (int k = 0; k < 4; k++) {
				soma = soma + (P[k][j] * m[i][k]);
			}
			mr[i][j] = soma;
		}
	}

}


void multMatrixPF(Coordenadas3D mr[4][4], float mt[4][4], Coordenadas3D r[4][4]) {


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Coordenadas3D soma = { 0.0f,0.0f,0.0f };
			for (int k = 0; k < 4; k++) {
				soma = soma + (mr[i][k] * mt[k][j]);
			}
			r[i][j] = soma;
		}
	}

}


void multMatrix_VP(float m[4], Coordenadas3D P[4][4], Coordenadas3D mr[4]) {

	for (int i = 0; i < 4; i++) {
		mr[i] = Coordenadas3D{ 0.0f,0.0f,0.0f };
		for (int j = 0; j < 4; j++) {
			mr[i] = mr[i] + (P[j][i] * m[j]);
		}
	}

}


Coordenadas3D multMatrix_PV(Coordenadas3D P[4], float m[4]) {
	Coordenadas3D res = { 0.0f,0.0f,0.0f };
	for (int i = 0; i < 4; i++) {
		res = res + (P[i] * m[i]);
	}
	return res;
}

Coordenadas3D calculaB(Coordenadas3D r[4][4], float u, float v) {
	Coordenadas3D rf;
	Coordenadas3D ru[4];
	float vu[4] = { pow(u,3), pow(u,2), u, 1 };
	float vv[4] = { pow(v,3),pow(v,2),v,1 };
	multMatrix_VP(vu, r, ru);
	rf = multMatrix_PV(ru, vv);
	return rf;

}

Coordenadas3D calculaNormal(Coordenadas3D r[4][4], float u, float v){
	Coordenadas3D du,dv,dr;
	float vu[4] = { pow(u,3), pow(u,2), u, 1 };
	float dvu[4] = { 3*u*u, 2*u, 1, 0 };
	float vv[4] = { pow(v,3),pow(v,2),v,1 };
	float dvv[4] = { 3*v*v,2*v,1,0};
	Coordenadas3D ru[4], rv[4];

	multMatrix_VP(dvu, r, ru);
	du = multMatrix_PV(ru,vv);

	multMatrix_VP(vu, r, rv);
	dv = multMatrix_PV(rv,dvv);

	dr = du.crossproduct(dv);
	return dr;
}