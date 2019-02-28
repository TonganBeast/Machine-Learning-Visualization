#pragma once
/*
Author:	Al Timofeyev
Date:	2/21/2019
Desc:	A brute force attempt to identify Dominant Square
with the best (highest) purity level.
*/

#include <climits>
#include <string>
#include <vector> 
#include <ctime>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

struct DataClass
{
	string className;
	vector<vector<float>> classDataset;
};

struct MLPoint
{
	float x;
	float y;
};

struct ClassSquareLimits
{
	string className;

	// Points of dominant square: Top Left, Top Right,
	// Bottom Right, Bottom Left.
	MLPoint pointTL, pointTR, pointBR, pointBL;
};

struct DominantSquare
{
	string dominantClass;
	double purity;
	int classPointsInSquare;
	int totalPointsInSquare;

	// Points of dominant square: Top Left, Top Right,
	// Bottom Right, Bottom Left.
	MLPoint pointTL, pointTR, pointBR, pointBL;
};

struct MLPlane
{
	// These are indexes to the attribute in
	// classDataset of DataClass struct. (DIMENSIONS OF PLANE)
	int attribute1;	// X
	int attribute2;	// Y
	vector<DominantSquare> domSquares;

	// Basically, all the points in the plane.
	vector<float> xCoordinates;	// All the x coordinates in plane.
	vector<float> yCoordinates;	// All the y coordinates in plane.

	// Only used to find dominant squares.
	vector<ClassSquareLimits> limitsOfClasses;
};

struct PlaneSet
{
	vector<MLPlane> planes;
};

class BruteForce
{
public:
	// --------------------------------------
	// ------------ CONSTRUCTORS ------------
	// --------------------------------------
	//BruteForce(vector<DataClass> classesSet);
	BruteForce(string filename);

	// --------------------------------------
	// -------------- METHODS ---------------
	// --------------------------------------
	void initializeBruteForce();
	vector<MLPlane> run();

private:
	string filename;				// Name of file with data;
	vector<DataClass> classes;		// The organized class data of all classes.
	vector<MLPlane> dominantPlanes;	// The dominant planes produced by Brute Force.
	vector<PlaneSet> setOfPlanes;	// Sets of planes (will be limited to 3 or 4).
	vector<DominantSquare> dominantSquares;
	int numOfAttributes;
	int numOfPlanesPossible;

	// --------------------------------------
	// -------------- METHODS ---------------
	// --------------------------------------
	void readFile();
	// Get the number of attributes.
	void getNumOfAttributes();

	// Get the number of planes possible. 
	void getNumOfPlanes();

	// Assign the planes based on the number of attributes.
	void assignPlanes(); // Make a PlaneSet and insert into setOfPlanes.

	// Determine the limits of dominant squares.
	void setDomSquareLimits();

	// Find the dominant squares in a Plane.
	void findDominantSquares(MLPlane pln);

	// Find a dominant set of planes with the best dominant squares.
	void findDominantPlanes();

	// Generates Random dominant squares for TESTING!!!
	// FOR MATT/LENI INTEGRATION WITH VIS.
	vector<MLPlane> GenerateDummyDominantPlanes();
};