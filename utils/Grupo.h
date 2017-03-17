#pragma once

#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include "Transformacao.h"

using namespace std;

class Grupo {

public:
	vector<Transformacao> transformacoes;
	vector<string> ficheiros;

};
