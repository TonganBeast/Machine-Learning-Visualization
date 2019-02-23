/*
Author:	Al Timofeyev
Date:	2/21/2019
Desc:	A brute force attempt to identify Dominant Square
with the best (highest) purity level.
*/


#include "BruteForceML.h"

#include <string>
#include <vector>

using namespace std;

// --------------------------------------
// ------------ CONSTRUCTORS ------------
// --------------------------------------
BruteForce::BruteForce(vector<DataClass> classesSet) : classes(classesSet) {}


// --------------------------------------
// -------------- METHODS ---------------
// --------------------------------------
vector<DominantSquare> BruteForce::run()
{
	return dominantSquares;
}