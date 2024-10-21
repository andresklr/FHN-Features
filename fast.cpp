#include "fast.h"

const int radius = 3;

float* getFast4Candidates(float* in, int* nx, int* ny, int* threshold, int* candidates1D, int* candidates1DIndex) {
	int i, j;
	float p, p1, p5, p9, p13;
	float* candidatesAux = (float*)malloc(*nx * *ny * sizeof(float));
	float* out = (float*)malloc(*nx * *ny * sizeof(float));
	int index = 0;

	//Only p5 and p9
	for (i = 0; i < radius; i++) {
		for (j = 0; j < radius; j++) {
			if (checkP5andP9(&p, &p5, &p9, in, &i, &j, ny, threshold)) {
				out[z(&i, &j, ny)] = 1;
			}
			else {
				out[z(&i, &j, ny)] = 0;
			}
		}
	}

	//Only p5,p9 and p13
	for (i = 0; i < radius; i++) {
		for (j = radius; j < *ny - radius; j++) {
			if (checkP5P9andP13(&p, &p5, &p9, &p13, in, &i, &j, ny, threshold)) {
				out[z(&i, &j, ny)] = 1;
			}
			else {
				out[z(&i, &j, ny)] = 0;
			}
		}
	}

	//Only p9 and p13
	for (i = 0; i < radius; i++) {
		for (j = *ny - radius; j < *ny; j++) {
			if (checkP9andP13(&p, &p9, &p13, in, &i, &j, ny, threshold)) {
				out[z(&i, &j, ny)] = 1;
			}
			else {
				out[z(&i, &j, ny)] = 0;
			}
		}
	}

	//Only p1, p5 and p9
	for (i = radius; i < *nx - radius; i++) {
		for (j = 0; j < radius; j++) {
			if (checkP1P5andP9(&p, &p1, &p5, &p9, in, &i, &j, ny, threshold)) {
				out[z(&i, &j, ny)] = 1;
			}
			else {
				out[z(&i, &j, ny)] = 0;
			}
		}
	}

	//Only p1, p9 and p13
	for (i = radius; i < *nx - radius; i++) {
		for (j = *ny - radius; j < *ny; j++) {
			if (checkP1P9andP13(&p, &p1, &p9, &p13, in, &i, &j, ny, threshold)) {
				out[z(&i, &j, ny)] = 1;
			}
			else {
				out[z(&i, &j, ny)] = 0;
			}
		}
	}

	//This is for the middle, all pixels have bresenham circles defined
	for (i = radius; i < *nx - radius; i++) {
		for (j = radius; j < *ny - radius; j++) {
			if (checkP1P5P9andP13(&p, &p1, &p5, &p9, &p13, in, &i, &j, ny, threshold)) {
				out[z(&i, &j, ny)] = 1;
			}
			else {
				out[z(&i, &j, ny)] = 0;
			}
		}
	}

	//Only p1 and p5
	for (i = *nx - radius; i < *nx; i++) {
		for (j = 0; j < radius; j++) {
			if (checkP1andP5(&p, &p1, &p5, in, &i, &j, ny, threshold)) {
				out[z(&i, &j, ny)] = 1;
			}
			else {
				out[z(&i, &j, ny)] = 0;
			}
		}
	}

	//Only p1,p5 and p13
	for (i = *nx - radius; i < *nx; i++) {
		for (j = radius; j < *ny - radius; j++) {
			if (checkP1P5andP13(&p, &p1, &p5, &p13, in, &i, &j, ny, threshold)) {
				out[z(&i, &j, ny)] = 1;
				//candidates1D[index] = z(&i, &j, ny);
				//index++;
			}
			else {
				out[z(&i, &j, ny)] = 0;
			}
		}
	}

	//Only p1 and p13
	for (i = *nx - radius; i < *nx; i++) {
		for (j = *ny - radius; j < *ny; j++) {
			if (checkP1andP13(&p, &p1, &p13, in, &i, &j, ny, threshold)) {
				out[z(&i, &j, ny)] = 1;
			}
			else {
				out[z(&i, &j, ny)] = 0;
			}
		}
	}

	//candidates1D[index] = -1;

	fillCandidatesAux(nx, ny, out, candidatesAux);

	fillCandidates1D(nx, ny, candidatesAux, candidates1D, candidates1DIndex);

	return out;
}

void fillCandidatesAux(int* nx, int* ny, float* out, float* candidatesAux) {
	if (out[z(0, 0, *ny)] > 0) {
		candidatesAux[z(0, 0, *ny)] = 1;
		candidatesAux[z(0, 1, *ny)] = 1;
		candidatesAux[z(1, 0, *ny)] = 1;
		candidatesAux[z(1, 1, *ny)] = 1;
	}

	if (out[z(0, *ny - 1, *ny)] > 0) {
		candidatesAux[z(0, *ny - 1, *ny)] = 1;
		candidatesAux[z(0, *ny - 2, *ny)] = 1;
		candidatesAux[z(1, *ny - 1, *ny)] = 1;
		candidatesAux[z(1, *ny - 2, *ny)] = 1;
	}

	if (out[z(*nx - 1, 0, *ny)] > 0) {
		candidatesAux[z(*nx - 1, 0, *ny)] = 1;
		candidatesAux[z(*nx - 2, 0, *ny)] = 1;
		candidatesAux[z(*nx - 1, 1, *ny)] = 1;
		candidatesAux[z(*nx - 2, 1, *ny)] = 1;
	}

	if (out[z(*nx - 1, *ny - 1, *ny)] > 0) {
		candidatesAux[z(*nx - 1, *ny - 1, *ny)] = 1;
		candidatesAux[z(*nx - 1, *ny - 2, *ny)] = 1;
		candidatesAux[z(*nx - 2, *ny - 1, *ny)] = 1;
		candidatesAux[z(*nx - 2, *ny - 2, *ny)] = 1;
	}

	for (int j = 1; j < *ny - 1; j++) {
		if (out[z(0, j, *ny) > 0]) {
			candidatesAux[z(0, j, *ny)] = 1;
			candidatesAux[z(0, j - 1, *ny)] = 1;
			candidatesAux[z(0, j + 1, *ny)] = 1;
			candidatesAux[z(1, j, *ny)] = 1;
			candidatesAux[z(1, j - 1, *ny)] = 1;
			candidatesAux[z(1, j + 1, *ny)] = 1;
		}

		if (out[z(*nx - 1, j, *ny) > 0]) {
			candidatesAux[z(*nx - 1, j, *ny)] = 1;
			candidatesAux[z(*nx - 1, j - 1, *ny)] = 1;
			candidatesAux[z(*nx - 1, j + 1, *ny)] = 1;
			candidatesAux[z(*nx - 2, j, *ny)] = 1;
			candidatesAux[z(*nx - 2, j - 1, *ny)] = 1;
			candidatesAux[z(*nx - 2, j + 1, *ny)] = 1;
		}
	}

	for (int i = 1; i < *nx - 1; i++) {
		if (out[z(i, 0, *ny) > 0]) {
			candidatesAux[z(i, 0, *ny)] = 1;
			candidatesAux[z(i - 1, 0, *ny)] = 1;
			candidatesAux[z(i + 1, 0, *ny)] = 1;
			candidatesAux[z(i, 1, *ny)] = 1;
			candidatesAux[z(i - 1, 1, *ny)] = 1;
			candidatesAux[z(i + 1, 1, *ny)] = 1;
		}

		if (out[z(i, *ny - 1, *ny) > 0]) {
			candidatesAux[z(i, *ny - 1, *ny)] = 1;
			candidatesAux[z(i - 1, *ny - 1, *ny)] = 1;
			candidatesAux[z(i + 1, *ny - 1, *ny)] = 1;
			candidatesAux[z(i, *ny - 2, *ny)] = 1;
			candidatesAux[z(i - 1, *ny - 2, *ny)] = 1;
			candidatesAux[z(i + 1, *ny - 2, *ny)] = 1;
		}
	}

	for (int i = 1; i < *nx - 1; i++) {
		for (int j = 1; j < *ny - 1; j++) {
			if (out[z(i, j, *ny)] > 0) {
				candidatesAux[z(i, j, *ny)] = 1;
				candidatesAux[z(i - 1, j, *ny)] = 1;
				candidatesAux[z(i + 1, j, *ny)] = 1;
				candidatesAux[z(i - 1, j - 1, *ny)] = 1;
				candidatesAux[z(i + 1, j - 1, *ny)] = 1;
				candidatesAux[z(i, j - 1, *ny)] = 1;
				candidatesAux[z(i, j + 1, *ny)] = 1;
				candidatesAux[z(i - 1, j + 1, *ny)] = 1;
				candidatesAux[z(i + 1, j + 1, *ny)] = 1;
			}
		}
	}
}

void fillCandidates1D(int* nx, int* ny, float* candidatesAux, int* candidates1D, int* candidates1DIndex) {
	int index = 0;
	for (int i = 1; i < *nx - 1; i++) {
		for (int j = 1; j < *ny - 1; j++) {
			if (candidatesAux[z(i, j, *ny)] > 0) {
				candidates1D[index] = z(i, j, *ny);
				index++;
			}
		}
	}
	candidates1D[index] = -1;
	*candidates1DIndex = index;
}

bool checkP1andP5(float* p, float* p1, float* p5, float* in, int* i, int* j, int* ny, int* threshold) {
	*p = in[z(i, j, ny)];

	*p1 = getP1(in, i, j, ny);
	if (diff(p, p1, threshold))
	{
		return true;
	}

	*p5 = getP5(in, i, j, ny);
	if (diff(p, p5, threshold))
	{
		return true;
	}

	return false;
}

bool checkP5andP9(float* p, float* p5, float* p9, float* in, int* i, int* j, int* ny, int* threshold) {
	*p = in[z(i, j, ny)];

	*p5 = getP5(in, i, j, ny);
	if (diff(p, p5, threshold))
	{
		return true;
	}

	*p9 = getP9(in, i, j, ny);
	if (diff(p, p9, threshold))
	{
		return true;
	}
	return false;
}

bool checkP9andP13(float* p, float* p9, float* p13, float* in, int* i, int* j, int* ny, int* threshold) {
	*p = in[z(i, j, ny)];

	*p13 = getP13(in, i, j, ny);
	if (diff(p, p13, threshold))
	{
		return true;
	}

	*p9 = getP9(in, i, j, ny);
	if (diff(p, p9, threshold))
	{
		return true;
	}

	return false;
}

bool checkP1andP13(float* p, float* p1, float* p13, float* in, int* i, int* j, int* ny, int* threshold) {
	*p = in[z(i, j, ny)];

	*p1 = getP1(in, i, j, ny);
	if (diff(p, p1, threshold))
	{
		return true;
	}

	*p13 = getP13(in, i, j, ny);
	if (diff(p, p13, threshold))
	{
		return true;
	}

	return false;
}

bool checkP5P9andP13(float* p, float* p5, float* p9, float* p13, float* in, int* i, int* j, int* ny, int* threshold) {
	*p = in[z(i, j, ny)];

	*p13 = getP13(in, i, j, ny);
	if (diff(p, p13, threshold))
	{
		return true;
	}

	*p5 = getP5(in, i, j, ny);
	if (diff(p, p5, threshold))
	{
		return true;
	}

	*p9 = getP9(in, i, j, ny);
	if (diff(p, p9, threshold))
	{
		return true;
	}

	return false;
}

bool checkP1P5andP9(float* p, float* p1, float* p5, float* p9, float* in, int* i, int* j, int* ny, int* threshold) {
	*p = in[z(i, j, ny)];

	*p1 = getP1(in, i, j, ny);
	if (diff(p, p1, threshold))
	{
		return true;
	}

	*p5 = getP5(in, i, j, ny);
	if (diff(p, p5, threshold))
	{
		return true;
	}

	*p9 = getP9(in, i, j, ny);
	if (diff(p, p9, threshold))
	{
		return true;
	}

	return false;
}

bool checkP1P9andP13(float* p, float* p1, float* p9, float* p13, float* in, int* i, int* j, int* ny, int* threshold) {
	*p = in[z(i, j, ny)];

	*p1 = getP1(in, i, j, ny);
	if (diff(p, p1, threshold))
	{
		return true;
	}

	*p13 = getP13(in, i, j, ny);
	if (diff(p, p13, threshold))
	{
		return true;
	}

	*p9 = getP9(in, i, j, ny);
	if (diff(p, p9, threshold))
	{
		return true;
	}

	return false;
}

bool checkP1P5andP13(float* p, float* p1, float* p5, float* p13, float* in, int* i, int* j, int* ny, int* threshold) {
	*p = in[z(i, j, ny)];

	*p1 = getP1(in, i, j, ny);
	if (diff(p, p1, threshold))
	{
		return true;
	}

	*p13 = getP13(in, i, j, ny);
	if (diff(p, p13, threshold))
	{
		return true;
	}

	*p5 = getP5(in, i, j, ny);
	if (diff(p, p5, threshold))
	{
		return true;
	}

	return false;
}

bool checkP1P5P9andP13(float* p, float* p1, float* p5, float* p9, float* p13, float* in, int* i, int* j, int* ny, int* threshold) {
	*p = in[z(i, j, ny)];

	*p1 = getP1(in, i, j, ny);	
	if (diff(p, p1, threshold)) 
	{
		return true;
	}
		
	*p13 = getP13(in, i, j, ny);
	if (diff(p, p13, threshold))
	{
		return true;
	}

	*p5 = getP5(in, i, j, ny);
	if (diff(p, p5, threshold))
	{
		return true;
	}

	*p9 = getP9(in, i, j, ny);
	if (diff(p, p9, threshold))
	{
		return true;
	}

	return false;
}

bool diff(float* p, float* px, int* threshold) {
	return *p > (*px + *threshold) || *p < (*px - *threshold);
}

float getP1(float* in, int* i, int* j, int* ny) {
	return in[z(*i - radius, *j, *ny)];
}

float getP5(float* in, int* i, int* j, int* ny) {
	return in[z(*i, *j + radius, *ny)];
}

float getP9(float* in, int* i, int* j, int* ny) {
	return in[z(*i + radius, *j, *ny)];
}

float getP13(float* in, int* i, int* j, int* ny) {
	return in[z(*i, *j - radius, *ny)];
}