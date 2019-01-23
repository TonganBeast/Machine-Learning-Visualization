#pragma once
#include "Plane.h"
#include <iostream>
#include <time.h>
#include <string>
#include <fstream>

class MD_Graph {
private:
	int numDimensions, numPlanes, numMD_Points;
	bool reuseDimension = false;
	int** dimCombos;
	float** points;
	Plane *planes;
	int calcNumPlanes(int);
	void setupDimCombos();
	void combineDimensions();
	void dimSwap(int*, int*);
	bool randomizeDimensions(int*, int);
	void readData(std::string);
	void initPoints(std::string);
	void findNumDimensions(std::string);
	void findNumPoints(std::string);
public:
	MD_Graph(std::string);
	void buildGraph();
	void printPlanePts(int);
};