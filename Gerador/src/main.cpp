#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../../utils/Ponto3D.h"
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
		int divsx = atoi(argv[4])+1;
		int divsz = atoi(argv[5]) + 1;
		nome_fich = argv[6];

		figura.geraPlanoY(Ponto3D{0.0,0.0,0.0},comp, largura, divsx, divsz, 1);
	}
	if ((str_figura == "circle" || str_figura == "circulo") && argc == 5) {
		cmd = true;
		//     0     1      2    3    
		// gerador circle raio fatias
		float raio = atof(argv[2]);
		int fatias = atoi(argv[3]);
		nome_fich = argv[4];

		figura.geraCirculo(Ponto3D{ 0.0,0.0,0.0 }, raio,fatias, 1);
	}
	if ((str_figura == "box" || str_figura == "caixa") && argc == 9) {
		cmd = true;
		//     0     1         2        3      4     5      6     7
		// gerador box comprimento largura altura divsx divsz divsy
		float comp = atof(argv[2]);
		float largura = atof(argv[3]);
		float altura = atof(argv[4]);
		int divsx = atoi(argv[5]) + 1;
		int divsz = atoi(argv[6]) + 1;
		int divsy = atoi(argv[7]);
		nome_fich = argv[8];

		figura.geraCaixa(Ponto3D{ 0.0,0.0,0.0 }, comp, largura, altura, divsx, divsz, divsy);
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

		figura.geraCone(Ponto3D{ 0.0,0.0,0.0 }, raio, altura,fatias,camadas);
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

		figura.geraCilindro(Ponto3D{ 0.0,0.0,0.0 }, raio, altura, fatias, camadas);
	}

	if ((str_figura == "esfera" || str_figura == "sphere") && argc == 6) {
		cmd = true;
		//     0     1     2      3     4
		// gerador esfera raio fatias camdas
		float raio = atof(argv[2]);
		int fatias = atoi(argv[3]);
		int camadas = atoi(argv[4]);
		nome_fich = argv[5];

		figura.geraEsfera(Ponto3D{0,0,0},raio, fatias, camadas);
		//figura.geraEsfera(Ponto3D{0,5,0}, raio, fatias, camadas);
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

		figura.geraTorus(Ponto3D{ 0.0,0.0,0.0 }, R, r, fatias, camadas);
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

		figura.geraElipsoide(Ponto3D{0,0,0},a,b,c,fatias,camadas);
	}

	//Ponto3D o, float R, float largura, float divsl, float divscomp

	if (str_figura == "mobius" && argc == 7) {
		cmd = true;
		//     0     1      2     3      4      5
		// gerador mobius raio largura divsl divscomp
		float raio = atof(argv[2]);
		float largura = atof(argv[3]);
		int divsl = atof(argv[4]);
		int divscomp = atof(argv[5]);
		nome_fich = argv[6];

		figura.geraFitaMobius(Ponto3D{ 0,0,0 }, raio, largura, divsl, divscomp);
	}

	if (!cmd) std::cout << "Comando invalido! :(" << std::endl;

	std::ofstream ficheiro(modelos_path + nome_fich);
	for (Ponto3D ponto : figura.getPontos()) {
		ficheiro << ponto.x << " " << ponto.y << " " << ponto.z << std::endl;
	}

	std::ofstream ficheiro_xml(modelos_path + "figura.xml");
	ficheiro_xml << "<scene>"							 << std::endl <<
					"\t<model file=\"" << nome_fich << "\" />"	 << std::endl <<
					"</scene>"							 << std::endl;

}


