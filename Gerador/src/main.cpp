#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../../utils/Coordenadas3D.h"
#include "../../utils/CoordsPolares.h"
#include "../../utils/CoordsEsfericas.h"
#include "../../utils/Figura.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

int main(int argc, char** argv) {
	Figura figura;
	string nome_fich;
	string modelos_path("../../../../Motor/Modelos/");
	string str_figura = argv[1];
	bool cmd = false;

	if ((str_figura == "plane" || str_figura == "plano") && argc == 7) {
		cmd = true;
		//     0     1         2        3      4     5
		// gerador plane comprimento largura divsx divsz
		float comp = atof(argv[2]);
		float largura = atof(argv[3]);
		int divsx = atoi(argv[4]);
		int divsz = atoi(argv[5]);
		nome_fich = argv[6];

		figura.geraPlanoY(Coordenadas3D{0.0,0.0,0.0},comp, largura, divsx, divsz, 1);
	}
	if ((str_figura == "circle" || str_figura == "circulo") && argc == 5) {
		cmd = true;
		//     0     1      2    3    
		// gerador circle raio fatias
		float raio = atof(argv[2]);
		int fatias = atoi(argv[3]);
		nome_fich = argv[4];

		figura.geraCirculo(Coordenadas3D{ 0.0,0.0,0.0 }, raio,fatias, 1);
	}

	if ((str_figura == "ring" || str_figura == "anel") && argc == 6) {
		cmd = true;
		//     0     1   2  3   4  
		// gerador anel rI rE fatias
		float rI = atof(argv[2]);
		float rE = atof(argv[3]);
		int fatias = atoi(argv[4]);
		nome_fich = argv[5];

		figura.geraAnel(Coordenadas3D{ 0.0,0.0,0.0 }, rI,rE, fatias);
	}

	if ((str_figura == "box" || str_figura == "caixa") && argc == 9) {
		cmd = true;
		//     0     1         2        3      4     5      6     7
		// gerador box comprimento largura altura divsx divsz divsy
		float comp = atof(argv[2]);
		float largura = atof(argv[3]);
		float altura = atof(argv[4]);
		int divsx = atoi(argv[5]);
		int divsz = atoi(argv[6]);
		int divsy = atoi(argv[7]);
		nome_fich = argv[8];

		figura.geraCaixa(Coordenadas3D{ 0.0,0.0,0.0 }, comp, largura, altura, divsx, divsz, divsy);
	}

	if (str_figura == "cone" && argc == 7) {
		cmd = true;
		//     0     1   2      3      4     5
		// gerador cone raio altura fatias camadas
		float raio = atof(argv[2]);
		float altura = atof(argv[3]);
		int fatias = atoi(argv[4]);
		int camadas = atoi(argv[5]);
		nome_fich = argv[6];

		figura.geraCone(Coordenadas3D{ 0.0,0.0,0.0 }, raio, altura,fatias,camadas);
	}

	if ((str_figura == "cylinder" || str_figura == "cilindro") && argc == 7) {
		cmd = true;
		//     0     1        2     3      4     5
		// gerador cylinder raio altura fatias camadas
		float raio = atof(argv[2]);
		float altura = atof(argv[3]);
		int fatias = atoi(argv[4]);
		int camadas = atoi(argv[5]);
		nome_fich = argv[6];

		figura.geraCilindro(Coordenadas3D{ 0.0,0.0,0.0 }, raio, altura, fatias, camadas);
	}

	if ((str_figura == "esfera" || str_figura == "sphere") && argc == 6) {
		cmd = true;
		//     0     1     2      3     4
		// gerador esfera raio fatias camdas
		float raio = atof(argv[2]);
		int fatias = atoi(argv[3]);
		int camadas = atoi(argv[4]);
		nome_fich = argv[5];

		figura.geraEsfera(Coordenadas3D{0,0,0},raio, fatias, camadas);
		//figura.geraEsfera(Coordenadas3D{0,5,0}, raio, fatias, camadas);
	}

	if (str_figura == "torus" && argc == 7) {
		cmd = true;
		//     0     1    2      3      4     5
		// gerador torus raioE raioI fatias camadas
		float R = atof(argv[2]);
		float r = atof(argv[3]);
		int fatias = atoi(argv[4]);
		int camadas = atoi(argv[5]);
		nome_fich = argv[6];

		figura.geraTorus(Coordenadas3D{ 0.0,0.0,0.0 }, R, r, fatias, camadas);
	}

	if ((str_figura == "ellipsoid" || str_figura == "elipsoide") && argc == 8) {
		cmd = true;
		//     0     1       2 3 4   5      6
		// gerador ellipsoid a b c fatias camadas
		float a = atof(argv[2]);
		float b = atof(argv[3]);
		float c = atof(argv[4]);
		int fatias = atoi(argv[5]);
		int camadas = atoi(argv[6]);
		nome_fich = argv[7];

		figura.geraElipsoide(Coordenadas3D{0,0,0},a,b,c,fatias,camadas);
	}

	//Coordenadas3D o, float R, float largura, float divsl, float divscomp

	if (str_figura == "mobius" && argc == 7) {
		cmd = true;
		//     0     1      2     3      4      5
		// gerador mobius raio largura divsl divscomp
		float raio = atof(argv[2]);
		float largura = atof(argv[3]);
		int divsl = atoi(argv[4]);
		int divscomp = atoi(argv[5]);
		nome_fich = argv[6];

		figura.geraFitaMobius(Coordenadas3D{ 0,0,0 }, raio, largura, divsl, divscomp);
	}

	if (str_figura == "seashell" && argc == 9) {
		cmd = true;
		//     0     1      2 3 4 5   6     7
		// gerador seashell a b c n divsU divsV
		float a = atof(argv[2]);
		float b = atof(argv[3]);
		float c = atof(argv[4]);
		int n = atoi(argv[5]);
		int divsU = atoi(argv[6]);
		int divsV = atoi(argv[7]);
		nome_fich = argv[8];

		figura.geraSeashell(Coordenadas3D{ 0,0,0 }, a, b, c, n, divsU, divsV);
	}

	if (!cmd) {
		std::cout << "Comando invalido ou numero de argumentos invalido." << std::endl;
	}
	else {
		std::ofstream ficheiro(modelos_path + nome_fich);
		for (Coordenadas3D ponto : figura.getPontos()) {
			ficheiro << ponto.x << " " << ponto.y << " " << ponto.z << std::endl;
		}
	}
	
}


