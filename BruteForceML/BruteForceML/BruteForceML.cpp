/*
Author:	Al Timofeyev
Date:	2/21/2019
Desc:	A brute force attempt to identify Dominant Square
with the best (highest) purity level.
*/


#include "BruteForceML.h"

#include <iostream> // For Debugginng.

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
	//numOfAttributes = -1;
	cout << "\nNum Of Attributes: " << numOfAttributes << endl;
	getNumOfPlanes();
	cout << "Num Of Planes: " << numOfPlanesPossible << "\n\n";

	// Can't really do anything with 1 attribute.
	if(numOfAttributes < 2 || numOfPlanesPossible < 1)
		return dominantPlanes;

	// Generate some planes.
	for(int i = 0; i < 4; i++)
		assignPlanes();

	// Set the limits of the dominant squares
	// for each class in each plane.
	setDomSquareLimits();

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

			cout << "DomSquare Limits: " << pln.limitsOfClasses.size() << "\n\n";
			
			for (int i = 0; i < pln.limitsOfClasses.size(); i++)
			{
				ClassSquareLimits limits = pln.limitsOfClasses[i];
				cout << "CLASS: ";
				cout << limits.className << endl;
				cout << "[" << limits.pointTL.x << " , " << limits.pointTL.y << "]  ";
				cout << "[" << limits.pointTR.x << " , " << limits.pointTR.y << "]  ";
				cout << "[" << limits.pointBR.x << " , " << limits.pointBR.y << "]  ";
				cout << "[" << limits.pointBL.x << " , " << limits.pointBL.y << "]  \n";
			}
			cout << "\n*****************\n\n";
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

// Get the number of planes possible. 
void BruteForce::getNumOfPlanes()
{
	int numOfPlanes = numOfAttributes / 2;
	if (numOfPlanes != 0 && numOfAttributes % 2 != 0)
		numOfPlanes++;

	numOfPlanesPossible = numOfPlanes;
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
	}

	PlaneSet planesSt;

	// First set of planes must pair the attributes the way it is in file.
	if (setOfPlanes.size() == 0)
	{
		for (int i = 0, k = 0; i < numOfPlanesPossible; i++)
		{
			Plane currPlane;

			// FIRST ATTRUBUTE
			currPlane.attribute1 = k;	// Assing Attribute
			k++;

			// SECOND ATTRIBUTE
			// If it's the last plane to be made and there is an additional attribute.
			if (i == (numOfPlanesPossible - 1) && addAttribute != -1)
			{
				// Make sure that the additional attribute doesn't equal
				// the first attribute of the plane.
				while(addAttribute == currPlane.attribute1)
					addAttribute = rand() % numOfAttributes;

				currPlane.attribute2 = addAttribute;	// Assing Attribute
			}
			else
				currPlane.attribute2 = k;				// Assing Attribute
			k++;

			planesSt.planes.push_back(currPlane);
		}
	}

	// If not the first set of Planes, then randomize attribute pairs.
	else
	{
		vector<int> alreadyChosen;
		int k = rand() % numOfAttributes;
		for (int i = 0; i < numOfPlanesPossible; i++)
		{
			Plane currPlane;

			// FIRST ATTRIBUTE
			// Find attribute that hasn't been chosen yet.
			while (find(alreadyChosen.begin(), alreadyChosen.end(), k) != alreadyChosen.end())
				k = rand() % numOfAttributes;
			alreadyChosen.push_back(k);

			currPlane.attribute1 = k;	// Assign Attribute

			// SECOND ATTRIBUTE
			// If it's the last plane to be made and there is an additional attribute.
			if (i == (numOfPlanesPossible - 1) && addAttribute != -1)
			{
				// Make sure that the additional attribute doesn't equal
				// the first attribute of the plane.
				while (addAttribute == currPlane.attribute1)
					addAttribute = rand() % numOfAttributes;
				currPlane.attribute2 = addAttribute;	// Assign Attribute
			}
			else
			{
				while (find(alreadyChosen.begin(), alreadyChosen.end(), k) != alreadyChosen.end())
					k = rand() % numOfAttributes;
				alreadyChosen.push_back(k);

				currPlane.attribute2 = k;				// Assign Attribute
			}

			planesSt.planes.push_back(currPlane);
		}
	}

	// Insert into setOfPlanes.
	setOfPlanes.push_back(planesSt);
}

// Determine the limits of dominant squares.
void BruteForce::setDomSquareLimits()
{
	for (int i = 0; i < classes.size(); i++)
	{
		DataClass classData = classes[i];

		double xMin = DBL_MAX;
		double xMax = DBL_MIN;
		double yMin = DBL_MAX;
		double yMax = DBL_MIN;
		
		// For each Set of Planes.
		for (int ps = 0; ps < setOfPlanes.size(); ps++)
		{
			PlaneSet &planeSt = setOfPlanes[ps];

			// For each Plane.
			for (int p = 0; p < planeSt.planes.size(); p++)
			{
				Plane &pln = planeSt.planes[p];
				int attr1 = pln.attribute1;	// X
				int attr2 = pln.attribute2;	// Y

				// Initialize the class limits for dominant squares.
				ClassSquareLimits classLimits;
				classLimits.className = classData.className;

				// Determine the min/max X and Y for the two attributes.
				for (int d = 0; d < classData.classDataset.size(); d++)
				{
					vector<double> data = classData.classDataset[d];
					if (data[attr1] < xMin) // Determine xMin.
						xMin = data[attr1];
					if (data[attr1] > xMax) // Determine xMax.
						xMax = data[attr1];

					if (data[attr2] < yMin) // Determine yMin.
						yMin = data[attr2];
					if (data[attr2] > yMax) // Determine yMax.
						yMax = data[attr2];
				}

				// Set the coordinate Points.
				Point tL, tR, bR, bL;
				tL.x = xMin-10.0;	// Top Left Corner.
				tL.y = yMax+10.0;
				tR.x = xMax+10.0;	// Top Right Corner.
				tR.y = yMax+10.0;
				bR.x = xMax+10.0;	// Bottom Right Corner.
				bR.y = yMin-10.0;
				bL.x = xMin-10.0;	// Bottom Left Corner.
				bL.y = yMin-10.0;

				// Assign coordinates to classLimits.
				classLimits.pointTL = tL;
				classLimits.pointTR = tR;
				classLimits.pointBR = bR;
				classLimits.pointBL = bL;

				// Add the classLimits to this Planes list of class limits.
				pln.limitsOfClasses.push_back(classLimits);
			}
		}
	}
}

// Find the dominant squares in a Plane.
void BruteForce::findDominantSquares(Plane pln)
{

}

// Find a dominant set of planes with the best dominant squares.
void BruteForce::findDominantPlanes()
{

}