#pragma once
#include "Plane.h"
#include "Classification.h"
#include <iostream>
#include <time.h>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>

#define CLASS_RED	1.0f, 0.0f, 0.0f
#define CLASS_GREEN 0.0f, 1.0f, 0.0f
#define CLASS_BLUE	0.0f, 0.0f, 1.0f

class MD_Graph {
private:
	int numDimensions, numPlanes, numMD_Points;
	bool reuseDimension = false;
	int** dimCombos;
	float** points;
	std::vector<std::vector<float>> pts;
	std::vector<Classification> uniqueClassifications;
	std::vector<Classification> classificationForPoints;
	Plane *planes;
	int calcNumPlanes(int);
	std::vector<std::vector<int>> combineDimensions(int, int);
	void dimSwap(int*, int*);
	bool randomizeDimensions(int*, int);
	void readData(std::string);
	std::vector<std::string> readData_v(std::string);
	std::vector<std::vector<float>> parseData(std::vector<std::string>, int);
	void initPoints(std::string);
	int findNumDimensions(std::string);
	int findNumPoints(std::string);
	void findClassifications(std::string, int);
	bool alreadyClassified(std::string);
	void updatePtsClassification();
public:
	std::string *classificationList;
	MD_Graph(std::string);
	void buildGraph();
	void buildGraph(std::vector<Plane>, int, int);
	void printPlanePts(int);
	void associateColors();
	//~MD_Graph();
};