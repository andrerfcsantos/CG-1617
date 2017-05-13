#pragma once


#include <vector>
#include <fstream>
#include "Coordenadas3D.h"
#include "CoordsPolares.h"
#include "CoordsEsfericas.h"
#include "CoordsTextura.h"


using namespace std;

enum ORIENTACAO_FIG { CIMA, BAIXO, AMBOS };

class Figura {
public:
	
	Figura& geraEsfera(Coordenadas3D o,
						float raio, int fatias, int camadas) {
		Coordenadas3D A, B, C, D;
		CoordsTextura ctA, ctB, ctC, ctD;
		Coordenadas3D na, nb, nc, nd;

		float deltaAz = (float)2 * M_PI / fatias;
		float deltaPolar = (float)M_PI / camadas;

		for (int j = 0; j < camadas; ++j) {
			for (int i = 0; i < fatias; ++i) {
				/*
					A ---------- B               i
					|            |           ----->    
					|            |       j |
					C ---------- D         v
				*/

				A = CoordsEsfericas(raio, deltaAz*i, deltaPolar*j).toCartesianas() + o;
				B = CoordsEsfericas(raio, deltaAz*(i + 1), deltaPolar*j).toCartesianas() + o;
				C = CoordsEsfericas(raio, deltaAz*i, deltaPolar*(j + 1)).toCartesianas() + o;
				D = CoordsEsfericas(raio, deltaAz*(i + 1), deltaPolar*(j + 1)).toCartesianas() + o;
				
				na = A - o;
				nb = B - o;
				nc = C - o;
				nd = D - o;
				float pi = (float) M_PI;
				ctA = CoordsTextura{ (deltaAz*i) / (2.0f * pi), 1.0f - (deltaPolar*j) / pi };
				ctB = CoordsTextura{ (deltaAz*(i + 1)) / (2.0f * pi), 1.0f - (deltaPolar*j) / pi };
				ctC = CoordsTextura{ (deltaAz*i) / (2.0f * pi), 1.0f - (deltaPolar*(j + 1)) / pi };
				ctD = CoordsTextura{ (deltaAz*(i + 1)) / (2.0f * pi), 1.0f - (deltaPolar*(j + 1)) / pi };

				pontos.push_back(A);
				normais.push_back(na.normalize());
				textCoords.push_back(ctA);

				pontos.push_back(C);
				normais.push_back(nc.normalize());
				textCoords.push_back(ctC);

				pontos.push_back(D);
				normais.push_back(nd.normalize());
				textCoords.push_back(ctD);

				pontos.push_back(A);
				normais.push_back(na.normalize());
				textCoords.push_back(ctA);
				
				pontos.push_back(D);
				normais.push_back(nd.normalize());
				textCoords.push_back(ctD);

				pontos.push_back(B);
				normais.push_back(nb.normalize());
				textCoords.push_back(ctB);
			}
		}

		return *this;
	}

	Figura& geraAnel(Coordenadas3D o,
						float raioInterno, float raioExterno,int fatias,
						ORIENTACAO_FIG orientacao) {
		float deltaAz = (float)2 * M_PI / fatias;
		Coordenadas3D A, B, C, D;
		CoordsTextura ctA, ctB, ctC, ctD;
		Coordenadas3D up = Coordenadas3D{0,1,0};
		Coordenadas3D down = Coordenadas3D{ 0,-1,0 };


		for (int i = 0; i < fatias; ++i) {
			/*
			A ---------- C  raio interno        i
			|            |                   ----->
			|            |       
			B ---------- D  raio externo
			*/

			A = CoordsPolares(o, raioInterno, deltaAz*i).toCartesianas();
			B = CoordsPolares(o, raioExterno, deltaAz*i).toCartesianas();
			C = CoordsPolares(o, raioInterno, deltaAz*(i + 1)).toCartesianas();
			D = CoordsPolares(o, raioExterno, deltaAz*(i + 1)).toCartesianas();

			ctA = CoordsTextura{ (deltaAz*i) / (float)(2.0*M_PI)      , 1 };
			ctB = CoordsTextura{ (deltaAz*i) / (float)(2.0 * M_PI)      , 0 };
			ctC = CoordsTextura{ (deltaAz*(i + 1)) / (float)(2.0 * M_PI), 1 };
			ctD = CoordsTextura{ (deltaAz*(i + 1)) / (float)(2.0 * M_PI), 0 };

			if (orientacao == CIMA || orientacao == AMBOS) {
				pontos.push_back(A);
				normais.push_back(up);
				textCoords.push_back(ctA);

				pontos.push_back(B);
				normais.push_back(up);
				textCoords.push_back(ctB);

				pontos.push_back(C);
				normais.push_back(up);
				textCoords.push_back(ctC);

				pontos.push_back(C);
				normais.push_back(up);
				textCoords.push_back(ctC);

				pontos.push_back(B);
				normais.push_back(up);
				textCoords.push_back(ctB);

				pontos.push_back(D);
				normais.push_back(up);
				textCoords.push_back(ctD);
			}
			

			if (orientacao == BAIXO || orientacao == AMBOS) {
				pontos.push_back(A);
				normais.push_back(down);
				textCoords.push_back(ctA);

				pontos.push_back(C);
				normais.push_back(down);
				textCoords.push_back(ctC);

				pontos.push_back(B);
				normais.push_back(down);
				textCoords.push_back(ctB);

				pontos.push_back(C);
				normais.push_back(down);
				textCoords.push_back(ctC);

				pontos.push_back(D);
				normais.push_back(down);
				textCoords.push_back(ctD);

				pontos.push_back(B);
				normais.push_back(down);
				textCoords.push_back(ctB);
			}
		}
		return *this;

	}

	Figura& geraCirculo(Coordenadas3D o,
							float raio, int fatias,
							ORIENTACAO_FIG orientacao) {

		float deltaAz = (float) (2.0f * M_PI) / fatias;
		Coordenadas3D A, B;
		CoordsTextura ctA, ctB, ctO;
		Coordenadas3D up = Coordenadas3D{ 0,1,0 };
		Coordenadas3D down = Coordenadas3D{ 0,-1,0 };

		for (int i = 0; i < fatias; ++i) {
			A = CoordsPolares(o, raio, deltaAz*i).toCartesianas();
			float s1 = 0.5f + 0.5*cos((deltaAz*i));
			float t1 = 0.5f + 0.5*sin((deltaAz*i));

			ctA = CoordsTextura{ s1,t1 };

			B = CoordsPolares(o, raio, deltaAz*(i + 1)).toCartesianas();
			
			float s2 = 0.5f + 0.5f*cos((deltaAz*(i + 1)));
			float t2 = 0.5f + 0.5f*sin((deltaAz*(i + 1)));
			ctB = CoordsTextura{s2,t2};

			ctO = CoordsTextura{ 0.5f,0.5f };

			if (orientacao == CIMA || orientacao == AMBOS) {
				pontos.push_back(A);
				normais.push_back(up);
				textCoords.push_back(ctA);

				pontos.push_back(B);
				normais.push_back(up);
				textCoords.push_back(ctB);

				pontos.push_back(o);
				normais.push_back(up);
				textCoords.push_back(ctO);
			}

			if (orientacao == BAIXO || orientacao == AMBOS) {
				pontos.push_back(A);
				normais.push_back(down);
				textCoords.push_back(ctA);

				pontos.push_back(o);
				normais.push_back(down);
				textCoords.push_back(ctO);

				pontos.push_back(B);
				normais.push_back(down);
				textCoords.push_back(ctB);
			}
		}
		return *this;
	}

	Figura& geraCone(Coordenadas3D o, float raio, int altura, int fatias, int camadas) {
		float deltaAz = (float)2 * M_PI / fatias;
		float deltaAltura = (float)altura / camadas;
		float deltaRaio = (float)raio / camadas;
		float s1, t1, s2, t2;
		Coordenadas3D A, B, C, D;
		CoordsTextura ctA, ctB, ctC, ctD;

		Coordenadas3D down = Coordenadas3D{ 0,-1,0 };

		CoordsTextura ctOrigem = CoordsTextura{0.5f, 0.1875f};
		float raioTex = 0.1875;

		// Circulo de baixo
		for (int i = 0; i < fatias; ++i) {
			A = CoordsPolares(o, raio, deltaAz*i).toCartesianas();
			float s1 = ctOrigem.s + raioTex*cos((deltaAz*i));
			float t1 = ctOrigem.t + raioTex*sin((deltaAz*i));

			ctA = CoordsTextura{ s1,t1 };

			B = CoordsPolares(o, raio, deltaAz*(i + 1)).toCartesianas();
			
			float s2 = ctOrigem.s + raioTex*cos((deltaAz*(i+1)));
			float t2 = ctOrigem.t + raioTex*sin((deltaAz*(i+1)));
			ctB = CoordsTextura{s2,t2};

			pontos.push_back(A);
			normais.push_back(down);
			textCoords.push_back(ctA);

			pontos.push_back(o);
			normais.push_back(down);
			textCoords.push_back(ctOrigem);

			pontos.push_back(B);
			normais.push_back(down);
			textCoords.push_back(ctB);

			
		}

		// Superficie lateral
		float alfaTex = (float) 2 * M_PI * 0.1875f / (1 - 0.375f);
		float alfaTexInic = (float)3 * M_PI / 2.0f - alfaTex / 2.0f;
		float deltaAlfaTex = (float)alfaTex / fatias;
		float deltaRaioTex = (float)(1 - 0.375f) / camadas;

		for (int j = 0; j < camadas; ++j) {
			Coordenadas3D cBaixo = { o.x, o.y + deltaAltura*(j) , o.z };
			Coordenadas3D cAlto = { o.x, o.y + deltaAltura*(j+1) , o.z };
			for (int i = 0; i < fatias; ++i) {
				A = CoordsPolares(Coordenadas3D{ o.x , o.y + deltaAltura*j, o.z },
					raio - (deltaRaio*j),
					deltaAz*i).toCartesianas();
				B = CoordsPolares(Coordenadas3D{ o.x , o.y + deltaAltura*j, o.z },
					raio - (deltaRaio*j),
					deltaAz*(i + 1)).toCartesianas();
				C = CoordsPolares(Coordenadas3D{ o.x , o.y + deltaAltura*(j + 1), o.z },
					raio - (deltaRaio*(j + 1)),
					deltaAz*(i + 1)).toCartesianas();
				D = CoordsPolares(Coordenadas3D{ o.x , o.y + deltaAltura*(j + 1), o.z },
					raio - (deltaRaio*(j + 1)),
					deltaAz*i).toCartesianas();

				Coordenadas3D na = A - cBaixo;
				na.y = (float) raio / altura;
				Coordenadas3D nb = B - cBaixo;
				nb.y = (float) raio / altura;
				Coordenadas3D nc = C - cAlto;
				nc.y = (float) raio / altura;
				Coordenadas3D nd = D - cAlto;
				nd.y = (float) raio / altura;

				CoordsTextura at = CoordsTextura{(0.625f - deltaRaioTex * j) * cos(alfaTexInic + deltaAlfaTex * i) + 0.5f,
												(0.625f - deltaRaioTex * j) * sin(alfaTexInic + deltaAlfaTex * i) + 1.0f};
				CoordsTextura bt = CoordsTextura{(0.625f - deltaRaioTex * j) * cos(alfaTexInic + deltaAlfaTex * (i+1)) + 0.5f,
												(0.625f - deltaRaioTex * j) * sin(alfaTexInic + deltaAlfaTex * (i+1)) + 1.0f};
				CoordsTextura ct = CoordsTextura{(0.625f - deltaRaioTex * (j+1)) * cos(alfaTexInic + deltaAlfaTex * (i+1)) + 0.5f,
												(0.625f - deltaRaioTex * (j+1)) * sin(alfaTexInic + deltaAlfaTex * (i+1)) + 1.0f};
				CoordsTextura dt = CoordsTextura{(0.625f - deltaRaioTex * (j+1)) * cos(alfaTexInic + deltaAlfaTex * i) + 0.5f,
												(0.625f - deltaRaioTex * (j+1)) * sin(alfaTexInic + deltaAlfaTex * i) + 1.0f};
				
				pontos.push_back(A);
				normais.push_back(na.normalize());
				textCoords.push_back(at);

				pontos.push_back(B);
				normais.push_back(nb.normalize());
				textCoords.push_back(bt);

				pontos.push_back(C);
				normais.push_back(nc.normalize());
				textCoords.push_back(ct);

				pontos.push_back(A);
				normais.push_back(na.normalize());
				textCoords.push_back(at);

				pontos.push_back(C);
				normais.push_back(nc.normalize());
				textCoords.push_back(ct);

				pontos.push_back(D);
				normais.push_back(nd.normalize());
				textCoords.push_back(dt);
			}
		}
		return *this;
	}

	Figura& geraPlanoY(Coordenadas3D o, float comp, float larg, int divsx, int divsz, ORIENTACAO_FIG orientacao) {
		float x, z;
		float deltaComp = (float)comp / divsx;
		float deltaLarg = (float)larg / divsz;
		float deltaTexS = (float)1.0f / divsx;
		float deltaTexT = (float)1.0f / divsz;

		Coordenadas3D up = { 0,1,0 };
		Coordenadas3D down = { 0,-1,0 };

		for (int j = 0; j < divsz; ++j) {
			for (int i = 0; i < divsx; ++i) {
				x = -comp / 2 + deltaComp*i;
				z = larg / 2 - deltaLarg*j;
				Coordenadas3D a = { x + o.x            , 0 + o.y, z + o.z };
				Coordenadas3D b = { x + deltaComp + o.x, 0 + o.y, z + o.z };
				Coordenadas3D c = { x + deltaComp + o.x, 0 + o.y, z - deltaLarg + o.z };
				Coordenadas3D d = { x + o.x            , 0 + o.y, z - deltaLarg + o.z };

				if (orientacao == CIMA || orientacao == AMBOS) {
					pontos.push_back(a);
					normais.push_back(up);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});

					pontos.push_back(b);
					normais.push_back(up);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, j * deltaTexT});

					pontos.push_back(c);
					normais.push_back(up);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});

					pontos.push_back(a);
					normais.push_back(up);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});
					
					pontos.push_back(c);
					normais.push_back(up);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});

					pontos.push_back(d);
					normais.push_back(up);
					textCoords.push_back(CoordsTextura{i * deltaTexS, (j+1) * deltaTexT});
				}
				if (orientacao == BAIXO || orientacao == AMBOS) {
					pontos.push_back(a);
					normais.push_back(down);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});

					pontos.push_back(c);
					normais.push_back(down);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});
					
					pontos.push_back(b);
					normais.push_back(down);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, j * deltaTexT});

					pontos.push_back(a);
					normais.push_back(down);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});

					pontos.push_back(d);
					normais.push_back(down);
					textCoords.push_back(CoordsTextura{i * deltaTexS, (j+1) * deltaTexT});

					pontos.push_back(c);
					normais.push_back(down);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});
				}
			}
		}
		return *this;
	}

	Figura& geraPlanoX(Coordenadas3D o, float largura, float altura, int divsz, int divsy, ORIENTACAO_FIG orientacao) {
		float z, y;
		float deltaLargura = (float)largura / divsz;
		float deltaAltura = (float)altura / divsy;
		float deltaTexS = (float)1.0f / divsz;
		float deltaTexT = (float)1.0f / divsy;
		Coordenadas3D normal1 = Coordenadas3D{1, 0, 0};
		Coordenadas3D normal2 = Coordenadas3D{-1, 0, 0};

		for (int j = 0; j < divsy; ++j) {
			for (int i = 0; i < divsz; ++i) {
				z = largura / 2 - deltaLargura*i;
				y = -altura / 2 + deltaAltura*j;
				Coordenadas3D a = { 0 + o.x            , y + o.y, z + o.z };
				Coordenadas3D b = { 0 + o.x, y + o.y, z - deltaLargura + o.z };
				Coordenadas3D c = { 0 + o.x, y + deltaAltura + o.y, z - deltaLargura + o.z };
				Coordenadas3D d = { 0 + o.x            , y + deltaAltura + o.y, z + o.z };

				if (orientacao == CIMA || orientacao == AMBOS) {
					pontos.push_back(a);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});

					pontos.push_back(b);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, j * deltaTexT});

					pontos.push_back(c);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});

					pontos.push_back(a);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});
					
					pontos.push_back(c);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});

					pontos.push_back(d);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{i * deltaTexS, (j+1) * deltaTexT});
				}
				if (orientacao == BAIXO || orientacao == AMBOS) {
					pontos.push_back(a);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});

					pontos.push_back(c);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});
					
					pontos.push_back(b);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, j * deltaTexT});

					pontos.push_back(a);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});

					pontos.push_back(d);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{i * deltaTexS, (j+1) * deltaTexT});

					pontos.push_back(c);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});
				}
			}
		}
		return *this;
	}

	Figura& geraPlanoZ(Coordenadas3D o, float comp, float altura, int divsx, int divsy, ORIENTACAO_FIG orientacao) {
		float x, y;
		float deltaComp = (float)comp / divsx;
		float deltaAltura = (float)altura / divsy;
		float deltaTexS = (float)1.0f / divsx;
		float deltaTexT = (float)1.0f / divsy;
		Coordenadas3D normal1 = Coordenadas3D{0, 0, 1};
		Coordenadas3D normal2 = Coordenadas3D{0, 0, -1};

		for (int j = 0; j < divsy; ++j) {
			for (int i = 0; i < divsx; ++i) {
				x = -comp / 2 + deltaComp*i;
				y = -altura / 2 + deltaAltura*j;
				Coordenadas3D a = { x + o.x            , y + o.y, 0 + o.z };
				Coordenadas3D b = { x + deltaComp + o.x, y + o.y, 0 + o.z };
				Coordenadas3D c = { x + deltaComp + o.x, y + deltaAltura + o.y, 0 + o.z };
				Coordenadas3D d = { x + o.x            , y + deltaAltura + o.y, 0 + o.z };

				if (orientacao == CIMA || orientacao == AMBOS) {
					pontos.push_back(a);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});

					pontos.push_back(b);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, j * deltaTexT});

					pontos.push_back(c);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});

					pontos.push_back(a);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});
					
					pontos.push_back(c);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});

					pontos.push_back(d);
					normais.push_back(normal1);
					textCoords.push_back(CoordsTextura{i * deltaTexS, (j+1) * deltaTexT});
				}
				if (orientacao == BAIXO || orientacao == AMBOS) {
					pontos.push_back(a);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});

					pontos.push_back(c);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});
					
					pontos.push_back(b);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, j * deltaTexT});

					pontos.push_back(a);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{i * deltaTexS, j * deltaTexT});

					pontos.push_back(d);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{i * deltaTexS, (j+1) * deltaTexT});

					pontos.push_back(c);
					normais.push_back(normal2);
					textCoords.push_back(CoordsTextura{(i+1) * deltaTexS, (j+1) * deltaTexT});
				}
			}
		}
		return *this;
	}

	Figura& geraCaixa(Coordenadas3D o,
						float comprimento, float largura, float altura,
						int divsx, int divsz, int divsy) {
		Coordenadas3D centroPlano;
		float mAltura = (float)altura / 2;
		float mComprimento = (float)comprimento / 2;
		float mLargura = (float)largura / 2;

		// Plano B
		centroPlano = { o.x + 0.0f ,o.y + 0.0f, o.z + mLargura };
		geraPlanoZ(centroPlano, comprimento, altura, divsx, divsy, CIMA);

		// Plano B'
		centroPlano = { o.x + 0.0f ,o.y + 0.0f, o.z - mLargura };
		geraPlanoZ(centroPlano, comprimento, altura, divsx, divsy, BAIXO);

		// Plano C
		centroPlano = { o.x + mComprimento, o.y + 0.0f,o.z + 0.0f };
		geraPlanoX(centroPlano, largura, altura,  divsz, divsy, CIMA);

		// Plano C'
		centroPlano = { o.x - mComprimento, o.y + 0.0f,o.z + 0.0f };
		geraPlanoX(centroPlano, largura, altura, divsz, divsy, BAIXO);

		// Plano A
		centroPlano = { o.x + 0.0f, o.y + mAltura, o.z + 0.0f };
		geraPlanoY(centroPlano, comprimento,largura, divsx, divsz, CIMA);

		// Plano A'
		centroPlano = { o.x + 0.0f, o.y - mAltura, o.z + 0.0f };
		geraPlanoY(centroPlano, comprimento, largura, divsx, divsz, BAIXO);

		return *this;
	}

	Figura& geraCilindro(Coordenadas3D o, float raio, float altura, int fatias, int camadas) {

		float deltaAz = (float) (2.0f * M_PI) / fatias;
		float deltaAlt = (float) altura/camadas;
		Coordenadas3D A, B;
		CoordsTextura ctA, ctB;
		
		CoordsTextura ctOrigemCima = CoordsTextura{0.4375f, 0.1875f};
		CoordsTextura ctOrigemBaixo = CoordsTextura{0.8125f, 0.1875f};
		float raioTex = 0.1875;

		Coordenadas3D up = Coordenadas3D{ 0,1,0 };
		Coordenadas3D down = Coordenadas3D{ 0,-1,0 };

		Coordenadas3D oCima = Coordenadas3D{ o.x, o.y + altura, o.z };

		// Circulo superior
		for (int i = 0; i < fatias; ++i) {
			A = CoordsPolares(oCima, raio, deltaAz*i).toCartesianas();
			float s1 = ctOrigemCima.s + raioTex*cos((deltaAz*i));
			float t1 = ctOrigemCima.t + raioTex*sin((deltaAz*i));

			ctA = CoordsTextura{ s1,t1 };

			B = CoordsPolares(oCima, raio, deltaAz*(i + 1)).toCartesianas();
			
			float s2 = ctOrigemCima.s + raioTex*cos((deltaAz*(i+1)));
			float t2 = ctOrigemCima.t + raioTex*sin((deltaAz*(i+1)));
			ctB = CoordsTextura{s2,t2};

			pontos.push_back(A);
			normais.push_back(up);
			textCoords.push_back(ctA);

			pontos.push_back(B);
			normais.push_back(up);
			textCoords.push_back(ctB);

			pontos.push_back(oCima);
			normais.push_back(up);
			textCoords.push_back(ctOrigemCima);
		}

		// Círculo inferior
		for (int i = 0; i < fatias; ++i) {
			A = CoordsPolares(o, raio, deltaAz*i).toCartesianas();
			float s1 = ctOrigemBaixo.s + raioTex*cos((deltaAz*i));
			float t1 = ctOrigemBaixo.t + raioTex*sin((deltaAz*i));

			ctA = CoordsTextura{ s1,t1 };

			B = CoordsPolares(o, raio, deltaAz*(i + 1)).toCartesianas();
			
			float s2 = ctOrigemBaixo.s + raioTex*cos((deltaAz*(i+1)));
			float t2 = ctOrigemBaixo.t + raioTex*sin((deltaAz*(i+1)));
			ctB = CoordsTextura{s2,t2};

			pontos.push_back(A);
			normais.push_back(down);
			textCoords.push_back(ctA);

			pontos.push_back(o);
			normais.push_back(down);
			textCoords.push_back(ctOrigemBaixo);

			pontos.push_back(B);
			normais.push_back(down);
			textCoords.push_back(ctB);
		}

		// Corpo
		float deltaTexS = (float) 1.0f/fatias;
		float deltaTexT = (float) (1.0f - 0.375f)/camadas;
		for (int j = 0; j < camadas; ++j) {
			Coordenadas3D cBaixo = { o.x, o.y + deltaAlt*(j) , o.z };
			Coordenadas3D cAlto = { o.x, o.y + deltaAlt*(j+1) , o.z };
			for (int i = 0; i < fatias; i++) {
				CoordsPolares a = CoordsPolares(cBaixo, raio, deltaAz * i);
				CoordsPolares b = CoordsPolares(cBaixo, raio, deltaAz * (i+1));
				CoordsPolares c = CoordsPolares(cAlto, raio, deltaAz *  (i+1));
				CoordsPolares d = CoordsPolares(cAlto, raio, deltaAz * i);

				Coordenadas3D na = a.toCartesianas() - cBaixo;
				Coordenadas3D nb = b.toCartesianas() - cBaixo;
				Coordenadas3D nc = c.toCartesianas() - cAlto;
				Coordenadas3D nd = d.toCartesianas() - cAlto;

				pontos.push_back(a.toCartesianas());
				normais.push_back(na.normalize());
				textCoords.push_back(CoordsTextura{deltaTexS * i, deltaTexT * j + 0.375f});

				pontos.push_back(b.toCartesianas());
				normais.push_back(nb.normalize());
				textCoords.push_back(CoordsTextura{deltaTexS * (i+1), deltaTexT * j + 0.375f});

				pontos.push_back(c.toCartesianas());
				normais.push_back(nc.normalize());
				textCoords.push_back(CoordsTextura{deltaTexS * (i+1), deltaTexT * (j+1) + 0.375f});
				
				pontos.push_back(a.toCartesianas());
				normais.push_back(na.normalize());
				textCoords.push_back(CoordsTextura{deltaTexS * i, deltaTexT * j + 0.375f});
				
				pontos.push_back(c.toCartesianas());
				normais.push_back(nc.normalize());
				textCoords.push_back(CoordsTextura{deltaTexS * (i+1), deltaTexT * (j+1) + 0.375f});
				
				pontos.push_back(d.toCartesianas());
				normais.push_back(nd.normalize());
				textCoords.push_back(CoordsTextura{deltaTexS * i, deltaTexT * (j+1) + 0.375f});
			}
		}
		return *this;
	}

	Figura& geraTorus(Coordenadas3D o, float R, float r, int fatias, int camadas) {

		float deltaAz = (float)(2.0f * M_PI) / fatias;
		float deltaPolar = (float)(2.0f * M_PI) / camadas;


		for (int j = 0; j < fatias; ++j) {
			for (int i = 0; i < camadas; ++i) {
				
				float ro = deltaAz*j;
				float t = deltaPolar*i;
				float ro2 = deltaAz*(j+1);
				float t2 = deltaPolar*(i+1);
				Coordenadas3D a = {o.x + (R + r*cos(t))*sin(ro)  , o.y + r*sin(t) ,o.z + (R + r*cos(t))*cos(ro)};
				Coordenadas3D b = {o.x + (R + r*cos(t2))*sin(ro) , o.y + r*sin(t2),o.z + (R + r*cos(t2))*cos(ro) };
				Coordenadas3D c = {o.x + (R + r*cos(t2))*sin(ro2), o.y + r*sin(t2),o.z + (R + r*cos(t2))*cos(ro2) };
				Coordenadas3D d = {o.x + (R + r*cos(t))*sin(ro2) , o.y + r*sin(t) ,o.z + (R + r*cos(t))*cos(ro2) };

				pontos.push_back(a);
				pontos.push_back(b);
				pontos.push_back(c);
				pontos.push_back(a);
				pontos.push_back(c);
				pontos.push_back(d);
			}
		}
		
		return *this;
	}

	Figura& geraElipsoide(Coordenadas3D o, float a, float b, float c, int fatias, int camadas) {

		float deltaAz = (float)(2.0f * M_PI) / fatias;
		float deltaPolar = (float)(M_PI) / camadas;


		for (int j = 0; j < fatias; ++j) {
			for (int i = 0; i < camadas; ++i) {

				float ro = deltaAz*j;
				float t = deltaPolar*i;
				float ro2 = deltaAz*(j + 1);
				float t2 = deltaPolar*(i + 1);

				Coordenadas3D a_ = { o.x + b*cos(M_PI / 2 - t)*sin(ro), o.y + c*sin(M_PI / 2 - t) ,o.z + a*cos(M_PI / 2 - t)*cos(ro) };
				Coordenadas3D c_ = { o.x + b*cos(M_PI / 2 - t2)*sin(ro), o.y + c*sin(M_PI / 2 - t2) ,o.z + a*cos(M_PI / 2 - t2)*cos(ro) };
				Coordenadas3D b_ = { o.x + b*cos(M_PI / 2 - t)*sin(ro2), o.y + c*sin(M_PI / 2 - t) ,o.z + a*cos(M_PI / 2 - t)*cos(ro2) };
				Coordenadas3D d_ = { o.x + b*cos(M_PI / 2 - t2)*sin(ro2) , o.y + c*sin(M_PI / 2 - t2) ,o.z + a*cos(M_PI / 2 - t2)*cos(ro2) };

				pontos.push_back(a_);
				pontos.push_back(c_);
				pontos.push_back(b_);
				pontos.push_back(b_);
				pontos.push_back(c_);
				pontos.push_back(d_);
			}
		}

		return *this;
	}

	Figura& geraFitaMobius(Coordenadas3D o, float R, float largura, float divsl, float divscomp) {

		float deltaU = (float)(2.0f * M_PI) / divscomp;
		float deltaL = (float)largura / divsl;
		float mLargura = (float) largura / 2;

		for (int j = 0; j < divsl; ++j) {
			for (int i = 0; i < divscomp; ++i) {

				float u = deltaU*i;
				float v = -mLargura + deltaL*j;
				float u2 = deltaU*(i+1);
				float v2 = -mLargura + deltaL*(j+1);
				Coordenadas3D a = { o.x + (1+(v/2.0)*cos(u/2.0))*sin(u)   , o.y + (v/2.0)*sin(u/2.0)   ,o.z + (1+(v/2.0)*cos(u/2.0))*cos(u)};
				Coordenadas3D b = { o.x + (1+(v2/2.0)*cos(u/2.0))*sin(u)  , o.y + (v2/2.0)*sin(u/2.0)  ,o.z + (1+(v2/2.0)*cos(u/2.0))*cos(u)};
				Coordenadas3D c = { o.x + (1+(v2/2.0)*cos(u2/2.0))*sin(u2), o.y + (v2/2.0)*sin(u2/2.0) ,o.z + (1+(v2/2.0)*cos(u2/2.0))*cos(u2)};
				Coordenadas3D d = { o.x + (1+(v/2.0)*cos(u2/2.0))*sin(u2) , o.y + (v/2.0)*sin(u2/2.0)  ,o.z + (1+(v/2.0)*cos(u2/2.0))*cos(u2)};

				pontos.push_back(a);
				pontos.push_back(b);
				pontos.push_back(d);
				pontos.push_back(b);
				pontos.push_back(c);
				pontos.push_back(d);

				pontos.push_back(a);
				pontos.push_back(d);
				pontos.push_back(b);
				pontos.push_back(b);
				pontos.push_back(d);
				pontos.push_back(c);
			}
		}

		return *this;
	}

	Figura& geraSeashell(Coordenadas3D o, float p_a, float p_b, float p_c, int n, float divsU, float divsV) {

		float deltaU = (float)(2.0f * M_PI) / divsU;
		float deltaV = (float)(2.0f * M_PI) / divsV;

		for (int j = 0; j < divsU; ++j) {
			for (int i = 0; i < divsV; ++i) {

				float u = deltaU*i;
				float v = deltaV*j;
				float u2 = deltaU*(i + 1);
				float v2 = deltaV*(j+1);
				Coordenadas3D a = {   o.x + ((1 - (v / (2 * M_PI))) * (1 + cos(u)) + p_c)*sin(n*v),
								o.y + ((p_b*v)/(2*M_PI)) + p_a*sin(u)*(1-(v/(2*M_PI))),
								o.z + ((1-(v/(2*M_PI))) * (1 + cos(u)) + p_c)*cos(n*v) };

				Coordenadas3D b = { o.x + ((1 - (v2 / (2 * M_PI))) * (1 + cos(u)) + p_c)*sin(n*v2),
								o.y + ((p_b*v2) / (2 * M_PI)) + p_a*sin(u)*(1 - (v2 / (2 * M_PI))),
								o.z + ((1 - (v2 / (2 * M_PI))) * (1 + cos(u)) + p_c)*cos(n*v2) };

				Coordenadas3D c = { o.x + ((1 - (v2 / (2 * M_PI))) * (1 + cos(u2)) + p_c)*sin(n*v2),
								o.y + ((p_b*v2) / (2 * M_PI)) + p_a*sin(u2)*(1 - (v2 / (2 * M_PI))),
								o.z + ((1 - (v2 / (2 * M_PI))) * (1 + cos(u2)) + p_c)*cos(n*v2) };

				Coordenadas3D d = { o.x + ((1 - (v / (2 * M_PI))) * (1 + cos(u2)) + p_c)*sin(n*v),
								o.y + ((p_b*v) / (2 * M_PI)) + p_a*sin(u2)*(1 - (v / (2 * M_PI))),
								o.z + ((1 - (v / (2 * M_PI))) * (1 + cos(u2)) + p_c)*cos(n*v) };

				pontos.push_back(a);
				pontos.push_back(b);
				pontos.push_back(c);
				pontos.push_back(a);
				pontos.push_back(c);
				pontos.push_back(d);

				pontos.push_back(a);
				pontos.push_back(d);
				pontos.push_back(c);
				pontos.push_back(a);
				pontos.push_back(c);
				pontos.push_back(b);
			}
		}

		return *this;
	}

	std::vector<Coordenadas3D> getPontos() {
		return pontos;
	}

	std::vector<Coordenadas3D> getNormais() {
		return normais;
	}


	std::vector<CoordsTextura> getTextCoords() {
		return textCoords;
	}


private:
	std::vector<Coordenadas3D> pontos;
	std::vector<Coordenadas3D> normais;
	std::vector<CoordsTextura> textCoords;

};
