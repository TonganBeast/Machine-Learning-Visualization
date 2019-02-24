/*
Author:	Al Timofeyev
Date:	2/21/2019
Desc:	A brute force attempt to identify Dominant Square
with the best (highest) purity level.
*/


#include "BruteForceML.h"

#include <iostream> // For Debugginng.
#include <string>
#include <vector>
#include<ctime>

using namespace std;

// --------------------------------------
// ------------ CONSTRUCTORS ------------
// --------------------------------------
BruteForce::BruteForce(vector<DataClass> classesSet) : classes(classesSet) {}


// --------------------------------------
// -------------- METHODS ---------------
// --------------------------------------
vector<Plane> BruteForce::run()
{
	srand(time(0)); // Initalize rand() with random seed.

	getNumOfAttributes();
	cout << "\nNum Of Attributes: " << numOfAttributes << "\n\n";

	// Generate some planes.
	for(int i = 0; i < 4; i++)
		assignPlanes();

	// DEBUGGING.
	for (int i = 0; i < setOfPlanes.size(); i++)
	{
		PlaneSet planesSt = setOfPlanes[i];
		for (int j = 0; j < planesSt.planes.size(); j++)
		{
			Plane pln = planesSt.planes[j];
			cout << "Plane " << j << ":\n";
			cout << "Attribute 1 = " << pln.attribute1 << endl;
			cout << "Attribute 2 = " << pln.attribute2 << "\n\n";
		}
		cout << "****************************************************\n\n";
	}

	return dominantPlanes;
}

// Get the number of attributes.
void BruteForce::getNumOfAttributes()
{
	DataClass classData;
	int numOfAttr = classes[0].classDataset[0].size();
	int currNumOfAttr;

	for (int i = 0; i < classes.size(); i++)
	{
		classData = classes[i];
		currNumOfAttr = classData.classDataset[0].size();
		if (currNumOfAttr < numOfAttr)
			numOfAttr = currNumOfAttr;
	}

	numOfAttributes = numOfAttr;
}

// Assign the planes based on the number of attributes.
void BruteForce::assignPlanes()
{
	int addAttribute = -1; // Additional attribute in case of odd attribures.
	if (numOfAttributes % 2 != 0)
	{
		// The additional attribute index will be
		// between 0 and numOfAttributes.
		addAttribute = rand() % numOfAttributes;
		numOfAttributes++;
	}

	PlaneSet planesSt;

	// First set of planes must pair the attributes the way it is in file.
	if (setOfPlanes.size() == 0)
	{
		for (int i = 0, k = 0; i < (numOfAttributes / 2); i++)
		{
			Plane currPlane;
			currPlane.attribute1 = k;
			k++;
			if (i == ((numOfAttributes / 2) - 1) && addAttribute != -1)
				currPlane.attribute2 = addAttribute;
			else
				currPlane.attribute2 = k;
			k++;

			planesSt.planes.push_back(currPlane);
		}
	}

	// If not the first set of Planes, then randomize attribute pairs.
	else
	{
		vector<int> alreadyChosen;
		int k = rand() % (numOfAttributes - 1);
		for (int i = 0; i < (numOfAttributes / 2); i++)
		{
			Plane currPlane;

			// Find attribute that hasn't been chosen yet.
			while (find(alreadyChosen.begin(), alreadyChosen.end(), k) != alreadyChosen.end())
			{
				// If there was an additional attribute.
				if (addAttribute != -1)
					k = rand() % (numOfAttributes - 1);
				else
					k = rand() % numOfAttributes;
			}
			alreadyChosen.push_back(k);

			currPlane.attribute1 = k;	// Assign Attribute

			// Find attribute that hasn't been chosen yet.
			while (find(alreadyChosen.begin(), alreadyChosen.end(), k) != alreadyChosen.end())
			{
				// If there was an additional attribute.
				if (addAttribute != -1)
					k = rand() % (numOfAttributes - 1);
				else
					k = rand() % numOfAttributes;
			}
			alreadyChosen.push_back(k);

			// If it's the last plane to be made and there is an additional attribute.
			if (i == ((numOfAttributes / 2) - 1) && addAttribute != -1)
				currPlane.attribute2 = addAttribute;	// Assign Attribute
			else
				currPlane.attribute2 = k;				// Assign Attribute

			planesSt.planes.push_back(currPlane);
		}
	}

	// Insert into setOfPlanes.
	setOfPlanes.push_back(planesSt);
}

// Find the dominant squares in a Plane.
void BruteForce::findDominantSquares(Plane pln)
{

}

// Find a dominant set of planes with the best dominant squares.
void BruteForce::findDominantPlanes()
{

}