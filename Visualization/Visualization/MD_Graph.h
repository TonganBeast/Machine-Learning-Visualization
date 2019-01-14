#pragma once
#include "Plane.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

class MD_Graph {
private:
	int numDimensions, numPlanes, numMD_Points;
	bool reuseDimension = false;
	int** dimCombos;
	float** points;
	int calcNumPlanes(int);
	void combineDimensions();
	bool axisIsAvailable(int);
	void readData(std::string);
	void initPoints(std::string);
	void findNumDimensions(std::string);
	void findNumPoints(std::string);
public:
	MD_Graph(std::string);

};