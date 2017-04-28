#pragma once

#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <utility>
#include "Transformacoes.h"
#include "Coordenadas3D.h"
#include "DefsDesenho.h"
#include "Desenho.h"

using namespace std;

class Grupo {

public:
	std::string nome;
	std::vector<std::string> ficheiros;
	std::vector<Transformacao> transformacoes;
	std::vector<Desenho> desenhos;
	std::vector<Desenho> catmullDes;
	int nrCRPtsToDraw;

	friend std::ostream& operator<<(std::ostream& os, const Grupo& t) {
		if (t.nome != "") {
			os << "Grupo\"" << t.nome << "\"{";
		}
		else {
			os << "Grupo{";
		}
		os << "transformacoes=[";
		for (auto it = t.transformacoes.begin(); it != t.transformacoes.end(); ++it) {
			if (it== t.transformacoes.end()-1) os << *it;
			else os << *it << ", ";
		}
		os << "],";
		os << "ficheiros=[";
		for (auto it = t.ficheiros.begin(); it != t.ficheiros.end(); ++it) {
			if (it == t.ficheiros.end() - 1) os << *it;
			else os << *it << ", ";
		}
		os << "]";
		os << "}";

		return os;
	}
};
