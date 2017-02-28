#pragma once

struct Ponto3D {
	float x, y, z;

	Ponto3D& operator+(const Ponto3D& p2) {
		x += p2.x;
		y += p2.y;
		z += p2.z;
		return *this;
	}
};

