#pragma once
#include <stdlib.h>
#include <iostream>
#include <fstream>

enum TipoTransformacao { ROTACAO, TRANSLACAO, ESCALA };


std::ostream& operator<<(std::ostream& os, TipoTransformacao c){
	switch (c)
	{
	case ROTACAO:    os << "ROTACAO";       break;
	case TRANSLACAO: os << "TRANSLACAO";    break;
	case ESCALA:     os << "ESCALA";        break;
	default: os.setstate(std::ios_base::failbit);
	}
	return os;
}

class Rotacao {
public:
	float rang, rx, ry, rz;
	float time;
	bool ccw;

	Rotacao() : Rotacao(0.0f, 0.0f, 1.0f, 0.0f, true) {
	}

	Rotacao(float p_rang, float p_rx, float p_ry, float p_rz, bool p_ccw) :
			rang(p_rang), rx(p_rx),   ry(p_ry),   rz(p_rz), ccw(p_ccw) {
	}
};

class Translacao {
public:
	float tx, ty, tz, time;
	std::vector <Coordenadas3D> ctrlPoints;
	bool ccw;

	Translacao() : Translacao(0.0f, 0.0f, 0.0f, true) {
	}

	Translacao(float p_time) : time(p_time) {
	}

	Translacao(float p_tx, float p_ty, float p_tz, bool p_ccw) :
			tx(p_tx), ty(p_ty), tz(p_tz), time(-1.0f), ccw(p_ccw) {
	}
};

class Escala {
public:
	float sx, sy, sz;

	Escala(): Escala(1.0f, 1.0f, 1.0f) {
	}

	Escala(float p_sx, float p_sy, float p_sz) :
			sx(p_sx)  , sy(p_sy)  , sz(p_sz){
	}
};


class Transformacao {
public:
	TipoTransformacao tipo;

	union UTr {
		Translacao t;
		Rotacao r;
		Escala e;

		UTr() {memset(this, 0, sizeof(UTr));}
		~UTr() {}
	} Tr;

	Transformacao(TipoTransformacao tt) : tipo(tt) {
	}
	
	Transformacao(const Transformacao &t2) {
		tipo = t2.tipo;
		switch (t2.tipo) {
		case TRANSLACAO:
			Tr.t = t2.Tr.t;
			break;
		case ROTACAO:
			Tr.r = t2.Tr.r;
			break;
		case ESCALA:
			Tr.e = t2.Tr.e;
			break;
		}
	}
	
	

	friend std::ostream& operator<<(std::ostream& os, const Transformacao& t) {
		os << t.tipo;
		return os;
	}
};