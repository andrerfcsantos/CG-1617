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

class Transformacao {
public:

	TipoTransformacao tipo;

	virtual ~Transformacao() = default;

	friend std::ostream& operator<<(std::ostream& os, const Transformacao& t) {
		os << t.tipo;
		return os;
	}
};


class Rotacao: public Transformacao {
public:
	float rang, rx, ry, rz;
	
	Rotacao() : Rotacao(0.0f, 0.0f, 1.0f, 0.0f) {
	}

	Rotacao(float p_rang, float p_rx, float p_ry, float p_rz) :
		    rang(p_rang), rx(p_rx),   ry(p_ry),   rz(p_rz) {
		tipo = ROTACAO;
	}
};

class Translacao : public Transformacao {
public:
	float tx, ty, tz;

	Translacao() : Translacao(0.0f, 0.0f, 0.0f) {
	}

	Translacao(float p_tx, float p_ty, float p_tz) :
		tx(p_tx), ty(p_ty), tz(p_tz) {
		tipo = TRANSLACAO;
	}
};

class Escala : public Transformacao {
public:
	float sx, sy, sz;

	Escala(): Escala(1.0f, 1.0f, 1.0f) {
	}

	Escala(float p_sx, float p_sy, float p_sz) :
		   sx(p_sx)  , sy(p_sy)  , sz(p_sz){
		tipo = ESCALA;
	}
};
