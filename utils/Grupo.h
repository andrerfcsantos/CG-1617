#pragma once

#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include "Transformacoes.h"
#include "Ponto3D.h"

using namespace std;

class Grupo {

public:
	std::vector<std::string> ficheiros;
	std::vector<Transformacao> transformacoes;
	std::vector<Ponto3D> pontos;

	friend std::ostream& operator<<(std::ostream& os, const Grupo& t) {
		os << "Grupo{";
		os << "ficheiros={";

		
		os << "transformacoes={";
		for (auto it = t.transformacoes.begin(); it != t.transformacoes.end(); ++it) {
			os << *it << ", ";
		}
		os << "},";
		os << "ficheiros={";
		for (auto it = t.ficheiros.begin(); it != t.ficheiros.end(); ++it) {
			os << *it << ", ";
		}
		os << "}";

		return os;
	}
};
