#pragma once
#include <opencv.hpp>
#include <iostream>
#include "tools.h"

using namespace cv;

Mat readImage(std::string fileAddress);

float* getGrayScaleFromImage(Mat* image, int* nx, int* ny);

int z(int* i, int* j, int* ny);

int z(int i, int j, int ny);

Mat grayScaleFloatToImage(Mat* image, float* in, int* nx, int* ny);

Mat writeCorners(Mat* image, float* out, int* nx, int* ny, uint* threshold, bool blackAndWhite);

float getGrayScalePixelFromBGR(float* p);

