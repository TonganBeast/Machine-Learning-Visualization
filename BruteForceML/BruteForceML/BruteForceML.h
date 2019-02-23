#pragma once
/*
Author:	Al Timofeyev
Date:	2/21/2019
Desc:	A brute force attempt to identify Dominant Square
with the best (highest) purity level.
*/

#include <string>
#include <vector>

using namespace std;

struct DataClass
{
	string className;
	vector<vector<double>> classDataset;
};

struct Point
{
	int x;
	int y;
};

struct DominantSquare
{
	string dominantClass;
	int xMin, xMax;
	int yMin, yMax;
};

class BruteForce
{
public:
	// --------------------------------------
	// ------------ CONSTRUCTORS ------------
	// --------------------------------------
	BruteForce(vector<DataClass> classesSet);

	// --------------------------------------
	// -------------- METHODS ---------------
	// --------------------------------------
	vector<DominantSquare> run();

private:
	vector<DataClass> classes;
	vector<DominantSquare> dominantSquares;

	// --------------------------------------
	// -------------- METHODS ---------------
	// --------------------------------------

};