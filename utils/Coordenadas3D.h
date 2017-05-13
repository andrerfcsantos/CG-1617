#pragma once

struct Coordenadas3D {
	float x, y, z;
	
	Coordenadas3D operator+(const Coordenadas3D& p2) {
		Coordenadas3D res;
		res.x = x + p2.x;
		res.y = y + p2.y;
		res.z = z + p2.z;
		return res;
	}

	Coordenadas3D operator-(const Coordenadas3D& p2) {
		Coordenadas3D res;
		res.x = x - p2.x;
		res.y = y - p2.y;
		res.z = z - p2.z;
		return res;
	}

	Coordenadas3D crossproduct(const Coordenadas3D& p2) {
		Coordenadas3D res;
		res.x = y*p2.z - z*p2.y;
		res.y = z*p2.x - x*p2.z;
		res.z = x*p2.y - y*p2.x;
		return res;
	}

	Coordenadas3D operator*(float k) {
		Coordenadas3D res;
		res.x = x * k;
		res.y = y * k;
		res.z = z * k;
		return res;
	}

	Coordenadas3D inv() {
		Coordenadas3D res;
		res.x = -x;
		res.y = -y;
		res.z = -z;
		return res;
	}

	Coordenadas3D times(float k) {
		Coordenadas3D res;
		res.x = x * k;
		res.y = y * k;
		res.z = z * k;
		return res;
	}

	Coordenadas3D normalize() {
		Coordenadas3D res;
		float l = sqrt(x*x + y*y + z*z);
		if (l == 0) {
			res = Coordenadas3D{ 0.0,0.0,0.0 };
		}
		else{
			res.x = x / l;
			res.y = y / l;
			res.z = z / l;
		}
		return res;
	}
};

