// HarrisFast.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <chrono>
#include <iostream>
#include "harris.h"
#include "tools.h"
#include "image.h"
#include "fast.h"
#include "nms.h"
#include "harrisFast.h"
#include <filesystem>

using namespace std;

void main() {

	int fastThreshold = 10;
	int nmsWindowSize = 3;
	int maxCornersNMS = 6;
	float threshold = 100000;
	int groundTruthThreshold = 1;
	//int groundTruthThreshold = threshold;
	int typeExecution = 1; //0 - Harris, 1 - Fast+Harris, 2 - Both, 3 - Both + BSDS500 GroundTruth
	bool nms = false;
	bool blackAndWhite = true;
	string textNMS = nms ? "-nms-" : "";
	int numberOfExecutions = 1;
	string file = "Sara";
	string fileFormat = ".jpg";
	// string basePath = "C:\\GitHub\\UERJME\\Dissertação\\Benchmark\\BSDS500\\";
	string basePath = "C:\\GitHub\\UERJME\\Dissertação\\Code\\HarrisFast\\data\\";
	//string imageName = basePath + "images\\" + file;
	string imageName = basePath + "\\" + file;
	string groundTruthFile = basePath + "groundTruth\\txts\\train-edges\\" + file + ".txt";

	Mat image = readImage(imageName + fileFormat);

	int nx, ny;

	float* in = getGrayScaleFromImage(&image, &nx, &ny);

	Mat image2 = grayScaleFloatToImage(&image, in, &nx, &ny);

	imwrite(imageName + "-grayscale" + fileFormat, image2);

	int candidates1DIndex = 0;
	int* candidates1D = (int*)malloc(nx * ny * sizeof(int));
	float* out = (float*)malloc(nx * ny * sizeof(float));
	float* out2 = (float*)malloc(nx * ny * sizeof(float));
	float* out3 = (float*)malloc(nx * ny * sizeof(float));

	auto start_time_F = std::chrono::high_resolution_clock::now();
	auto end_time_F = start_time_F;

	for (int i = 0; i < numberOfExecutions; i++) {
		if (typeExecution > 0) {
			float* candidates = getFast4Candidates(in, &nx, &ny, &fastThreshold, candidates1D, &candidates1DIndex);
			out3 = candidates;

			HarrisFast(nx, ny, in, out2, candidates, &threshold, candidates1D, &candidates1DIndex);
			if (nms) {
				NonMaximumSuppression(&nmsWindowSize, &maxCornersNMS, &nx, &ny, out2, &threshold);
			}
			//countScores(&nx, &ny, out2, &threshold);
		}
	}

	end_time_F = std::chrono::high_resolution_clock::now();

	auto time_F = end_time_F - start_time_F;

	auto start_time_H = std::chrono::high_resolution_clock::now();
	auto end_time_H = start_time_H;

	for (int i = 0; i < numberOfExecutions; i++) {
		if (typeExecution != 1) {
			Harris(nx, ny, in, out);
			/*if (nms) {
				NonMaximumSuppression(&nmsWindowSize, &maxCornersNMS, &nx, &ny, out, &threshold);
			}*/
		}
	}

	end_time_H = std::chrono::high_resolution_clock::now();

	auto time_H = end_time_H - start_time_H;

	float speedUp = ((float)(time_H / (std::chrono::milliseconds(1)))) / ((float)(time_F / std::chrono::milliseconds(1)));

	printMessage("tiempo de duración Harris (ms): " + to_string(time_H / std::chrono::milliseconds(1)));

	printMessage("tiempo de duración FAST (ms): " + to_string(time_F / std::chrono::milliseconds(1)));

	printMessage("Speedup: " + to_string(speedUp));

	printMessage(to_string(time_H / std::chrono::milliseconds(1)) + ";" +
		to_string(time_F / std::chrono::milliseconds(1)) + ";" + to_string(speedUp) + ";" + file);

	float* groundTruth;

	if (typeExecution > 2)
	{
		groundTruth = getGroundTruth(groundTruthFile);
	}
	else {
		groundTruth = out;
	}

	float FPH1 = 0, TPH1 = 0, TNH1 = 0, FNH1 = 0, FPH2 = 0, TPH2 = 0, TNH2 = 0, FNH2 = 0;
	float NOP = 0, NOF1 = 0, NOF2 = 0, POF1 = 0, POF2 = 0;

	Mat result = Mat(nx, ny, image2.type());

	int nRows = result.rows;
	int nCols = result.cols;

	if (result.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	NOP = nx * ny;

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			if (groundTruth[z(i, j, ny)] > groundTruthThreshold) {
				if (out[z(i, j, ny)] > threshold) {
					TPH1++;
					NOF1++;
				}
				else {
					FNH1++;
				}

				if (out2[z(i, j, ny)] > threshold) {
					TPH2++;
					NOF2++;
				}
				else {
					FNH2++;
				}
			}
			else {
				if (out[z(i, j, ny)] > threshold) {
					FPH1++;
					NOF1++;
				}
				else {
					TNH1++;
				}

				if (out2[z(i, j, ny)] > threshold) {
					FPH2++;
					NOF2++;
				}
				else {
					TNH2++;
				}
			}
		}
	}

	printMessage("TPH1: " + to_string(TPH1) + " FPH1: " + to_string(FPH1) + " TNH1: " + to_string(TNH1) + " FNH1: " + to_string(FNH1));

	printMessage("TPH2: " + to_string(TPH2) + " FPH2: " + to_string(FPH2) + " TNH2: " + to_string(TNH2) + " FNH2: " + to_string(FNH2));

	float accuracyH1 = 0, precisionH1 = 0, recallH1 = 0, f1H1 = 0, accuracyH2 = 0, precisionH2 = 0, recallH2 = 0, f1H2 = 0;

	accuracyH1 = (TPH1 + TNH1) / (TPH1 + TNH1 + FPH1 + FNH1);

	precisionH1 = (TPH1) / (TPH1 + FPH1);

	recallH1 = (TPH1) / (TPH1 + FNH1);

	f1H1 = (2 * TPH1) / ((2 * TPH1) + FPH1 + FNH1);

	printMessage("accuracyH1: " + to_string(accuracyH1) +
		" precisionH1: " + to_string(precisionH1) + " recallH1: " + to_string(recallH1) +
		" f1H1: " + to_string(f1H1));

	accuracyH2 = (TPH2 + TNH2) / (TPH2 + TNH2 + FPH2 + FNH2);

	precisionH2 = (TPH2) / (TPH2 + FPH2);

	recallH2 = (TPH2) / (TPH2 + FNH2);

	f1H2 = (2 * TPH2) / ((2 * TPH2) + FPH2 + FNH2);

	printMessage("accuracyH2: " + to_string(accuracyH2) +
		" precisionH2: " + to_string(precisionH2) + " recallH2: " + to_string(recallH2) +
		" f1H2: " + to_string(f1H2));

	/*printMessage(to_string(accuracyH2) +
		";" + to_string(precisionH2) + ";" + to_string(recallH2) +
		";" + to_string(f1H2));*/

	POF1 = NOF1 / NOP;

	POF2 = NOF2 / NOP;

	//printMessage("POF1: " + to_string(POF1) + " POF2: " + to_string(POF2));
	printMessage(to_string(POF1) + ";" + to_string(POF2));

	bool multiple = false;

	if (multiple) {
		for (float i = 1; i <= 16; i++) {
			uint t = (uint)threshold * (i / 4);

			Mat image3 = writeCorners(&image, out, &nx, &ny, &t, blackAndWhite);

			imwrite(imageName + "-t-" + to_string(t) + fileFormat, image3);
		}
	}
	else {
		uint t = (uint)threshold;
		uint t2 = (uint)groundTruthThreshold;

		if (typeExecution != 1) {
			Mat image3 = writeCorners(&image, out, &nx, &ny, &t, blackAndWhite);

			imwrite(imageName + "-Harris-t-" + to_string(t) + textNMS + fileFormat, image3);
		}
		if (typeExecution > 0) {
			Mat image3 = writeCorners(&image, out2, &nx, &ny, &t, blackAndWhite);

			imwrite(imageName + "-FastAndHarris-t-" + to_string(t) + textNMS + fileFormat, image3);

			Mat image4 = writeCorners(&image, out3, &nx, &ny, &t2, blackAndWhite);

			imwrite(imageName + "-Fast4-t-" + to_string(t) + textNMS + fileFormat, image4);
		}

		if (typeExecution > 2) {
			Mat image3 = writeCorners(&image, groundTruth, &nx, &ny, &t2, blackAndWhite);

			imwrite(imageName + "-GroundTruth" + fileFormat, image3);
		}
	}

	//identifyCorners(out, &nx, &ny, &threshold);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
