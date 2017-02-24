#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../../utils/Ponto3D.h"

std::vector<Ponto3D> figura;

void geraPlano(float comp, float larg, int divs) {
	float x, z;
	float deltaComp = (float) comp/divs;
	float deltaLarg = (float) larg/divs;

	for (int j = 0; j < divs; ++j) {
		for (int i = 0; i < divs; ++i) {
			x = -larg / 2 + deltaComp*i;
			z = comp / 2 + deltaLarg*j;
			Ponto3D a = { x, 0, z };
			Ponto3D b = { x + deltaComp, 0, z };
			Ponto3D c = { x + deltaComp, 0, z - deltaLarg };
			Ponto3D d = { x, 0, z - deltaLarg };

			figura.push_back(a);
			figura.push_back(b);
			figura.push_back(c);
			figura.push_back(a);
			figura.push_back(c);
			figura.push_back(d);
		}
	}


}

int main(int argc, char** argv) {
	std::string str_ficheiro;
	std::string str_figura = argv[1];
	bool cmd = false;

	if (str_figura == "plane") {
		cmd = true;
		str_ficheiro = argv[5];
		// gerador plane comprimento largura divisoes
		float comp = atof(argv[2]), largura = atof(argv[3]);
		int divisoes = atoi(argv[4])+1;
		geraPlano(comp,largura,divisoes);
	}

	if (!cmd) std::cout << "Comando invalido! :(" << std::endl;

	std::ofstream ficheiro(str_ficheiro);
	for (Ponto3D ponto : figura) {
		ficheiro << ponto.x << " " << ponto.y << " " << ponto.z << std::endl;
	}

	

}


