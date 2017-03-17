#pragma once

#include "Transformacao.h"

class Escala : public Transformacao {
public:
	float sx, sy, sz;

	Escala() : sx(1), sy(1), sz(1) {
		tipo = ESCALA;
	}

};
