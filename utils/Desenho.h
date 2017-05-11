#pragma once
#include <vector>
#include "Coordenadas3D.h"
#include "DefsDesenho.h"
#include "CoordsTextura.h"

struct Desenho {
	DefsDesenho defsDesenho;
	std::vector<Coordenadas3D> pontos;
	std::vector<Coordenadas3D> normais;
	std::vector<CoordsTextura> coordsText;
	int nBuffPontos, nBuffNormal, nBuffTex, idTex;
};


