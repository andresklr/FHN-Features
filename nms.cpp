#include "nms.h"


void NonMaximumSuppression(int* windowSize, int* maxCornersPerWindow, int* nx, int* ny, float* scores, float* threshold) {
	int radius = *windowSize - 2;	
	for (int i = radius; i < *nx - radius; i++) {
		for (int j = radius; j < *ny - radius; j++) {
			int index = z(i, j, *ny);
			if (scores[index] > *threshold)
			{
				calculateNMSByWindow(windowSize, maxCornersPerWindow, &radius, &i, &j,nx, ny, scores, threshold);

			}
		}
	}
}


void calculateNMSByWindow(int* windowSize, int* maxCornersPerWindow, int *radius, int *i, int *j, int* nx, int* ny, float* scores, float* threshold) {
	vector<pair<int, float> > windowValue;	
	int valuesCount = 0;

	for (int a = 0; a < *windowSize; a++) {
		for (int b = 0; b < *windowSize; b++) {
			int index = z(*i + a - *radius, *j + b - *radius, *ny);
			float R = scores[index];
			if (R > *threshold) {
				pair<int, float> myPair = make_pair(index, R);
				windowValue.push_back(myPair);
				valuesCount++;
			}
		}
	}	

	if (valuesCount > 0) {
		sort(windowValue.begin(), windowValue.end(), sortbysecdesc);

		for (int a = *maxCornersPerWindow; a < valuesCount; a++) {
			scores[windowValue[a].first] = 0;
		}

		valuesCount = 0;
	}	
}

bool sortbysecdesc(const pair<int, float>& a, const pair<int, float>& b)
{
	return a.second > b.second;
}
