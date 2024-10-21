#pragma once
#include "image.h"
#include "tools.h"

float* getFast4Candidates(float* in, int* nx, int* ny, int* threshold, int* candidates1D, int* candidates1DIndex);

bool checkP1andP5(float* p, float* p1, float* p5, float* in, int* i, int* j, int* ny, int* threshold);

bool checkP5andP9(float* p, float* p5, float* p9, float* in, int* i, int* j, int* ny, int* threshold);

bool checkP9andP13(float* p, float* p9, float* p13, float* in, int* i, int* j, int* ny, int* threshold);

bool checkP1andP13(float* p, float* p1, float* p13, float* in, int* i, int* j, int* ny, int* threshold);

bool checkP5P9andP13(float* p, float* p5, float* p9, float* p13, float* in, int* i, int* j, int* ny, int* threshold);

bool checkP1P5andP9(float* p, float* p1, float* p5, float* p9, float* in, int* i, int* j, int* ny, int* threshold);

bool checkP1P9andP13(float* p, float* p1, float* p9, float* p13, float* in, int* i, int* j, int* ny, int* threshold);

bool checkP1P5andP13(float* p, float* p1, float* p5, float* p13, float* in, int* i, int* j, int* ny, int* threshold);

bool checkP1P5P9andP13(float* p, float* p1, float* p5, float* p9, float* p13, float* in, int* i, int* j, int* ny, int* threshold);

bool diff(float* p, float* px, int* threshold);

float getP1(float* in, int* i, int* j, int* ny);

float getP5(float* in, int* i, int* j, int* ny);

float getP9(float* in, int* i, int* j, int* ny);

float getP13(float* in, int* i, int* j, int* ny);

void fillCandidatesAux(int* nx, int* ny, float* out, float* candidatesAux);

void fillCandidates1D(int* nx, int* ny, float* candidatesAux, int* candidates1D, int* candidates1DIndex);