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
	double purity;
	int classPointsInSquare;

	// Points of dominant square: Top Left, Top Right,
	// Bottom Right, Bottom Left.
	Point pointTL, pointTR, pointBR, pointBL;
};

struct Plane
{
	// These are indexes to the attribute in
	// classDataset of DataClass struct.
	int attribute1;
	int attribute2;
	vector<DominantSquare> domSquares;
};

struct PlaneSet
{
	vector<Plane> planes;
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
	vector<Plane> run();

private:
	vector<DataClass> classes;		// The organized class data of all classes.
	vector<Plane> dominantPlanes;	// The dominant planes produced by Brute Force.
	vector<PlaneSet> setOfPlanes;	// Sets of planes (will be limited to 3).
	vector<DominantSquare> dominantSquares;
	int numOfAttributes;
	int numOfPlanesPossible;

	// --------------------------------------
	// -------------- METHODS ---------------
	// --------------------------------------
	// Get the number of attributes.
	void getNumOfAttributes();

	// Get the number of planes possible. 
	void getNumOfPlanes();

	// Assign the planes based on the number of attributes.
	void assignPlanes(); // Make a PlaneSet and insert into setOfPlanes.

	// Find the dominant squares in a Plane.
	void findDominantSquares(Plane pln);

	// Find a dominant set of planes with the best dominant squares.
	void findDominantPlanes();
};