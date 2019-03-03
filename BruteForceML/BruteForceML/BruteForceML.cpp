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
	readFile();
}


// --------------------------------------
// -------------- METHODS ---------------
// --------------------------------------
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

	// Initiate the genetic algorithm to find dominant squares
	// of each class in each plane.
	geneticAlgorithm();

	// DEBUGGING.
	for (int sP = 0; sP < setOfPlanes.size(); sP++)
	{
		cout << "PLANE SET: " << sP << endl;
		for (int p = 0; p < setOfPlanes[sP].planes.size(); p++)
		{
			cout << "Plane: " << p << endl;
			cout << "Number of Dominant Squares: " << setOfPlanes[sP].planes[p].domSquares.size() << endl;
			cout << "Dom Square Classes: ";
			for (int dM = 0; dM < setOfPlanes[sP].planes[p].domSquares.size(); dM++)
			{
				cout << setOfPlanes[sP].planes[p].domSquares[dM].dominantClass << "\t\t";
			}
			cout << "\n\n";
		}
		cout << "****************************************************\n\n";
	}

	
	// ******************************************************************************
	// DEBUGGING FOR VISUALIZATION FOR INTEGRATION (DELETE LATER) (MATT/LENI PART)
	// ******************************************************************************
	vector<MLPlane> dummyPlanes = GenerateDummyDominantPlanes();
	return dummyPlanes;
	// ******************************************************************************
	
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
				// Add the class name to the list of classnames for the plane.
				currPlane.allClassNames.push_back(classes[c].className);

				DataClass tempClass = classes[c];
				for (int cD = 0; cD < tempClass.classDataset.size(); cD++)
				{
					// Loop through all the multidimensional points of every class.
					vector<float> data = tempClass.classDataset[cD];
					currPlane.xCoordinates.push_back(data[currPlane.attribute1]);
					currPlane.yCoordinates.push_back(data[currPlane.attribute2]);

					// Make a point as well with combined x,y values.
					MLPoint point;
					point.className = tempClass.className;
					point.x = data[currPlane.attribute1];
					point.y = data[currPlane.attribute2];
					currPlane.pointsInPlane.push_back(point); // Store the point in plane.
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
				// Add the class name to the list of classnames for the plane.
				currPlane.allClassNames.push_back(classes[c].className);

				DataClass tempClass = classes[c];
				for (int cD = 0; cD < tempClass.classDataset.size(); cD++)
				{
					// Loop through all the multidimensional points of every class.
					vector<float> data = tempClass.classDataset[cD];
					currPlane.xCoordinates.push_back(data[currPlane.attribute1]);
					currPlane.yCoordinates.push_back(data[currPlane.attribute2]);

					// Make a point as well with combined x,y values.
					MLPoint point;
					point.className = tempClass.className;
					point.x = data[currPlane.attribute1];
					point.y = data[currPlane.attribute2];
					currPlane.pointsInPlane.push_back(point); // Store the point in plane.
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

				// Set classname for all coordinate points.
				tL.className = classData.className;
				tR.className = classData.className;
				bR.className = classData.className;
				bL.className = classData.className;

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
void BruteForce::calculatePurity(DominantSquare &box, MLPlane plane)
{
	//int count = 0;
	//int countDom = 0;

	float xMin = box.pointBL.x;
	float xMax = box.pointBR.x;
	float yMin = box.pointBL.y;
	float yMax = box.pointTL.y;

	// Reset the totalPointsInSquare and classPointsInSquare of the dominant square.
	box.totalPointsInSquare = 0;
	box.classPointsInSquare = 0;

	// Calculate total points in dominant square and points from class in dominant square.
	for (int p = 0; p < plane.pointsInPlane.size(); p++)
	{
		MLPoint tempPoint = plane.pointsInPlane[p];
		float x = tempPoint.x;
		float y = tempPoint.y;
		if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
		{
			//count++;
			box.totalPointsInSquare++;
			if (tempPoint.className.compare(box.dominantClass) == 0)
			{
				//countDom++;
				box.classPointsInSquare++;
			}
		}
	}

	//box.totalPointsInSquare = count;
	//box.classPointsInSquare = countDom;

	// Calculate dominant square purity.
	if (box.totalPointsInSquare > 0)
		box.purity = 100 * (box.classPointsInSquare / (double)box.totalPointsInSquare);
	else
		box.purity = 0;

	// Normalize purity based on how useful it is.
	for (int i = 0; i < classes.size(); i++)
	{
		if (box.dominantClass.compare(classes[i].className))
		{
			if (box.classPointsInSquare < (classes[i].classDataset.size() * 0.85))
				box.purity = box.purity * box.classPointsInSquare / (double)classes[i].classDataset.size();
			break;
		}
	}
}

// ********************************************************************************
// ********************************************************************************
// Find the dominant squares in a Plane.
void BruteForce::findDominantSquares(MLPlane pln)
{
	for (int i = 0; i < pln.limitsOfClasses.size(); i++)
	{
		DominantSquare box;
		box.pointTL = pln.limitsOfClasses[i].pointTL;
		box.pointTR = pln.limitsOfClasses[i].pointTR;
		box.pointBR = pln.limitsOfClasses[i].pointBR;
		box.pointBL = pln.limitsOfClasses[i].pointBL;

		box.dominantClass = pln.limitsOfClasses[i].className;

		int count = 0;
		calculatePurity(box, pln);

		while (box.purity < 80 && box.classPointsInSquare > 0)
		{
			cout << box.purity << endl;
			count = count % 4;
			switch (count)
			{
				//shrink the DomSq left
			case 0:
				box.pointTL.x = (box.pointTR.x - box.pointTL.x) / 10 + box.pointTL.x;
				box.pointBL.x = (box.pointBR.x - box.pointBL.x) / 10 + box.pointBL.x;
				count++;
				calculatePurity(box, pln);
				break;
				//shrink the DomSq down
			case 1:
				box.pointTL.y = (box.pointBL.y - box.pointTL.y) / 10 + box.pointTL.y;
				box.pointTR.y = (box.pointBR.y - box.pointTR.y) / 10 + box.pointTR.y;
				count++;
				calculatePurity(box, pln);
				break;
				//shrink the DomSq right
			case 2:
				box.pointTR.x = (box.pointTL.x - box.pointTR.x) / 10 + box.pointTR.x;
				box.pointBR.x = (box.pointBL.x - box.pointBR.x) / 10 + box.pointBR.x;
				count++;
				calculatePurity(box, pln);
				break;
				//shrink the DomSq up
			case 3:
				box.pointTL.y = (box.pointBL.y - box.pointTL.y) / 10 + box.pointTL.y;
				box.pointTR.y = (box.pointBR.y - box.pointTR.y) / 10 + box.pointTR.y;
				count++;
				calculatePurity(box, pln);
				break;
			}//end switch
			cout << "Class Points In Square: " << box.classPointsInSquare << "\t\t";
			cout << "Total Points In Square: " << box.totalPointsInSquare << "\t\t";
			cout << "Total Points In Plane: " << pln.pointsInPlane.size() << endl;
			
			cout << "Top Left:  [" << box.pointTL.x << " , " << box.pointTL.y << "]\t\t";
			cout << "Top Right: [" << box.pointTR.x << " , " << box.pointTR.y << "]\n";
			cout << "Bot Left:  [" << box.pointBL.x << " , " << box.pointBL.y << "]\t\t";
			cout << "Bot Right: [" << box.pointBR.x << " , " << box.pointBR.y << "]\n\n";
			
		}//end while
	}//end for
}//end function

// ********************************************************************************
// ********************************************************************************
// Find a dominant set of planes with the best dominant squares.
void BruteForce::findDominantPlanes()
{

}


// ********************************************************************************
// **************************** GENETIC ALGORITHM PART ****************************
// ********************************************************************************
// The main function of the Genetic Algorithm.
void BruteForce::geneticAlgorithm()
{
	for (int sP = 0; sP < setOfPlanes.size(); sP++)
	{
		for (int p = 0; p < setOfPlanes[sP].planes.size(); p++)
		{
			for (int lim = 0; lim < setOfPlanes[sP].planes[p].limitsOfClasses.size(); lim++)
			{
				// Create the population of dominant squares based on the limits of classes.
				vector<DominantSquare> domSquares = generateRandomDominantSquares(25, setOfPlanes[sP].planes[p].limitsOfClasses[lim]);
				cout << "Population Size: " << domSquares.size() << endl;

				// Grade the original population and get its average purity.
				float avgPurity = gradeDominantSquares(domSquares, setOfPlanes[0].planes[0]);

				// Set the generations counter;
				int generations = 0;

				// Evolve the population until the average purity is 90 or greater.
				while (avgPurity < 90 && generations < 500)
				{
					domSquares = evolveSquares(domSquares, setOfPlanes[0].planes[0].limitsOfClasses[0]);
					avgPurity = gradeDominantSquares(domSquares, setOfPlanes[0].planes[0]);
					generations++;
				}

				// Save the topmost pure dominant square into the list of dominant squares for the plane.
				sortTheDomSquarePop(domSquares);
				setOfPlanes[sP].planes[p].domSquares.push_back(domSquares[0]);

				// DEBUGGING
				/*
				cout << "Average Purity: " << avgPurity << endl;
				for (int i = 0; i < domSquares.size(); i++)
				{
					cout << "Purity: " << domSquares[i].purity << "\t\t";
					cout << "Class Points In Square: " << domSquares[i].classPointsInSquare << "\t\t";
					cout << "Total Points In Square: " << domSquares[i].totalPointsInSquare << endl;
				}
				cout << "\n*************************************************************\n\n";
				system("pause");
				*/
			}
		}
	}
}

// ********************************************************************************
// Generates a population of dominant squares within the limits of the squareLimits classname.
vector<DominantSquare> BruteForce::generateRandomDominantSquares(int numOfSquares, ClassSquareLimits squareLimits)
{
	// Dominant Square limits.
	float xMinLim = squareLimits.pointBL.x;
	float xMaxLim = squareLimits.pointBR.x;
	float yMinLim = squareLimits.pointBL.y;
	float yMaxLim = squareLimits.pointTL.y;

	// Initialize the Dominant Square Population.
	vector<DominantSquare> domSquares;

	// Generate a random dominant squares.
	for (int i = 0; i < numOfSquares; i++)
	{
		
		DominantSquare domSquare;
		domSquare.dominantClass = squareLimits.className;	// Set classname.

		// Generate random x and y min/max points for dominant square.
		float xMin = xMaxLim + 1;
		while(xMin >= (xMaxLim - 0.5))
			xMin = xMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xMaxLim - xMinLim)));

		float xMax = xMin - 1;
		while (xMax <= xMin)
			xMax = xMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xMaxLim - xMinLim)));

		float yMin = yMaxLim + 1;
		while(yMin >= (yMaxLim - 0.5))
			yMin = yMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (yMaxLim - yMinLim)));

		float yMax = yMin - 1;
		while (yMax <= yMin)
			yMax = yMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (yMaxLim - yMinLim)));

		// Make Dominant square coordinate points.
		MLPoint tL, tR, bR, bL;
		tL.x = xMin;	// Top Left Corner.
		tL.y = yMax;
		tR.x = xMax;	// Top Right Corner.
		tR.y = yMax;
		bR.x = xMax;	// Bottom Right Corner.
		bR.y = yMin;
		bL.x = xMin;	// Bottom Left Corner.
		bL.y = yMin;

		// Set classname for all coordinate points.
		tL.className = squareLimits.className;
		tR.className = squareLimits.className;
		bR.className = squareLimits.className;
		bL.className = squareLimits.className;

		// Set them in Dominant Square.
		domSquare.pointTL = tL;
		domSquare.pointTR = tR;
		domSquare.pointBR = bR;
		domSquare.pointBL = bL;

		domSquares.push_back(domSquare);
	}

	return domSquares;
}

// ********************************************************************************
// Calculates the purity of the population of dominant squares.
void BruteForce::fitness(vector<DominantSquare> &domSquares, MLPlane plane)
{
	for (int i = 0; i < domSquares.size(); i++)
		calculatePurity(domSquares[i], plane);
}

// ********************************************************************************
// Returns the average purity of the whole population of dominant squares.
float BruteForce::gradeDominantSquares(vector<DominantSquare> &domSquares, MLPlane plane)
{
	// First calculate the fitness of each dominant square in the population.
	fitness(domSquares, plane);

	double totalPopulationPurity = 0;
	double averagePopulationPurity;

	// Calcualte the total purity of the population.
	for (int i = 0; i < domSquares.size(); i++)
		totalPopulationPurity += domSquares[i].purity;

	// Calculate the average purity for the population as a whole.
	averagePopulationPurity = totalPopulationPurity / domSquares.size();

	return averagePopulationPurity;
}

// ********************************************************************************
// Sorts the population (highest purity first, lowest purity last).
void BruteForce::sortTheDomSquarePop(vector<DominantSquare> &domSquares)
{
	quicksort(domSquares, 0, domSquares.size() - 1, true);
}

// ********************************************************************************
// Mutate one of the retained squares. (Can mutate left/right/top/bottom sides.)
void BruteForce::mutateRetainedSquare(DominantSquare &retainedSquare, ClassSquareLimits squareLimits)
{
	// Randomly decide either to move or transpose dominant square.
	// 0 = Transpose (Change side dimensions), 1 = move dominant square.
	int moveOrTranspose = rand() % 2;

	// Transpose a random side of the dominant square by 10%.
	if (moveOrTranspose == 0)
	{
		// Dominant Square limits.
		float xMinLim = squareLimits.pointBL.x;
		float xMaxLim = squareLimits.pointBR.x;
		float yMinLim = squareLimits.pointBL.y;
		float yMaxLim = squareLimits.pointTL.y;

		// Randomly generate a new poinnt within the limits.
		float newX = xMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xMaxLim - xMinLim)));
		float newY = yMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (yMaxLim - yMinLim)));

		// Calculate how close the new axes are to the top/bottom/left/right of retained square.
		float distToLeft = fabs(newX - retainedSquare.pointBL.x);
		float distToRight = fabs(newX - retainedSquare.pointBR.x);
		float distToTop = fabs(newY - retainedSquare.pointTL.y);
		float distToBottom = fabs(newY - retainedSquare.pointBL.y);

		// Decide which point of the retained square to replace.
		// Top Left Corner.
		if (distToLeft < distToRight && distToBottom > distToTop)
		{
			retainedSquare.pointTL.x = newX; // Change x,y of top left point.
			retainedSquare.pointTL.y = newY;
			retainedSquare.pointBL.x = newX; // Change x of bottom left point.
			retainedSquare.pointTR.y = newY; // Change y of top right point.
		}
		// Top Right Corner.
		if (distToLeft > distToRight && distToBottom > distToTop)
		{
			retainedSquare.pointTR.x = newX; // Change x,y of top right point.
			retainedSquare.pointTR.y = newY;
			retainedSquare.pointBR.x = newX; // Change x of bottom right point.
			retainedSquare.pointTL.y = newY; // Change y of top left point.
		}
		// Bottom Right Corner.
		else if (distToLeft > distToRight && distToBottom < distToTop)
		{
			retainedSquare.pointBR.x = newX; // Change x,y of bottom Right point.
			retainedSquare.pointBR.y = newY;
			retainedSquare.pointTR.x = newX; // Change x of top right point.
			retainedSquare.pointBL.y = newY; // Change y of bottom left point.
		}
		// Bottom Left Corner.
		else if (distToLeft < distToRight && distToBottom < distToTop)
		{
			retainedSquare.pointBL.x = newX; // Change x,y of bottom left point.
			retainedSquare.pointBL.y = newY;
			retainedSquare.pointTL.x = newX; // Change x of top left point.
			retainedSquare.pointBR.y = newY; // Change y of bottom right point.
		}
		
	}
	// Move dominant square by 10% in a random direction.
	else
	{
		// Which direction to move.
		// 0 = left, 1 = up, 2 = right, 3 = down.
		int direction = rand() % 4;
		double difference;

		switch (direction)
		{
			// Move Left.
		case 0:
			difference = (retainedSquare.pointTR.x - retainedSquare.pointTL.x) * 0.1;
			retainedSquare.pointTL.x -= difference;
			retainedSquare.pointBL.x -= difference;
			retainedSquare.pointTR.x -= difference;
			retainedSquare.pointBR.x -= difference;
			break;
			// Move Up.
		case 1:
			difference = (retainedSquare.pointTL.y - retainedSquare.pointBL.y) * 0.1;
			retainedSquare.pointTL.y += difference;
			retainedSquare.pointTR.y += difference;
			retainedSquare.pointBL.y += difference;
			retainedSquare.pointBR.y += difference;
			break;
			// Move Right.
		case 2:
			difference = (retainedSquare.pointTR.x - retainedSquare.pointTL.x) * 0.1;
			retainedSquare.pointTL.x += difference;
			retainedSquare.pointBL.x += difference;
			retainedSquare.pointTR.x += difference;
			retainedSquare.pointBR.x += difference;
			break;
			// Move Down.
		case 3:
			difference = (retainedSquare.pointTL.y - retainedSquare.pointBL.y) * 0.1;
			retainedSquare.pointTL.y -= difference;
			retainedSquare.pointTR.y -= difference;
			retainedSquare.pointBL.y -= difference;
			retainedSquare.pointBR.y -= difference;
			break;
		}
	}
}

// ********************************************************************************
// Crossover from the parent.
void BruteForce::crossoverFromParent(DominantSquare &retainedSquare, vector<DominantSquare> parents)
{
	// Randomly choose which parent to crossover from.
	int whichParent = rand() % parents.size();

	// Randomly choose which axis to crossover from the parent.
	// 0 = xMin, 1, = yMin, 2 = xMax, 3 = yMax.
	int whichAxis = rand() % 4;

	// These will be the max/min axes from the chosen parent.
	float xMin;
	float xMax;
	float yMin;
	float yMax;

	switch (whichAxis)
	{
	case 0:
		// Crossover the minimum x axis to retainedSquare.
		xMin = parents[whichParent].pointBL.x;
		
		// If the minimum x axis of parent is greater than the
		// maximum x axis of retainedSquare, update the maximum
		// x axis of retainedSquare.
		if (xMin >= retainedSquare.pointBR.x)
		{
			retainedSquare.pointTR.x = xMin;
			retainedSquare.pointBR.x = xMin;
		}
		// Else upate the minimum x axis of retainedSquare.
		else
		{
			retainedSquare.pointTL.x = xMin;
			retainedSquare.pointBL.x = xMin;
		}
		break;
	case 1:
		// Crossover the minimum y axis to retainedSquare.
		yMin = parents[whichParent].pointBL.y;
		if (yMin >= retainedSquare.pointTL.y)
		{
			retainedSquare.pointTL.y = yMin;
			retainedSquare.pointTR.y = yMin;
		}
		else
		{
			retainedSquare.pointBL.y = yMin;
			retainedSquare.pointBR.y = yMin;
		}
		break;
	case 2:
		// Crossover the maximum x axis to retainedSquare.
		xMax = parents[whichParent].pointTL.x;
		if (xMax <= retainedSquare.pointBL.x)
		{
			retainedSquare.pointTL.x = xMax;
			retainedSquare.pointBL.x = xMax;
		}
		else
		{
			retainedSquare.pointTR.x = xMax;
			retainedSquare.pointBR.x = xMax;
		}
		break;
	case 3:
		// Crossover the maximum y axis to retainedSquare.
		yMax = parents[whichParent].pointTL.y;
		if (yMax <= retainedSquare.pointBL.y)
		{
			retainedSquare.pointBL.y = yMax;
			retainedSquare.pointBR.y = yMax;
		}
		else
		{
			retainedSquare.pointTL.y = yMax;
			retainedSquare.pointTR.y = yMax;
		}
		break;
	default:
		break;
	}
}

// ********************************************************************************
// Evolves the dominant squares (select, mutate, crossover).
vector<DominantSquare> BruteForce::evolveSquares(vector<DominantSquare> &domSquares, ClassSquareLimits squareLimits, float retain, float select, float mutate)
{
	// Sort the dominant squares population (highest fitness first).
	sortTheDomSquarePop(domSquares);

	// How much of the original population to retain.
	int retainLength = domSquares.size() * retain;
	vector<DominantSquare> parents;
	vector<DominantSquare> retained;

	// Parents are the first two options in the population.
	parents.push_back(domSquares[0]);
	parents.push_back(domSquares[1]);

	// Retain some of the population.
	for (int i = 2; i < retainLength + 2 && i < domSquares.size(); i++)
		retained.push_back(domSquares[i]);

	// Randomly select some of the remaining population to retain.
	for (int i = retainLength + 2; i < domSquares.size(); i++)
	{
		float randFloat = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (select > randFloat)
			retained.push_back(domSquares[i]);
	}

	// Randomly mutate some of the retained population.
	for (int i = 0; i < retained.size(); i++)
	{
		float randFloat = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (mutate > randFloat)
			mutateRetainedSquare(retained[i], squareLimits);
	}

	// Crossover with parents.
	int desiredLength = domSquares.size() - parents.size() - retained.size();
	vector<DominantSquare> children;

	while (children.size() < desiredLength)
	{
		// Choose a random dominant square from the retained population.
		int randRetain = rand() % retained.size();
		DominantSquare child = retained[randRetain];

		// Crossover child with one of the parents.
		crossoverFromParent(child, parents);

		// Add the child to the list of crossover children.
		children.push_back(child);
	}

	// Add the retained population and the children population to parents population.
	for (int i = 0; i < retained.size(); i++)
		parents.push_back(retained[i]);
	for (int i = 0; i < children.size(); i++)
		parents.push_back(children[i]);

	// Return the new population of dominant squares.
	return parents;
}

// ********************************************************************************
// ************ QUICKSORT IMPLEMENTATION ************
// NOTE: L and R are INCLUSIVE!!
// Example Initial Call: quicksort(domSquares, 0, domSquares.size()-1);
void BruteForce::quicksort(vector<DominantSquare> &domSquares, int L, int R, bool reverse)
{
	int i, j, mid;
	double piv;
	i = L;
	j = R;
	mid = L + (R - L) / 2;
	piv = domSquares[mid].purity;

	while (i<R || j>L)
	{
		if (reverse)
		{
			while (domSquares[i].purity > piv)
				i++;
			while (domSquares[j].purity < piv)
				j--;
		}
		else
		{
			while (domSquares[i].purity < piv)
				i++;
			while (domSquares[j].purity > piv)
				j--;
		}

		if (i <= j)
		{
			swap(domSquares, i, j); //error=swap function doesnt take 3 arguments
			i++;
			j--;
		}
		else
		{
			if (i < R)
				quicksort(domSquares, i, R, reverse);
			if (j > L)
				quicksort(domSquares, L, j, reverse);
			return;
		}
	}
}

// ********************************************************************************
void BruteForce::swap(vector<DominantSquare> &domSquare, int x, int y)
{
	DominantSquare temp = domSquare[x];
	domSquare[x] = domSquare[y];
	domSquare[y] = temp;
}
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************












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

			// Make Dominant square coordinate points.
			MLPoint tL, tR, bR, bL;
			tL.x = xMin;	// Top Left Corner.
			tL.y = yMax;
			tR.x = xMax;	// Top Right Corner.
			tR.y = yMax;
			bR.x = xMax;	// Bottom Right Corner.
			bR.y = yMin;
			bL.x = xMin;	// Bottom Left Corner.
			bL.y = yMin;

			// Set classname for all coordinate points.
			tL.className = squLimits.className;
			tR.className = squLimits.className;
			bR.className = squLimits.className;
			bL.className = squLimits.className;

			// Set them in Dominant Square.
			domSquare.pointTL = tL;
			domSquare.pointTR = tR;
			domSquare.pointBR = bR;
			domSquare.pointBL = bL;

			// Calculate total points in dominant square and points from class in dominant square.
			for (int p = 0; p < pln.pointsInPlane.size(); p++)
			{
				MLPoint tempPoint = pln.pointsInPlane[p];
				int x = tempPoint.x;
				int y = tempPoint.y;
				if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
				{
					domSquare.totalPointsInSquare++;
					if (tempPoint.className.compare(domSquare.dominantClass) == 0)
						domSquare.classPointsInSquare++;
				}
			}

			// Calculate dominant square purity.
			if(domSquare.totalPointsInSquare > 0)
				domSquare.purity = domSquare.classPointsInSquare / (double)domSquare.totalPointsInSquare;

			// Add dominant square to Plane's dominant squares vector.
			pln.domSquares.push_back(domSquare);
		}

		dummyDominantPlanes[i] = pln; // Update the plane in dummyDominantPlanes.
	}

	return dummyDominantPlanes;
}