#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <fstream>
#include "Ponto3D.h"
#include "CoordsPolares.h"
#include "CoordsEsfericas.h"

using namespace std;

class Figura {
public:
	
	Figura& geraEsfera(Ponto3D o,float raio, int fatias, int camadas) {

		float deltaAz = (float)2 * M_PI / fatias;
		float deltaPolar = (float)M_PI / camadas;

		for (int j = 0; j < camadas; ++j) {
			for (int i = 0; i < fatias; ++i) {
				Ponto3D a = CoordsEsfericas(raio, deltaAz*i, deltaPolar*j).toCartesianas() + o;
				Ponto3D b = CoordsEsfericas(raio, deltaAz*(i + 1), deltaPolar*j).toCartesianas() + o;
				Ponto3D c = CoordsEsfericas(raio, deltaAz*i, deltaPolar*(j + 1)).toCartesianas() + o;
				Ponto3D d = CoordsEsfericas(raio, deltaAz*(i + 1), deltaPolar*(j + 1)).toCartesianas() + o;

				pontos.push_back(a);
				pontos.push_back(c);
				pontos.push_back(d);
				pontos.push_back(a);
				pontos.push_back(d);
				pontos.push_back(b);
			}
		}

		return *this;
	}

	Figura& geraCirculo(Ponto3D o, float raio, int fatias, int orientacao) {
		float deltaAz = (float)2 * M_PI / fatias;

		for (int i = 0; i < fatias; ++i) {
			CoordsPolares a = CoordsPolares(o, raio, deltaAz*i);
			CoordsPolares b = CoordsPolares(o, raio, deltaAz*(i + 1));

			if (orientacao == 1) {
				pontos.push_back(a.toCartesianas());
				pontos.push_back(b.toCartesianas());
				pontos.push_back(o);
			}
			else {
				pontos.push_back(a.toCartesianas());
				pontos.push_back(o);
				pontos.push_back(b.toCartesianas());
			}
		}
		return *this;
	}

	Figura& geraCone(Ponto3D o, float raio, int altura, int fatias, int camadas) {
		float deltaAz = (float)2 * M_PI / fatias;
		float deltaAltura = (float)altura / camadas;
		float deltaRaio = (float)raio / camadas;

		geraCirculo(o, raio, fatias, 0);

		for (int j = 0; j < camadas; ++j) {
			for (int i = 0; i < fatias; ++i) {
				CoordsPolares a = CoordsPolares(Ponto3D{ o.x , o.y + deltaAltura*j, o.z },
					raio - (deltaRaio*j),
					deltaAz*i);
				CoordsPolares b = CoordsPolares(Ponto3D{ o.x , o.y + deltaAltura*j, o.z },
					raio - (deltaRaio*j),
					deltaAz*(i + 1));
				CoordsPolares c = CoordsPolares(Ponto3D{ o.x , o.y + deltaAltura*(j + 1), o.z },
					raio - (deltaRaio*(j + 1)),
					deltaAz*(i + 1));
				CoordsPolares d = CoordsPolares(Ponto3D{ o.x , o.y + deltaAltura*(j + 1), o.z },
					raio - (deltaRaio*(j + 1)),
					deltaAz*i);

				pontos.push_back(a.toCartesianas());
				pontos.push_back(b.toCartesianas());
				pontos.push_back(c.toCartesianas());
				pontos.push_back(a.toCartesianas());
				pontos.push_back(c.toCartesianas());
				pontos.push_back(d.toCartesianas());
			}
		}
		return *this;
	}

	Figura& geraPlanoY(Ponto3D o, float comp, float larg, int divsx, int divsz, int orientacao) {
		float x, z;
		float deltaComp = (float)comp / divsx;
		float deltaLarg = (float)larg / divsz;

		for (int j = 0; j < divsz; ++j) {
			for (int i = 0; i < divsx; ++i) {
				x = -comp / 2 + deltaComp*i;
				z = larg / 2 - deltaLarg*j;
				Ponto3D a = { x + o.x            , 0 + o.y, z + o.z };
				Ponto3D b = { x + deltaComp + o.x, 0 + o.y, z + o.z };
				Ponto3D c = { x + deltaComp + o.x, 0 + o.y, z - deltaLarg + o.z };
				Ponto3D d = { x + o.x            , 0 + o.y, z - deltaLarg + o.z };

				if (orientacao == 1) {
					pontos.push_back(a);
					pontos.push_back(b);
					pontos.push_back(c);
					pontos.push_back(a);
					pontos.push_back(c);
					pontos.push_back(d);
				}
				else {
					pontos.push_back(a);
					pontos.push_back(c);
					pontos.push_back(b);
					pontos.push_back(a);
					pontos.push_back(d);
					pontos.push_back(c);
				}
			}
		}
		return *this;
	}

	Figura& geraPlanoX(Ponto3D o, float largura, float altura, int divsz, int divsy, int orientacao) {
		float z, y;
		float deltaLargura = (float)largura / divsz;
		float deltaAltura = (float)altura / divsy;

		for (int j = 0; j < divsy; ++j) {
			for (int i = 0; i < divsz; ++i) {
				z = largura / 2 - deltaLargura*i;
				y = -altura / 2 + deltaAltura*j;
				Ponto3D a = { 0 + o.x            , y + o.y, z + o.z };
				Ponto3D b = { 0 + o.x, y + o.y, z - deltaLargura + o.z };
				Ponto3D c = { 0 + o.x, y + deltaAltura + o.y, z - deltaLargura + o.z };
				Ponto3D d = { 0 + o.x            , y + deltaAltura + o.y, z + o.z };

				if (orientacao == 1) {
					pontos.push_back(a);
					pontos.push_back(b);
					pontos.push_back(c);
					pontos.push_back(a);
					pontos.push_back(c);
					pontos.push_back(d);
				}
				else {
					pontos.push_back(a);
					pontos.push_back(c);
					pontos.push_back(b);
					pontos.push_back(a);
					pontos.push_back(d);
					pontos.push_back(c);
				}
			}
		}
		return *this;
	}

	Figura& geraPlanoZ(Ponto3D o, float comp, float altura, int divsx, int divsy, int orientacao) {
		float x, y;
		float deltaComp = (float)comp / divsx;
		float deltaAltura = (float)altura / divsy;

		for (int j = 0; j < divsy; ++j) {
			for (int i = 0; i < divsx; ++i) {
				x = -comp / 2 + deltaComp*i;
				y = -altura / 2 + deltaAltura*j;
				Ponto3D a = { x + o.x            , y + o.y, 0 + o.z };
				Ponto3D b = { x + deltaComp + o.x, y + o.y, 0 + o.z };
				Ponto3D c = { x + deltaComp + o.x, y + deltaAltura + o.y, 0 + o.z };
				Ponto3D d = { x + o.x            , y + deltaAltura + o.y, 0 + o.z };

				if (orientacao == 1) {
					pontos.push_back(a);
					pontos.push_back(b);
					pontos.push_back(c);
					pontos.push_back(a);
					pontos.push_back(c);
					pontos.push_back(d);
				}
				else {
					pontos.push_back(a);
					pontos.push_back(c);
					pontos.push_back(b);
					pontos.push_back(a);
					pontos.push_back(d);
					pontos.push_back(c);
				}
			}
		}
		return *this;
	}

	Figura& geraCaixa(Ponto3D o,
						float comprimento, float largura, float altura,
						int divsx, int divsz, int divsy) {
		Ponto3D centroPlano;
		float mAltura = (float)altura / 2;
		float mComprimento = (float)comprimento / 2;
		float mLargura = (float)largura / 2;

		// Plano B
		centroPlano = { o.x + 0.0f ,o.y + 0.0f, o.z + mLargura };
		geraPlanoZ(centroPlano, comprimento, altura, divsx, divsy, 1);

		// Plano B'
		centroPlano = { o.x + 0.0f ,o.y + 0.0f, o.z - mLargura };
		geraPlanoZ(centroPlano, comprimento, altura, divsx, divsy, 0);

		// Plano C
		centroPlano = { o.x + mComprimento, o.y + 0.0f,o.z + 0.0f };
		geraPlanoX(centroPlano, largura, altura,  divsz, divsy,1);

		// Plano C'
		centroPlano = { o.x - mComprimento, o.y + 0.0f,o.z + 0.0f };
		geraPlanoX(centroPlano, largura, altura, divsz, divsy, 0);

		// Plano A
		centroPlano = { o.x + 0.0f, o.y + mAltura, o.z + 0.0f };
		geraPlanoY(centroPlano, comprimento,largura, divsx, divsz, 1);

		// Plano A'
		centroPlano = { o.x + 0.0f, o.y - mAltura, o.z + 0.0f };
		geraPlanoY(centroPlano, comprimento, largura, divsx, divsz, 0);


		return *this;
	}

	Figura& geraCilindro(Ponto3D o, float raio, float altura, int fatias, int camadas) {

		float deltaAz = (float) (2.0f * M_PI) / fatias;
		float deltaAlt = (float) altura/camadas;

		geraCirculo(o, raio, fatias,0);
		geraCirculo(Ponto3D{o.x,o.y + altura,o.z}, raio, fatias, 1);

		for (int j = 0; j < camadas; ++j) {
			Ponto3D cBaixo = { o.x, o.y + deltaAlt*(j) , o.z };
			Ponto3D cAlto = { o.x, o.y + deltaAlt*(j+1) , o.z };
			for (int i = 0; i < fatias; i++) {
				CoordsPolares a = CoordsPolares(cBaixo, raio, deltaAz * i);
				CoordsPolares b = CoordsPolares(cBaixo, raio, deltaAz * (i+1));
				CoordsPolares c = CoordsPolares(cAlto, raio, deltaAz *  (i+1));
				CoordsPolares d = CoordsPolares(cAlto, raio, deltaAz * i);

				pontos.push_back(a.toCartesianas());
				pontos.push_back(b.toCartesianas());
				pontos.push_back(c.toCartesianas());
				pontos.push_back(a.toCartesianas());
				pontos.push_back(c.toCartesianas());
				pontos.push_back(d.toCartesianas());
			}
		}
		return *this;
	}

	Figura& geraTorus(Ponto3D o, float R, float r, int fatias, int camadas) {

		float deltaAz = (float)(2.0f * M_PI) / fatias;
		float deltaPolar = (float)(2.0f * M_PI) / camadas;


		for (int j = 0; j < fatias; ++j) {
			for (int i = 0; i < camadas; ++i) {
				
				float ro = deltaAz*j;
				float t = deltaPolar*i;
				float ro2 = deltaAz*(j+1);
				float t2 = deltaPolar*(i+1);
				Ponto3D a = {o.x + (R + r*cos(t))*sin(ro)  , o.y + r*sin(t) ,o.z + (R + r*cos(t))*cos(ro)};
				Ponto3D b = {o.x + (R + r*cos(t2))*sin(ro) , o.y + r*sin(t2),o.z + (R + r*cos(t2))*cos(ro) };
				Ponto3D c = {o.x + (R + r*cos(t2))*sin(ro2), o.y + r*sin(t2),o.z + (R + r*cos(t2))*cos(ro2) };
				Ponto3D d = {o.x + (R + r*cos(t))*sin(ro2) , o.y + r*sin(t) ,o.z + (R + r*cos(t))*cos(ro2) };

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

	Figura& geraElipsoide(Ponto3D o, float a, float b, float c, int fatias, int camadas) {

		float deltaAz = (float)(2.0f * M_PI) / fatias;
		float deltaPolar = (float)(M_PI) / camadas;


		for (int j = 0; j < fatias; ++j) {
			for (int i = 0; i < camadas; ++i) {

				float ro = deltaAz*j;
				float t = deltaPolar*i;
				float ro2 = deltaAz*(j + 1);
				float t2 = deltaPolar*(i + 1);

				Ponto3D a_ = { o.x + b*cos(M_PI / 2 - t)*sin(ro), o.y + c*sin(M_PI / 2 - t) ,o.z + a*cos(M_PI / 2 - t)*cos(ro) };
				Ponto3D c_ = { o.x + b*cos(M_PI / 2 - t2)*sin(ro), o.y + c*sin(M_PI / 2 - t2) ,o.z + a*cos(M_PI / 2 - t2)*cos(ro) };
				Ponto3D b_ = { o.x + b*cos(M_PI / 2 - t)*sin(ro2), o.y + c*sin(M_PI / 2 - t) ,o.z + a*cos(M_PI / 2 - t)*cos(ro2) };
				Ponto3D d_ = { o.x + b*cos(M_PI / 2 - t2)*sin(ro2) , o.y + c*sin(M_PI / 2 - t2) ,o.z + a*cos(M_PI / 2 - t2)*cos(ro2) };

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

	Figura& geraFitaMobius(Ponto3D o, float R, float largura, float divsl, float divscomp) {

		float deltaU = (float)(2.0f * M_PI) / divscomp;
		float deltaV = (float)largura / divsl;
		float mLargura = (float) largura / 2;

		for (int j = 0; j < divsl; ++j) {
			for (int i = 0; i < divscomp; ++i) {

				float u = deltaU*i;
				float v = -mLargura + deltaV*j;
				float u2 = deltaU*(i+1);
				float v2 = -mLargura + deltaV*(j+1);
				Ponto3D a = { o.x + (1+(v/2.0)*cos(u/2.0))*sin(u)   , o.y + (v/2.0)*sin(u/2.0)   ,o.z + (1+(v/2.0)*cos(u/2.0))*cos(u)};
				Ponto3D b = { o.x + (1+(v2/2.0)*cos(u/2.0))*sin(u)  , o.y + (v2/2.0)*sin(u/2.0)  ,o.z + (1+(v2/2.0)*cos(u/2.0))*cos(u)};
				Ponto3D c = { o.x + (1+(v2/2.0)*cos(u2/2.0))*sin(u2), o.y + (v2/2.0)*sin(u2/2.0) ,o.z + (1+(v2/2.0)*cos(u2/2.0))*cos(u2)};
				Ponto3D d = { o.x + (1+(v/2.0)*cos(u2/2.0))*sin(u2) , o.y + (v/2.0)*sin(u2/2.0)  ,o.z + (1+(v/2.0)*cos(u2/2.0))*cos(u2)};

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

	Figura& geraSeashell(Ponto3D o, float p_a, float p_b, float p_c, int n, float divsU, float divsV) {

		float deltaU = (float)(2.0f * M_PI) / divsU;
		float deltaV = (float)(2.0f * M_PI) / divsV;

		for (int j = 0; j < divsU; ++j) {
			for (int i = 0; i < divsV; ++i) {

				float u = deltaU*i;
				float v = deltaV*j;
				float u2 = deltaU*(i + 1);
				float v2 = deltaV*(j+1);
				Ponto3D a = {   o.x + ((1 - (v / (2 * M_PI))) * (1 + cos(u)) + p_c)*sin(n*v),
								o.y + ((p_b*v)/(2*M_PI)) + p_a*sin(u)*(1-(v/(2*M_PI))),
								o.z + ((1-(v/(2*M_PI))) * (1 + cos(u)) + p_c)*cos(n*v) };

				Ponto3D b = { o.x + ((1 - (v2 / (2 * M_PI))) * (1 + cos(u)) + p_c)*sin(n*v2),
								o.y + ((p_b*v2) / (2 * M_PI)) + p_a*sin(u)*(1 - (v2 / (2 * M_PI))),
								o.z + ((1 - (v2 / (2 * M_PI))) * (1 + cos(u)) + p_c)*cos(n*v2) };

				Ponto3D c = { o.x + ((1 - (v2 / (2 * M_PI))) * (1 + cos(u2)) + p_c)*sin(n*v2),
								o.y + ((p_b*v2) / (2 * M_PI)) + p_a*sin(u2)*(1 - (v2 / (2 * M_PI))),
								o.z + ((1 - (v2 / (2 * M_PI))) * (1 + cos(u2)) + p_c)*cos(n*v2) };

				Ponto3D d = { o.x + ((1 - (v / (2 * M_PI))) * (1 + cos(u2)) + p_c)*sin(n*v),
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

	std::vector<Ponto3D> getPontos() {
		return pontos;
	}

private:
	std::vector<Ponto3D> pontos;

};
