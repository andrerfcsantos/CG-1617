#pragma once
#include <vector>
#include "Coordenadas3D.h"
#include "DefsDesenho.h"

struct Desenho {
	DefsDesenho defsDesenho;
	std::vector<Coordenadas3D> pontos;
	int nBuff;
};


