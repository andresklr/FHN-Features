#pragma once
#define _HARRIS_H
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

void HarrisFast(unsigned int sizeX, unsigned int sizeY, float* in, float* out, float* candidates, float* threshold, int* candidates1D, int* candidates1DIndex);

void SobelXY(float* GX, float* GY, float* X, int nx, int ny, int* candidates1D, int* candidates1DIndex);

void Gauss(float* S, float* X, int nx, int ny, int* candidates1D, int* candidates1DIndex);

void MultiplY(float* M, float* X, float* Y, int nx, int ny, int* candidates1D, int* candidates1DIndex);

void CoarsitY(float* K, float* X, float* Y, float* Z, int nx, int ny, int* candidates1D, int* candidates1DIndex);