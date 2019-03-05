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
	std::vector<std::vector<int>> dimCombos;
	std::vector<std::vector<float>> pts;
	std::vector<Classification> uniqueClassifications;
	std::vector<Classification> classificationForPoints;
	std::vector<Plane> planes;
	int calcNumPlanes(int);
	std::vector<std::vector<int>> combineDimensions(int, int);
	void dimSwap(int*, int*);
	bool randomizeDimensions(int*, int);
	std::vector<std::string> readData_v(std::string);
	std::vector<std::vector<float>> parseData(std::vector<std::string>, int);
	void initPoints(std::string);
	int findNumDimensions(std::string);
	int findNumPoints(std::string);
	void findClassifications(std::string, int);
	bool alreadyClassified(std::string);
	void updatePtsClassification();
public:
	//default constructor
	MD_Graph();
	//constructor which builds everything from a file
	MD_Graph(std::string);
	//constructor which accepts all elements as an argument
	MD_Graph(int, int, int, bool, std::vector<std::vector<int>>, std::vector<std::vector<float>>,
		std::vector<Classification>, std::vector<Classification>, std::vector<Plane>);
	//copy constructor
	MD_Graph(const MD_Graph&);
	//copy assignment
	MD_Graph& operator=(const MD_Graph&);
	//move assignment
	MD_Graph operator=(MD_Graph&&);
	void buildGraph();
	void buildGraph(std::vector<Plane>, int, int);
	void printPlanePts(int);
	void associateColors();
	void drawDominantRectangle(Point, Point, Point, Point);
	//~MD_Graph();
};