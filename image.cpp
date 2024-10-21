#include "image.h"

Mat readImage(std::string fileAddress) {
	Mat image = imread(fileAddress, IMREAD_COLOR);
	Mat image_float;
	image.convertTo(image_float, CV_32F);
	return image_float;
}

float* getGrayScaleFromImage(Mat* image, int* nx, int* ny) {
	*nx = image->rows;
	*ny = image->cols;

	int channels = image->channels();

	int nRows = *nx;
	int nCols = *ny;

	if (image->isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	float* result = (float*)malloc(*nx * *ny * sizeof(float));

	for (int i = 0; i < nRows; i++) {
		float* row = image->ptr<float>(i);
		for (int j = 0; j < nCols; j++) {
			//float* p = &row[j];
			float* p = (float*)malloc(channels * sizeof(float));
			for (int k = 0; k < channels; k++) {
				float* v = &row[(j * channels) + k];
				p[k] = v[0];
			}
			float pGS = getGrayScalePixelFromBGR(p);
			result[z(&i, &j, ny)] = pGS;
		}
	}

	return result;
}

int z(int* i, int* j, int* ny) {
	return (*i * *ny + *j);
}

int z(int i, int j, int ny) {
	return (i * ny + j);
}

float getGrayScalePixelFromBGR(float* p) {
	float result = (float)(0.2989 * p[2] + 0.5870 * p[1] + 0.1140 * p[0]);
	return result;
}

Mat grayScaleFloatToImage(Mat* image, float* in, int* nx, int* ny) {
	Mat result = Mat(*nx, *ny, image->type());

	int channels = result.channels();

	int nRows = result.rows;
	int nCols = result.cols;

	if (result.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	for (int i = 0; i < nRows; i++) {
		float* row = result.ptr<float>(i);
		for (int j = 0; j < nCols; j++) {
			for (int k = 0; k < channels; k++) {
				float* v = &row[(j * channels) + k];
				v[0] = in[z(&i, &j, ny)];
			}
		}
	}

	return result;
}

Mat writeCorners(Mat* image, float* out, int* nx, int* ny, uint* threshold, bool blackAndWhite) {
	Mat result = Mat(*nx, *ny, image->type());
	int nCorners = 0;

	int channels = result.channels();

	int nRows = result.rows;
	int nCols = result.cols;

	if (result.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	for (int i = 0; i < nRows; i++) {
		float* row = result.ptr<float>(i);
		float* rowR = image->ptr<float>(i);
		for (int j = 0; j < nCols; j++) {
			if (out[z(&i, &j, ny)] >= *threshold) {
				float* v = &row[(j * channels) + 0];
				v[0] = 255;
				nCorners++;
				for (int k = 1; k < channels; k++) {
					float* v = &row[(j * channels) + k];
					if (blackAndWhite)
					{
						v[k] = 255;
					}
					else
					{
						v[k] = 0;
					}
					
				}
			}
			else {
				for (int k = 0; k < channels; k++) {
					float* v = &row[(j * channels) + k];
					float* vR = &rowR[(j * channels) + k];
					if (blackAndWhite) 
					{
						v[k] = 0;
					}
					else 
					{
						v[k] = vR[k];
					}
					
				}
			}
		}
	}
	printMessage("Número de Corners:" + to_string(nCorners));
	return result;
}

