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
	float tx, ty, tz;
	float sx, sy, sz;
	float rang, rx, ry, rz;
	TipoTransformacao tipo;

	static Transformacao& getRotacao(float p_rang, float p_rx, float p_ry, float p_rz) {
		Transformacao t;
		t.tipo = ROTACAO;
		t.rang = p_rang;
		t.rx = p_rx;
		t.ry = p_ry;
		t.rz = p_rz;
		return t;
	}

	static Transformacao& getTranslacao(float p_tx, float p_ty, float p_tz) {
		Transformacao t;
		t.tipo = TRANSLACAO;
		t.tx = p_tx;
		t.ty = p_ty;
		t.tz = p_tz;
		return t;
	}
	
	static Transformacao& getEscala(float p_sx, float p_sy, float p_sz) {
		Transformacao t;
		t.tipo = ESCALA;
		t.sx = p_sx;
		t.sy = p_sy;
		t.sz = p_sz;
		return t;
	}

	friend std::ostream& operator<<(std::ostream& os, const Transformacao& t) {
		os << t.tipo;
		return os;
	}
};
