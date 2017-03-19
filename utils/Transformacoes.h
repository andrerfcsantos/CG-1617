#pragma once

enum TipoTransformacao { ROTACAO, TRANSLACAO, ESCALA };

class Transformacao {
public:
	TipoTransformacao tipo;
};

class Translacao : public Transformacao {
public:
	float tx, ty, tz;

	Translacao() : tx(0), ty(0), tz(0) {
		tipo = TRANSLACAO;
	}

};

class Rotacao : public Transformacao {
public:
	float rang, rx, ry, rz;

	Rotacao() : rang(0), rx(0), ry(0), rz(0) {
		tipo = ROTACAO;
	}

};

class Escala : public Transformacao {
public:
	float sx, sy, sz;

	Escala() : sx(1), sy(1), sz(1) {
		tipo = ESCALA;
	}

};
