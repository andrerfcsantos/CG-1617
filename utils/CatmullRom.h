#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include "Coordenadas3D.h"

void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float *a, float *b, float *res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(float *a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}


float length(float *v) {

	float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return res;

}



void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

void getCatmullRomPoint(float t,
						Coordenadas3D p0, Coordenadas3D p1,
						Coordenadas3D p2, Coordenadas3D p3,
						float *res, float *deriv) {

	// catmull-rom matrix
	float m[16] = { -0.5f,  1.5f, -1.5f,  0.5f,
		1.0f, -2.5f,  2.0f, -0.5f,
		-0.5f,  0.0f,  0.5f,  0.0f,
		0.0f,  1.0f,  0.0f,  0.0f };

	// reset res and deriv
	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;
	deriv[0] = 0.0; deriv[1] = 0.0; deriv[2] = 0.0;

	float Ax[4], Ay[4], Az[4];
	// Compute Ax = M * Px
	float Px[4] = { p0.x,p1.x,p2.x,p3.x };
	multMatrixVector(m, Px, Ax);

	// Compute Ay = M * Py
	float Py[4] = { p0.y,p1.y,p2.y,p3.y };
	multMatrixVector(m, Py, Ay);

	// Compute Az = M * Pz
	float Pz[4] = { p0.z,p1.z,p2.z,p3.z };
	multMatrixVector(m, Pz, Az);


	// T = [t^3, t^2, t, 1]
	float tv[4] = { pow(t,3),pow(t,2),pow(t,1),1 };

	// Compute point resx = T *Ax
	res[0] = tv[0] * Ax[0] + tv[1] * Ax[1] + tv[2] * Ax[2] + tv[3] * Ax[3];
	// Compute point resy = T *Ay
	res[1] = tv[0] * Ay[0] + tv[1] * Ay[1] + tv[2] * Ay[2] + tv[3] * Ay[3];
	// Compute point resz = T *Az
	res[2] = tv[0] * Az[0] + tv[1] * Az[1] + tv[2] * Az[2] + tv[3] * Az[3];


	// T = [3t^2, 2t, 1, 0]
	// compute deriv = T' * Ax
	float tvl[4] = { 3 * pow(t,2),2 * t,1,0 };
	deriv[0] = tvl[0] * Ax[0] + tvl[1] * Ax[1] + tvl[2] * Ax[2] + tvl[3] * Ax[3];
	// compute deriv = T' * Ay
	deriv[1] = tvl[0] * Ay[0] + tvl[1] * Ay[1] + tvl[2] * Ay[2] + tvl[3] * Ay[3];
	// compute deriv = T' * Az
	deriv[2] = tvl[0] * Az[0] + tvl[1] * Az[1] + tvl[2] * Az[2] + tvl[3] * Az[3];

}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(std::vector<Coordenadas3D> p,
								float gt,
								float *res, float *deriv) {
	int point_count = p.size();
	float t = gt * point_count;
	int index = floor(t);
	t = t - index;

	int indices[4];
	indices[0] = (index + point_count - 1) % point_count;
	indices[1] = (indices[0] + 1) % point_count;
	indices[2] = (indices[1] + 1) % point_count;
	indices[3] = (indices[2] + 1) % point_count;

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], res, deriv);
}


