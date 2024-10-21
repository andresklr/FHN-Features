#pragma once
#define _HARRIS_H

void SobelXY(float* GX, float* GY, float* X, int nx, int ny);

void Gauss(float* S, float* X, int nx, int ny);

void MultiplY(float* M, float* X, float* Y, int nx, int ny);


void CoarsitY(float* K, float* X, float* Y, float* Z, int nx, int ny);

void CoarsitY(float* K, float* X, float* Y, float* Z, int nx, int ny, float* candidates, float* threshold);

void Harris(unsigned int sizeX, unsigned int sizeY, float* in, float* out);