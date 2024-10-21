#pragma once
#include <iostream>
#include "image.h"

using namespace std;

void NonMaximumSuppression(int* windowSize, int* maxCornersPerWindow, int* nx, int* ny, float* scores, float* threshold);

void calculateNMSByWindow(int* windowSize, int* maxCornersPerWindow, int* radius, int* i, int* j, int* nx, int* ny, float* scores, float* threshold);

bool sortbysecdesc(const pair<int, float>& a, const pair<int, float>& b);