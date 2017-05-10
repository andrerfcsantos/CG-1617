#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "../../utils/Coordenadas3D.h"
#include "../../utils/CoordsPolares.h"
#include "../../utils/CoordsEsfericas.h"
#include "../../utils/Figura.h"
#include "SuperficieBezier.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

SuperficieBezier leBezier(char *fich_in) {
	int nPatches, nPtsControlo;
	char *delims = ", \r\n";
	SuperficieBezier superficie;
	
	ifstream fich_inp(fich_in);
	string linha;

	std::getline(fich_inp, linha);
	nPatches = stoi(linha);
	
	for (int i = 0; i < nPatches;i++) {
		std::vector<int> patch;
		int indice;
		std::getline(fich_inp, linha);
		stringstream ss(linha);

		while (ss) {
			if (!std::getline(ss, linha, ',')) {
				break;
			}
			indice = stoi(linha);
			patch.push_back(indice);
		}

		superficie.addPatch(patch);
	}

	std::getline(fich_inp, linha);
	nPtsControlo = stoi(linha);

	for (int i = 0; i < nPtsControlo; i++) {

		float x,y,z;
		std::getline(fich_inp, linha);
		stringstream ss(linha);
		std::getline(ss, linha, ',');
		x = stof(linha);

		std::getline(ss, linha, ',');
		y = stof(linha);

		std::getline(ss, linha, ',');
		z = stof(linha);

		superficie.addPontoControlo(Coordenadas3D{ x,y,z });
	}


	return superficie;
}


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

		figura.geraPlanoY(Coordenadas3D{0.0,0.0,0.0},comp, largura, divsx, divsz, CIMA);
	}
	if ((str_figura == "circle" || str_figura == "circulo") && argc == 5) {
		cmd = true;
		//     0     1      2    3    
		// gerador circle raio fatias
		float raio = atof(argv[2]);
		int fatias = atoi(argv[3]);
		nome_fich = argv[4];

		figura.geraCirculo(Coordenadas3D{ 0.0,0.0,0.0 }, raio,fatias, CIMA);
	}

	if ((str_figura == "ring" || str_figura == "anel") && argc == 6) {
		cmd = true;
		//     0     1   2  3   4  
		// gerador anel rI rE fatias
		float rI = atof(argv[2]);
		float rE = atof(argv[3]);
		int fatias = atoi(argv[4]);
		nome_fich = argv[5];

		figura.geraAnel(Coordenadas3D{ 0.0,0.0,0.0 }, rI,rE, fatias,AMBOS);
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

	if (str_figura == "bezier" && argc == 6) {
		SuperficieBezier superficie;
		std::vector<Coordenadas3D> ptsSuperfcie;
		ComponentesPonto cPts;
		char *nome_fich_in;
		cmd = true;
		//     0     1      2    3       4           5
		// gerador bezier tessU tessV ficheiroIn ficheiroOut
		float tessU = atof(argv[2]);
		float tessV = atof(argv[3]);
		nome_fich_in = argv[4];
		nome_fich = argv[5];

		std::cout << "A ler ficheiro bezier..." << std::endl;
		superficie = leBezier(nome_fich_in);
		std::cout << "A gerar triangulos..." << std::endl;
		cPts = superficie.getPontos(tessU, tessV);

		std::ofstream ficheiro(modelos_path + nome_fich);


		for (int i = 0; i < cPts.pontos.size(); i++) {
			ficheiro << cPts.pontos[i].x << " " << cPts.pontos[i].y << " " << cPts.pontos[i].z << std::endl;
			ficheiro << cPts.normais[i].x << " " << cPts.normais[i].y << " " << cPts.normais[i].z << std::endl;
			ficheiro << cPts.coordsTextura[i].s << " " << cPts.coordsTextura[i].t << std::endl;
		}

		return 0;
	}

	if (!cmd) {
		std::cout << "Comando invalido ou numero de argumentos invalido." << std::endl;
	}
	else {
		std::ofstream ficheiro(modelos_path + nome_fich);

		std::vector<Coordenadas3D> pontos= figura.getPontos();
		std::vector<Coordenadas3D> normais = figura.getNormais();
		std::vector<CoordsTextura> textCoords= figura.getTextCoords();
		Coordenadas3D ponto, normal;
		CoordsTextura ct;

		for (int i = 0; i < pontos.size(); i++) {
			ponto = pontos[i];
			normal= normais[i];
			ct = textCoords[i];

			ficheiro << ponto.x << " " << ponto.y << " " << ponto.z << std::endl;
			ficheiro << normal.x << " " << normal.y << " " << normal.z << std::endl;
			ficheiro << ct.s << " " << ct.t << std::endl;
		}
	}
	
}


