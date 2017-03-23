#pragma once

struct Ponto3D {
	float x, y, z;
	
	Ponto3D& operator+(const Ponto3D& p2) {
		Ponto3D res;
		res.x = x + p2.x;
		res.y = y + p2.y;
		res.z = z + p2.z;
		return res;
	}

	Ponto3D& operator-(const Ponto3D& p2) {
		Ponto3D res;
		res.x = x - p2.x;
		res.y = y - p2.y;
		res.z = z - p2.z;
		return res;
	}

	Ponto3D& crossproduct(const Ponto3D& p2) {
		Ponto3D res;
		res.x = y*p2.z - z*p2.y;
		res.y = z*p2.x - x*p2.z;
		res.z = x*p2.y - y*p2.x;
		return res;
	}

	Ponto3D& times(float k) {
		Ponto3D res;
		res.x = x * k;
		res.y = y * k;
		res.z = z * k;
		return res;
	}
};

