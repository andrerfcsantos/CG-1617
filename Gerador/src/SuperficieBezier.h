#pragma once

#include <vector>
#include "../../utils/Coordenadas3D.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../../utils/MatrizOp.h"

class ComponentesPonto {
public:
	std::vector<Coordenadas3D> pontos;
	std::vector<Coordenadas3D> normais;
	std::vector<CoordsTextura> coordsTextura;
}

class SuperficieBezier
{
  public:
	std::vector<Coordenadas3D> pontosControlo;
	std::vector<std::vector<int>> patches;

	SuperficieBezier &addPontoControlo(Coordenadas3D ponto) {
		pontosControlo.push_back(ponto);
		return *this;
	}

	SuperficieBezier &addPatch(std::vector<int> patch) {
		patches.push_back(patch);
		return *this;
	}

	ComponentesPonto getPontos(int divsU, int divsV) {
		ComponentesPonto cp;
		int i = 0, j = 0;

		float m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
						  {3.0f, -6.0f,  3.0f, 0.0f},
						  {-3.0f,  3.0f,  0.0f,  0.0f},
						  {1.0f,  0.0f,  0.0f,  0.0f } };

		float mt[4][4] = { { -1.0f,  3.0f, -3.0f,  1.0f },
						  { 3.0f, -6.0f,  3.0f, 0.0f },
						  { -3.0f,  3.0f,  0.0f,  0.0f },
		                  { 1.0f,  0.0f,  0.0f,  0.0f } };


		for (std::vector<int> patch : patches) {
			Coordenadas3D P[4][4];

			for (i = 0; i < 4; i++) {
				for (j = 0; j < 4; j++) {
					P[i][j] = pontosControlo[patch[i*4+j]];
				}
			}

			Coordenadas3D mr[4][4];
			Coordenadas3D mPmT[4][4];

			multMatrixFP(m,P,mr);
			multMatrixPF(mr,mt,mPmT);


			float deltaU = (float) 1.0f / divsU;
			float deltaV = (float) 1.0f / divsV;

			for (i = 0; i < (divsU); i++) {
				float u1 = i*deltaU;
				float u2 = (i+1)*deltaU;

				for (j = 0; j < (divsV);j++) {
					float v1 = j*deltaV;
					float v2 = (j+1)*deltaV;

					Coordenadas3D b1 = calculaB(mPmT, u1, v1);
					Coordenadas3D b2 = calculaB(mPmT, u2, v1);
					Coordenadas3D b3 = calculaB(mPmT, u1, v2);
					Coordenadas3D b4 = calculaB(mPmT, u2, v2);

					Coordenadas3D n1 = calculaNormal(mPmT, u1, v1);
					Coordenadas3D n2 = calculaNormal(mPmT, u2, v1);
					Coordenadas3D n3 = calculaNormal(mPmT, u1, v2);
					Coordenadas3D n4 = calculaNormal(mPmT, u2, v2);

					cp.pontos.push_back(b1);
					cp.textCoords.push_back(CoordsTextura{u1, v1});
					cp.textCoords.push_back(n1);

					cp.pontos.push_back(b3);
					cp.textCoords.push_back(CoordsTextura{u1, v2});
					cp.textCoords.push_back(n3);

					cp.pontos.push_back(b2);
					cp.textCoords.push_back(CoordsTextura{u2, v1});
					cp.textCoords.push_back(n2);

					cp.pontos.push_back(b2);
					cp.textCoords.push_back(CoordsTextura{u2, v1});
					cp.textCoords.push_back(n2);

					cp.pontos.push_back(b3);
					cp.textCoords.push_back(CoordsTextura{u1, v2});
					cp.textCoords.push_back(n3);

					cp.pontos.push_back(b4);
					cp.textCoords.push_back(CoordsTextura{u2, v2});
					cp.textCoords.push_back(n4);

				}
			}

		}

		return res;
	}



};