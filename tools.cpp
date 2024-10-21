#include "tools.h"
#include "image.h"
#include <fstream>

void printMessage(string msg) {
    cout << msg << "\n";
}

void identifyCorners(float* out, int* nx, int* ny, float* threshold) {
    for (int i = 0; i < *nx; i++) {
        for (int j = 0; j < *ny; j++) {
            if (out[z(i, j, *ny)] > *threshold) {
                printMessage(("i: " + to_string(i) + " j: " + to_string(j)));
            }            
        }
    }    
}

double getTime(time_t* start, time_t* end) {
    return double (end - start);
}

void printArray(int *nx, int *ny, float* out) {
    for (int i = 0; i < *nx; i++) {
        string s = "";
        for (int j = 0; j < *ny; j++) {
            s += " " + to_string((out[z(&i, &j, ny)]));
        }
        printMessage(s);
    }
}

void countScores(int *nx, int *ny, float* out, float* threshold) {
    float minValue = +1, maxValue = -1, nAverage = 0, pAverage = 0, nSum = 0, pSum = 0;
    int nCount = 0, pCount = 0;// , cCount = 0;

    for (int i = 0; i < *nx; i++) {        
        for (int j = 0; j < *ny; j++) {
            float value = out[z(&i, &j, ny)];
            if (value >= 0) {
                pCount++;
                pSum += value;
                if (maxValue < value) {
                    maxValue = value;
                }
                //if (value > *threshold) {
                //    cCount++;
                //}
            }
            else {
                nCount++;
                nSum += value;
                if (minValue > value) {
                    minValue = value;
                }
            }
        }        
    }

    nAverage = nSum / nCount;
    pAverage = pSum / pCount;

    printMessage("Valor máximo: " + to_string(maxValue));
    printMessage("Média positiva: " + to_string(pAverage));
    printMessage("Número de positivos: " + to_string(pCount));

    printMessage("Valor mínimo: " + to_string(minValue));
    printMessage("Média negativa: " + to_string(nAverage));
    printMessage("Número de negativos: " + to_string(nCount));

    *threshold = pAverage * 1.3;

    //print("Número de corners: " + to_string(cCount));
}

float* getGroundTruth(string groundTruthFile) {
    ifstream file(groundTruthFile);
    string groundTruth = "";
    string groundTruthLine = "";
    while (getline(file, groundTruthLine)) {
        groundTruth += groundTruthLine;
    }
    stringstream test(groundTruth);
    string segment;
    vector<float> seglist;

    while (getline(test, segment, ','))
    {
        seglist.push_back(stoi(segment));
    }

    int nx = seglist[0];
    int ny = seglist[1];

    float* out = (float*)malloc(nx * ny * sizeof(float));

    for (int i = 2; i < seglist.size(); i++) {
        out[i - 2] = seglist[i];
    }

    return out;
}