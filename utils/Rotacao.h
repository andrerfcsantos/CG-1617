#pragma once

#include "Transformacao.h"

class Rotacao : public Transformacao {
public:
	float rang, rx, ry, rz;

	Rotacao() : rang(0), rx(0), ry(0), rz(0) {
		tipo = ROTACAO;
	}

};
