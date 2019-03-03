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
	string className;
	float x;
	float y;
	vector<bool> inDomSquare;
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
	double purity = 0;
	int classPointsInSquare = 0;
	int totalPointsInSquare = 0;

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
	vector<MLPoint> pointsInPlane; // The combined x,y coordinates.
	vector<string> allClassNames;

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
	// Read the file and initialize classes vector.
	void readFile();

	// Get the number of attributes.
	void getNumOfAttributes();

	// Get the number of planes possible. 
	void getNumOfPlanes();

	// Assign the planes based on the number of attributes.
	void assignPlanes(); // Make a PlaneSet and insert into setOfPlanes.

	// Determine the limits of dominant squares.
	void setDomSquareLimits();

	void calculatePurity(DominantSquare &box, MLPlane plane);

	// Find the dominant squares in a Plane.
	void findDominantSquares(MLPlane pln);

	// Find a dominant set of planes with the best dominant squares.
	void findDominantPlanes();

	// *****************************************************
	// ************** GENETIC ALGORITHM PART ***************
	// *****************************************************
	// The main function of the Genetic Algorithm.
	void geneticAlgorithm();

	// Generates a population of dominant squares within the limits of the squareLimits classname.
	vector<DominantSquare> generateRandomDominantSquares(int numOfSquares, ClassSquareLimits squareLimits);

	// Calculates the purity of the population of dominant squares.
	void fitness(vector<DominantSquare> &domSquares, MLPlane plane);

	// Returns the average purity of the whole population of dominant squares.
	float gradeDominantSquares(vector<DominantSquare> &domSquares, MLPlane plane);

	// Sorts the population (highest purity first, lowest purity last).
	void sortTheDomSquarePop(vector<DominantSquare> &domSquares);

	// Mutate one of the retained squares. (Can mutate left/right/top/bottom sides.)
	void mutateRetainedSquare(DominantSquare &retainedSquare, ClassSquareLimits squareLimits);

	// Crossover from the parent.
	void crossoverFromParent(DominantSquare &retainedSquare, vector<DominantSquare> parents);

	// Evolves the dominant squares (select, mutate, crossover).
	vector<DominantSquare> evolveSquares(vector<DominantSquare> &domSquares, ClassSquareLimits squareLimits, float retain = 0.2, float select = 0.05, float mutate = 0.5);
	
	// ************ QUICKSORT IMPLEMENTATION ************
	// NOTE: L and R are INCLUSIVE!!
	// Example Initial Call: quicksort(domSquares, 0, domSquares.size()-1);
	void quicksort(vector<DominantSquare> &domSquares, int L, int R, bool reverse);
	void swap(vector<DominantSquare> &domSquare, int x, int y);
	// *****************************************************
	// *****************************************************

	// Generates Random dominant squares for TESTING!!!
	// FOR MATT/LENI INTEGRATION WITH VIS.
	vector<MLPlane> GenerateDummyDominantPlanes();
};