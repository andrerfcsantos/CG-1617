#pragma once

#include "Transformacao.h"

class Translacao: public Transformacao{
public:
	float tx, ty, tz;

	Translacao() : tx(0), ty(0), tz(0){
		tipo = TRANSLACAO;
	}

};

