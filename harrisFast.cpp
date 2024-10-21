#include "harrisFast.h"

void HarrisFast(unsigned int sizeX, unsigned int sizeY, float* in, float* out, float* candidates, float* threshold, int* candidates1D, int* candidates1DIndex) {
	unsigned int i, j;

	float* Gx, * Gy, * Ixx, * Iyy, * Ixy, * Sxx, * Sxy, * Syy;
	Gx = (float*)malloc(sizeX * sizeY * sizeof(float));
	Gy = (float*)malloc(sizeX * sizeY * sizeof(float));
	Ixx = (float*)malloc(sizeX * sizeY * sizeof(float));
	Iyy = (float*)malloc(sizeX * sizeY * sizeof(float));
	Ixy = (float*)malloc(sizeX * sizeY * sizeof(float));
	Sxx = (float*)malloc(sizeX * sizeY * sizeof(float));
	Sxy = (float*)malloc(sizeX * sizeY * sizeof(float));
	Syy = (float*)malloc(sizeX * sizeY * sizeof(float));

	/* Now we run the Harris procedure */
	//Sobel
	SobelXY(Gx, Gy, in, sizeX, sizeY, candidates1D, candidates1DIndex);
	//Multiply
	MultiplY(Ixx, Gx, Gx, sizeX, sizeY, candidates1D, candidates1DIndex);
	MultiplY(Iyy, Gy, Gy, sizeX, sizeY, candidates1D, candidates1DIndex);
	MultiplY(Ixy, Gx, Gy, sizeX, sizeY, candidates1D, candidates1DIndex);
	//Gauss
	Gauss(Sxx, Ixx, sizeX, sizeY, candidates1D, candidates1DIndex);
	Gauss(Syy, Iyy, sizeX, sizeY, candidates1D, candidates1DIndex);
	Gauss(Sxy, Ixy, sizeX, sizeY, candidates1D, candidates1DIndex);
	//Coarsity
	CoarsitY(out, Sxx, Syy, Sxy, sizeX, sizeY, candidates1D, candidates1DIndex);
}

void SobelXY(float* GX, float* GY, float* X, int nx, int ny, int* candidates1D, int* candidates1DIndex) {
	unsigned int i, j;
	//#pragma omp parallel
	for (i = 0; i < *candidates1DIndex; i++) {
		GX[candidates1D[i]] = (X[candidates1D[i] - ny - 1] - X[candidates1D[i] - ny + 1] + 2 * (X[candidates1D[i] - 1] - 2 * X[candidates1D[i] + 1]) + X[candidates1D[i] + ny - 1] - X[candidates1D[i] + ny + 1]) / 8;
		GY[candidates1D[i]] = (X[candidates1D[i] - ny - 1] - X[candidates1D[i] + ny - 1] + 2 * (X[candidates1D[i] - ny] - X[candidates1D[i] + ny]) + X[candidates1D[i] - ny + 1] + X[candidates1D[i] + ny + 1]) / 8;
	}
	/*for (i = 1; i < nx - 1; i++)
		for (j = 1; j < ny - 1; j++) {
			GX[z(i, j, ny)] = (X[z(i - 1, j - 1, ny)] - X[z(i - 1, j + 1, ny)] + 2 * (X[z(i, j - 1, ny)] - 2 * X[z(i, j + 1, ny)]) + X[z(i + 1, j - 1, ny)] - X[z(i + 1, j + 1, ny)]) / 8;
			GY[z(i, j, ny)] = (X[z(i - 1, j - 1, ny)] - X[z(i + 1, j - 1, ny)] + 2 * (X[z(i - 1, j, ny)] - X[z(i + 1, j, ny)]) + X[z(i - 1, j + 1, ny)] + X[z(i + 1, j + 1, ny)]) / 8;
		}*/
		
}

void Gauss(float* S, float* X, int nx, int ny, int* candidates1D, int* candidates1DIndex) {
	unsigned int i, j;
	//#pragma omp parallel
	for (i = 0; i < *candidates1DIndex; i++) {
		S[candidates1D[i]] = (X[candidates1D[i] - ny - 1] + 2 * X[candidates1D[i] - ny] + X[candidates1D[i] - ny + 1] + 2 * (X[candidates1D[i] - 1] + 2 * X[candidates1D[i]] + X[candidates1D[i] + 1]) + X[candidates1D[i] + ny - 1] + X[candidates1D[i] + ny] + X[candidates1D[i] + ny + 1]) / 16;
	}
	/*for (i = 1; i < nx - 1; i++) {
		for (j = 1; j < ny - 1; j++) {
			S[z(i, j, ny)] = (X[z(i - 1, j - 1, ny)] + 2 * X[z(i - 1, j, ny)] + X[z(i - 1, j + 1, ny)] + 2 * (X[z(i, j - 1, ny)] + 2 * X[z(i, j, ny)] + X[z(i, j + 1, ny)]) + X[z(i + 1, j - 1, ny)] + X[z(i + 1, j, ny)] + X[z(i + 1, j + 1, ny)]) / 16;
		}
	}*/
}

void MultiplY(float* M, float* X, float* Y, int nx, int ny, int* candidates1D, int* candidates1DIndex) {
	unsigned int i, j;
	//#pragma omp parallel

	for (i = 0; i < *candidates1DIndex; i++) {
		M[candidates1D[i]] = X[candidates1D[i]] * Y[candidates1D[i]];
	}

	/*for (i = 0; i < nx; i++) {
		for (j = 0; j < ny; j++) {
			M[z(i, j, ny)] = X[z(i, j, ny)] * Y[z(i, j, ny)];
		}
	}*/
}

void CoarsitY(float* K, float* X, float* Y, float* Z, int nx, int ny, int* candidates1D, int* candidates1DIndex) {
	unsigned int i, j;

	//#pragma omp parallel	
	for (i = 0; i < *candidates1DIndex; i++) {
		K[candidates1D[i]] = X[candidates1D[i]] * Y[candidates1D[i]] - Z[candidates1D[i]] * Z[candidates1D[i]];
	}

	/*for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			K[z(i, j, ny)] = X[z(i, j, ny)] * Y[z(i, j, ny)] - Z[z(i, j, ny)] * Z[z(i, j, ny)];*/
}