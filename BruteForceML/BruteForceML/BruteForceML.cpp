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
//BruteForce::BruteForce(vector<DataClass> classesSet) : classes(classesSet){}
BruteForce::BruteForce(string nameOfFile) : filename(nameOfFile)
{
	initializeBruteForce();
}


// --------------------------------------
// -------------- METHODS ---------------
// --------------------------------------
void BruteForce::initializeBruteForce()
{
	readFile();
}

// ********************************************************************************
// ********************************************************************************
void BruteForce::readFile()
{
	// Read from file.
	string filename = "iris-data.txt";

	// Open the file an make sure it exists.
	ifstream inputFile(filename);
	if (inputFile.fail())
	{
		cout << "Failed to open file: " << filename << endl;
		return;
	}

	// Read the file and parse the integers into numList.
	DataClass classData = DataClass();
	int classID = -1;
	string classname = "";
	string currentClassName;
	string line;
	while (!inputFile.eof())
	{
		// Read line from file and make sure it has something.
		getline(inputFile, line);
		if (line.length() == 0)
			break;

		// Tokenize the line.
		istringstream stream(line);
		vector<string> tokenized;
		string token;
		while (getline(stream, token, ','))
		{
			tokenized.push_back(token);
		}

		// Update the current class name.
		currentClassName = tokenized[tokenized.size() - 1];

		// If the current class name is different than the previous ones.
		if (classname.compare(currentClassName) != 0)
		{
			// Update the classname and classID.
			classname = currentClassName;
			classID++;

			// Make a new DataClass struct to hold class info.
			classData = DataClass();

			// Push the new class struct into the classes vector list.
			classes.push_back(classData);

			// Set the name of the new class in DataClass struct.
			classes[classID].className = classname;
		}

		// Extract data from tokenized string.
		vector<float> data;
		for (int i = 0; i < tokenized.size() - 1; i++)
		{
			data.push_back(stod(tokenized[i]));
		}

		// Add new data to the classes dataset.
		classes[classID].classDataset.push_back(data);
	}

	// CLose input file.
	inputFile.close();

	return;
}

// ********************************************************************************
// ********************************************************************************
vector<MLPlane> BruteForce::run()
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
		cout << "****************************************************\n";
		cout << "****** Plane Set " << i << endl;
		cout << "****************************************************\n";
		PlaneSet planesSt = setOfPlanes[i];
		for (int j = 0; j < planesSt.planes.size(); j++)
		{
			MLPlane pln = planesSt.planes[j];
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
			cout << "x\ty" << endl;
			for (int i = 0; i < pln.xCoordinates.size() && i < pln.yCoordinates.size(); i++)
			{
				cout << pln.xCoordinates[i] << "\t" << pln.yCoordinates[i] << endl;
			}
			cout << "\n*****************\n\n";
		}
		cout << "****************************************************\n\n";
	}

	// *********************************************
	// DEBUGGING FOR VISUALIZATION FOR INTEGRATION (DELETE LATER)
	// *********************************************
	vector<MLPlane> dummyPlanes = GenerateDummyDominantPlanes();
	cout << "\n\n\n\n\n\n";
	cout << "DUMMY PLANES\n\n";
	for (int i = 0; i < dummyPlanes.size(); i++)
	{
		cout << "Plane: " << i << endl;
		cout << "******************************\n";
		MLPlane dumPlane = dummyPlanes[i];
		for (int dS = 0; dS < dumPlane.domSquares.size(); dS++)
		{
			cout << "Dominant Square: " << dS << endl;
			cout << "CLASSNAME: " << dumPlane.domSquares[dS].dominantClass << endl;
			cout << "[" << dumPlane.domSquares[dS].pointTL.x << " , " << dumPlane.domSquares[dS].pointTL.y << "]  ";
			cout << "[" << dumPlane.domSquares[dS].pointTR.x << " , " << dumPlane.domSquares[dS].pointTR.y << "]  ";
			cout << "[" << dumPlane.domSquares[dS].pointBR.x << " , " << dumPlane.domSquares[dS].pointBR.y << "]  ";
			cout << "[" << dumPlane.domSquares[dS].pointBL.x << " , " << dumPlane.domSquares[dS].pointBL.y << "]  \n";
			cout << "******************************\n";
		}
		cout << "\n*****************\n\n";
	}
	return dummyPlanes;
	// *********************************************

	//return dominantPlanes; // THIS IS THE ACTUAL RETURN VALUE DO NOT DELETE!!
}

// ********************************************************************************
// ********************************************************************************
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

// ********************************************************************************
// ********************************************************************************
// Get the number of planes possible. 
void BruteForce::getNumOfPlanes()
{
	int numOfPlanes = numOfAttributes / 2;
	if (numOfPlanes != 0 && numOfAttributes % 2 != 0)
		numOfPlanes++;

	numOfPlanesPossible = numOfPlanes;
}

// ********************************************************************************
// ********************************************************************************
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
			MLPlane currPlane;

			// FIRST ATTRUBUTE
			currPlane.attribute1 = k;	// Assign Attribute
			k++;

			// SECOND ATTRIBUTE
			// If it's the last plane to be made and there is an additional attribute.
			if (i == (numOfPlanesPossible - 1) && addAttribute != -1)
			{
				// Make sure that the additional attribute doesn't equal
				// the first attribute of the plane.
				while(addAttribute == currPlane.attribute1)
					addAttribute = rand() % numOfAttributes;

				currPlane.attribute2 = addAttribute;	// Assign Attribute
			}
			else
				currPlane.attribute2 = k;				// Assign Attribute
			k++;

			// Store correct x and y coordinates of 2D points that belong in this plane.
			// Loop through all the classes.
			for (int c = 0; c < classes.size(); c++)
			{

				DataClass tempClass = classes[c];
				for (int cD = 0; cD < tempClass.classDataset.size(); cD++)
				{
					// Loop through all the multidimensional points of every class.
					vector<float> data = tempClass.classDataset[cD];
					currPlane.xCoordinates.push_back(data[currPlane.attribute1]);
					currPlane.yCoordinates.push_back(data[currPlane.attribute2]);
				}
			}

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
			MLPlane currPlane;

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

			// Store correct x and y coordinates of 2D points that belong in this plane.
			// Loop through all the classes.
			for (int c = 0; c < classes.size(); c++)
			{

				DataClass tempClass = classes[c];
				for (int cD = 0; cD < tempClass.classDataset.size(); cD++)
				{
					// Loop through all the multidimensional points of every class.
					vector<float> data = tempClass.classDataset[cD];
					currPlane.xCoordinates.push_back(data[currPlane.attribute1]);
					currPlane.yCoordinates.push_back(data[currPlane.attribute2]);
				}
			}

			planesSt.planes.push_back(currPlane);
		}
	}

	// Insert into setOfPlanes.
	setOfPlanes.push_back(planesSt);
}

// ********************************************************************************
// ********************************************************************************
// Determine the limits of dominant squares.
void BruteForce::setDomSquareLimits()
{
	for (int i = 0; i < classes.size(); i++)
	{
		DataClass classData = classes[i];
		
		// For each Set of Planes.
		for (int ps = 0; ps < setOfPlanes.size(); ps++)
		{
			PlaneSet &planeSt = setOfPlanes[ps];

			// For each Plane.
			for (int p = 0; p < planeSt.planes.size(); p++)
			{
				MLPlane &pln = planeSt.planes[p];
				int attr1 = pln.attribute1;	// X
				int attr2 = pln.attribute2;	// Y

				// Initialize the class limits for dominant squares.
				ClassSquareLimits classLimits;
				classLimits.className = classData.className;

				float xMin = DBL_MAX;
				float xMax = DBL_MIN;
				float yMin = DBL_MAX;
				float yMax = DBL_MIN;

				// Determine the min/max X and Y for the two attributes.
				for (int d = 0; d < classData.classDataset.size(); d++)
				{
					vector<float> data = classData.classDataset[d];
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
				MLPoint tL, tR, bR, bL;
				tL.x = xMin;	// Top Left Corner.
				tL.y = yMax;
				tR.x = xMax;	// Top Right Corner.
				tR.y = yMax;
				bR.x = xMax;	// Bottom Right Corner.
				bR.y = yMin;
				bL.x = xMin;	// Bottom Left Corner.
				bL.y = yMin;

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

// ********************************************************************************
// ********************************************************************************
// Find the dominant squares in a Plane.
void BruteForce::findDominantSquares(MLPlane pln)
{

}

// ********************************************************************************
// ********************************************************************************
// Find a dominant set of planes with the best dominant squares.
void BruteForce::findDominantPlanes()
{

}

// ********************************************************************************
// ********************************************************************************
// Generates Random dominant squares for TESTING!!!
// FOR MATT/LENI INTEGRATION WITH VIS.
vector<MLPlane> BruteForce::GenerateDummyDominantPlanes()
{
	// NOTE: NO PURITY OR POINTS-IN-DOMINANT-SQUARE CALCULATIONS!!
	vector<MLPlane> dummyDominantPlanes = setOfPlanes[0].planes;

	for (int i = 0; i < dummyDominantPlanes.size(); i++)
	{
		MLPlane pln = dummyDominantPlanes[i];
		for (int j = 0; j < pln.limitsOfClasses.size(); j++)
		{
			ClassSquareLimits squLimits = pln.limitsOfClasses[j];

			// Dominant Square limits.
			float xMinLim = squLimits.pointBL.x;
			float xMaxLim = squLimits.pointBR.x;
			float yMinLim = squLimits.pointBL.y;
			float yMaxLim = squLimits.pointTL.y;
			
			// Generate a random dominant square.
			DominantSquare domSquare;
			domSquare.dominantClass = squLimits.className;	// Set classname.

			// Generate random x and y min/max points for dominant square.
			float xMin = xMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xMaxLim - xMinLim)));
			float xMax = xMin - 1;
			while (xMax <= xMin)
				xMax = xMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xMaxLim - xMinLim)));

			float yMin = yMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (yMaxLim - yMinLim)));
			float yMax = yMin - 1;
			while (yMax <= yMin)
				yMax = yMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (yMaxLim - yMinLim)));

			// Make Dominant square points.
			MLPoint tL, tR, bR, bL;
			tL.x = xMin;	// Top Left Corner.
			tL.y = yMax;
			tR.x = xMax;	// Top Right Corner.
			tR.y = yMax;
			bR.x = xMax;	// Bottom Right Corner.
			bR.y = yMin;
			bL.x = xMin;	// Bottom Left Corner.
			bL.y = yMin;

			// Set them in Dominant Square.
			domSquare.pointTL = tL;
			domSquare.pointTR = tR;
			domSquare.pointBR = bR;
			domSquare.pointBL = bL;

			// Add dominant square to Plane's dominant squares vector.
			pln.domSquares.push_back(domSquare);
		}

		dummyDominantPlanes[i] = pln; // Update the plane in dummyDominantPlanes.
	}

	return dummyDominantPlanes;
}