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
	vector<Transformacao> transformacoes;
	vector<Ponto3D> pontos;
};
