#pragma once
#include <string>
#include <iostream>
using namespace std;

void printMessage(std::string msg);

double getTime(time_t* start, time_t* end);

void countScores(int* nx, int* ny, float* out, float* threshold);

void printArray(int* nx, int* ny, float* out);

void identifyCorners(float* out, int* nx, int* ny, float* threshold);

float* getGroundTruth(string groundTruthFile);