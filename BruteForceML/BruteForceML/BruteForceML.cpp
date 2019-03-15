/*
Author:	Al Timofeyev & Wesley Higbee
Date:	Feb.21 2019 - Mar.15 2019
Desc:	A brute force attempt to identify Dominant Square
with the best (highest) purity level.
*/


#include "BruteForceML.h"

#include <iostream> // For Debugginng.

// --------------------------------------
// ------------ CONSTRUCTORS ------------
// --------------------------------------
//BruteForce::BruteForce(vector<DataClass> classesSet) : classes(classesSet){}
BruteForce::BruteForce(std::string nameOfFile) : filename(nameOfFile)
{
	readFile();
	splitData(70, 30);
}


// --------------------------------------
// -------------- METHODS ---------------
// --------------------------------------
void BruteForce::readFile()
{
	// Read from file.
	// Open the file and make sure it exists.
	std::ifstream inputFile(filename);
	if (inputFile.fail())
	{
		std::cout << "Failed to open file: " << filename << std::endl;
		return;
	}

	// Read the file and parse the integers into numList.
	DataClass classData = DataClass();
	int classID = -1;
	std::string classname = "";
	std::string currentClassName;
	bool isInClass = false;
	std::string line;
	while (!inputFile.eof())
	{
		// Read line from file and make sure it has something.
		getline(inputFile, line);
		if (line.length() == 0)
			break;

		// Tokenize the line.
		std::istringstream stream(line);
		std::vector<std::string> tokenized;
		std::string token;
		while (getline(stream, token, ','))
		{
			tokenized.push_back(token);
		}

		// Update the current class name.
		currentClassName = tokenized[tokenized.size() - 1];

		// If the current class name is different than the previous ones.
		if (classname.compare(currentClassName) != 0)
		{
			// Update the classname.
			classname = currentClassName;

			// Check to see if the class doesn't already exist in classes.
			for (int c = 0; c < classes.size(); c++)
			{
				// If it exists.
				if (classname.compare(classes[c].className) == 0)
				{
					// Set the flag to true and leave the loop.
					isInClass = true;
					break;
				}
			}

			// If the class is not in classes.
			if (!isInClass)
			{
				// Update the classID.
				classID++;

				// Make a new DataClass struct to hold class info.
				classData = DataClass();

				// Push the new class struct into the classes vector list.
				classes.push_back(classData);

				// Set the name of the new class in DataClass struct.
				classes[classID].className = classname;
			}
		}

		// Extract data from tokenized string.
		std::vector<float> data;
		for (int i = 0; i < tokenized.size() - 1; i++)
		{
			data.push_back(stod(tokenized[i]));
		}

		// Add new data to the classes dataset.
		for (int c = 0; c < classes.size(); c++)
		{
			if (classname.compare(classes[c].className) == 0)
				classes[c].classDataset.push_back(data);
		}
	}

	// CLose input file.
	inputFile.close();

	return;
}

// ********************************************************************************
// ********************************************************************************
// Split data into testing and training.
void BruteForce::splitData(float trainPercent, float testPercent)
{
	if (trainPercent > 1.0)
		trainPercent = trainPercent / 100;
	if (trainPercent <= 0)
		trainPercent = 0.7;

	if (testPercent > 1.0)
		testPercent = testPercent / 100;
	if (testPercent <= 0)
		testPercent = 0.3;

	// For all the classes in classes vector.
	for (int c = 0; c < classes.size(); c++)
	{
		// Get the total, training, and testing number of points for the class.
		int totalNumOfPoints = classes[c].classDataset.size();
		int trainNumOfPoints = totalNumOfPoints * trainPercent;
		int testNumOfPoints = totalNumOfPoints - trainNumOfPoints;

		// Create train and test DataClass objects to hold data.
		DataClass train;
		DataClass test;
		train.className = classes[c].className;	// Set classname.
		test.className = classes[c].className;	// Set classname.

		// For each point in the classes dataset.
		for (int cD = 0; cD < classes[c].classDataset.size(); cD++)
		{
			// If the point counter is less than the training point counter, add to training set.
			if (cD < trainNumOfPoints)
				train.classDataset.push_back(classes[c].classDataset[cD]);
			// Else add the point to testing set.
			else
				test.classDataset.push_back(classes[c].classDataset[cD]);
		}

		// Place both datasets into their respective vector lists.
		training.push_back(train);
		testing.push_back(test);
	}
}

// ********************************************************************************
// ********************************************************************************
std::vector<MLPlane> BruteForce::run()
{
	srand(time(0)); // Initalize rand() with random seed.

	getNumOfAttributes();
	//numOfAttributes = -1;
	std::cout << "\nNum Of Attributes: " << numOfAttributes << std::endl;
	getNumOfPlanes();
	std::cout << "Num Of Planes: " << numOfPlanesPossible << "\n\n";

	// Can't really do anything with 1 attribute.
	if (numOfAttributes < 2 || numOfPlanesPossible < 1)
		return dominantPlanes;

	// Generate some planes.
	for (int i = 0; i < 4; i++)
		assignPlanes();

	// Set the limits of the dominant squares
	// for each class in each plane.
	setDomSquareLimits();
	

	// Initiate the genetic algorithm with a population size to
	// find dominant squares of each class in each plane.
	geneticAlgorithm(25);



	findDominantPlanes();
	std::cout << "-------------------------------------------\n";
	std::cout << "DOMINANT PLANES ---------------------------\n\n";
	for (int dP = 0; dP < dominantPlanes.size(); dP++)
	{
		std::cout << "Plane: " << dP << "\t\tDimensions: [" << dominantPlanes[dP].dimensionX << ", " << dominantPlanes[dP].dimensionY << "]" << std::endl;
		std::cout << "Number of Dominant Squares: " << dominantPlanes[dP].domSquares.size() << std::endl;
		std::cout << "Dom Square Classes:\n";
		for (int dS = 0; dS < dominantPlanes[dP].domSquares.size(); dS++)
		{
			std::cout << dominantPlanes[dP].domSquares[dS].className << "\t\t";
			std::cout << "Purity: " << dominantPlanes[dP].domSquares[dS].purity << "\t\t";
			std::cout << "Class Points in Square: " << dominantPlanes[dP].domSquares[dS].classPointsInSquare << "\t\t";
			std::cout << "Total Points in Square: " << dominantPlanes[dP].domSquares[dS].totalPointsInSquare << std::endl;
			std::cout << "Dominant Square Points:" << std::endl;
			std::cout << "Top Left:    [" << dominantPlanes[dP].domSquares[dS].pointTL.x << " , " << dominantPlanes[dP].domSquares[dS].pointTL.y << "]\t\t";
			std::cout << "Top Right:    [" << dominantPlanes[dP].domSquares[dS].pointTR.x << " , " << dominantPlanes[dP].domSquares[dS].pointTR.y << "]\n";
			std::cout << "Bottom Left: [" << dominantPlanes[dP].domSquares[dS].pointBL.x << " , " << dominantPlanes[dP].domSquares[dS].pointBL.y << "]\t\t";
			std::cout << "Bottom Right: [" << dominantPlanes[dP].domSquares[dS].pointBR.x << " , " << dominantPlanes[dP].domSquares[dS].pointBR.y << "]\n\n";
		}
	}

	// Save the dominant squares to file.
	saveDominantSquaresToFile();

	// Save the dominant squares from each plane into a list in proper order (by class).
	saveDominantSquaresToList();
	std::cout << "\n\n----- Dominant Squares List -----\n";
	for (int dS = 0; dS < dominantSquares.size(); dS++)
	{
		for (int s = 0; s < dominantSquares[dS].size(); s++)
		{
			std::cout << "Classname: " << dominantSquares[dS][s].className << std::endl;
			std::cout << "Top Left:    [" << dominantSquares[dS][s].pointTL.x << " , " << dominantSquares[dS][s].pointTL.y << "]\t\t";
			std::cout << "Top Right:    [" << dominantSquares[dS][s].pointTR.x << " , " << dominantSquares[dS][s].pointTR.y << "]\n";
			std::cout << "Bottom Left: [" << dominantSquares[dS][s].pointBL.x << " , " << dominantSquares[dS][s].pointBL.y << "]\t\t";
			std::cout << "Bottom Right: [" << dominantSquares[dS][s].pointBR.x << " , " << dominantSquares[dS][s].pointBR.y << "]\n\n";
		}
		std::cout << "---------------------------------\n\n";
	}

	// Save the plane dimensions as combinations and keep them in a vector list.
	savePlaneDimensionCombinations();
	std::cout << "\n----- Plane Dimensions Saved -----\nX:\tY:\n";
	for (int pD = 0; pD < planeDimensions.size(); pD++)
		std::cout << planeDimensions[pD][0] << "\t" << planeDimensions[pD][1] << std::endl;
	std::cout << "\n\n";

	// Create the rules.
	createRules();
	std::cout << "\n----- Rules Saved -----\n";
	std::cout << "Number of Rules: " << rules.size() << std::endl;
	std::cout << "-----------------------\n";
	for (int r = 0; r < rules.size(); r++)
	{
		std::cout << "Rule number: " << rules[r].ruleNum << std::endl;
		std::cout << "Rule classname: " << rules[r].className << std::endl;
		std::cout << "Rule Accuracy: " << rules[r].accuracy << std::endl;
		std::cout << "Number of classes of points in Rule: " << rules[r].pointsInRule.size() << std::endl;
		std::cout << "Number of In dominant squares: " << rules[r].in.size() << "\t\t\tNumber of Out dominant squares: " << rules[r].out.size() << std::endl;
		std::cout << "Dominant Square Purities in In:\t";
		for (int rl = 0; rl < rules[r].in.size(); rl++)
			std::cout << rules[r].in[rl].purity << "\t";
		std::cout << "\t\t";
		std::cout << "Dominant Square Purities in Out:\t";
		for (int rl = 0; rl < rules[r].out.size(); rl++)
			std::cout << rules[r].out[rl].purity << "\t";
		std::cout << std::endl;

		std::cout << "Dominant Square Plane Index In:\t";
		for (int rl = 0; rl < rules[r].in.size(); rl++)
			std::cout << rules[r].in[rl].planeIndex << "\t";
		std::cout << "\t\t";
		std::cout << "Dominant Square Plane Index Out:\t";
		for (int rl = 0; rl < rules[r].out.size(); rl++)
			std::cout << rules[r].out[rl].planeIndex << "\t";
		std::cout << std::endl;

		// For all the classes of points.
		for (int pC = 0; pC < rules[r].pointsInRule.size(); pC++)
		{
			std::cout << "------------\n";
			std::cout << "ClassName: " << rules[r].pointsInRule[pC].className << "\t\t";
			std::cout << "Number of Points from Class: " << rules[r].pointsInRule[pC].classDataset.size() << std::endl;
			for (int p = 0; p < rules[r].pointsInRule[pC].classDataset.size(); p++)
			{
				for (int dP = 0; dP < rules[r].pointsInRule[pC].classDataset[p].size(); dP++)
					std::cout << rules[r].pointsInRule[pC].classDataset[p][dP] << "\t";
				std::cout << "\n\n";
			}
			std::cout << std::endl;
		}
		std::cout << "\n-----------------------\n";
	}

	// Save rules to file.
	saveRulesToFile();

	//testRule();

	return dominantPlanes; // THIS IS THE ACTUAL RETURN VALUE DO NOT DELETE!!
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
			currPlane.dimensionX = k;	// Assign Attribute
			k++;

			// SECOND ATTRIBUTE
			// If it's the last plane to be made and there is an additional attribute.
			if (i == (numOfPlanesPossible - 1) && addAttribute != -1)
			{
				// Make sure that the additional attribute doesn't equal
				// the first attribute of the plane.
				while (addAttribute == currPlane.dimensionX)
					addAttribute = rand() % numOfAttributes;

				currPlane.dimensionY = addAttribute;	// Assign Attribute
			}
			else
				currPlane.dimensionY = k;				// Assign Attribute
			k++;

			// Store correct x and y coordinates of 2D points that belong in this plane.
			// Loop through all the classes.
			for (int c = 0; c < classes.size() && c < 2; c++) // c < 2 is Classes Limiter!!
			{
				// Add the class name to the list of classnames for the plane.
				currPlane.allClassNames.push_back(classes[c].className);

				DataClass tempClass = classes[c];
				for (int cD = 0; cD < tempClass.classDataset.size(); cD++)
				{
					// Loop through all the multidimensional points of every class.
					std::vector<float> data = tempClass.classDataset[cD];
					currPlane.xCoordinates.push_back(data[currPlane.dimensionX]);
					currPlane.yCoordinates.push_back(data[currPlane.dimensionY]);

					// Make a point as well with combined x,y values.
					MLPoint point;
					point.className = tempClass.className;
					point.x = data[currPlane.dimensionX];
					point.y = data[currPlane.dimensionY];
					currPlane.pointsInPlane.push_back(point); // Store the point in plane.
				}
			}

			// Do the same as above using only the training set of points.
			for (int t = 0; t < training.size() && t < 2; t++) // t < 2 is Classes Limiter!!
			{
				DataClass tempTrainingClass = training[t];
				for (int cD = 0; cD < tempTrainingClass.classDataset.size(); cD++)
				{
					// Loop through all the multidimensional points of every class for the training set.
					std::vector<float> data = tempTrainingClass.classDataset[cD];

					// Make a point with combined x,y values.
					MLPoint point;
					point.className = tempTrainingClass.className;
					point.x = data[currPlane.dimensionX];
					point.y = data[currPlane.dimensionY];
					currPlane.trainingPointsInPlane.push_back(point); // Store the point in plane.
				}
			}

			planesSt.planes.push_back(currPlane);
		}
	}

	// If not the first set of Planes, then randomize attribute pairs.
	else
	{
		std::vector<int> alreadyChosen;
		int k = rand() % numOfAttributes;
		for (int i = 0; i < numOfPlanesPossible; i++)
		{
			MLPlane currPlane;

			// FIRST ATTRIBUTE
			// Find attribute that hasn't been chosen yet.
			while (find(alreadyChosen.begin(), alreadyChosen.end(), k) != alreadyChosen.end())
				k = rand() % numOfAttributes;
			alreadyChosen.push_back(k);

			currPlane.dimensionX = k;	// Assign Attribute

			// SECOND ATTRIBUTE
			// If it's the last plane to be made and there is an additional attribute.
			if (i == (numOfPlanesPossible - 1) && addAttribute != -1)
			{
				// Make sure that the additional attribute doesn't equal
				// the first attribute of the plane.
				while (addAttribute == currPlane.dimensionX)
					addAttribute = rand() % numOfAttributes;
				currPlane.dimensionY = addAttribute;	// Assign Attribute
			}
			else
			{
				while (find(alreadyChosen.begin(), alreadyChosen.end(), k) != alreadyChosen.end())
					k = rand() % numOfAttributes;
				alreadyChosen.push_back(k);

				currPlane.dimensionY = k;				// Assign Attribute
			}

			// Store correct x and y coordinates of 2D points that belong in this plane.
			// Loop through all the classes.
			for (int c = 0; c < classes.size() && c < 2; c++) // c < 2 is Classes Limiter!!
			{
				// Add the class name to the list of classnames for the plane.
				currPlane.allClassNames.push_back(classes[c].className);

				DataClass tempClass = classes[c];
				for (int cD = 0; cD < tempClass.classDataset.size(); cD++)
				{
					// Loop through all the multidimensional points of every class.
					std::vector<float> data = tempClass.classDataset[cD];
					currPlane.xCoordinates.push_back(data[currPlane.dimensionX]);
					currPlane.yCoordinates.push_back(data[currPlane.dimensionY]);

					// Make a point as well with combined x,y values.
					MLPoint point;
					point.className = tempClass.className;
					point.x = data[currPlane.dimensionX];
					point.y = data[currPlane.dimensionY];
					currPlane.pointsInPlane.push_back(point); // Store the point in plane.
				}
			}

			// Do the same as above using only the training set of points.
			for (int t = 0; t < training.size() && t < 2; t++) // t < 2 is Classes Limiter!!
			{
				DataClass tempTrainingClass = training[t];
				for (int cD = 0; cD < tempTrainingClass.classDataset.size(); cD++)
				{
					// Loop through all the multidimensional points of every class for the training set.
					std::vector<float> data = tempTrainingClass.classDataset[cD];

					// Make a point with combined x,y values.
					MLPoint point;
					point.className = tempTrainingClass.className;
					point.x = data[currPlane.dimensionX];
					point.y = data[currPlane.dimensionY];
					currPlane.trainingPointsInPlane.push_back(point); // Store the point in plane.
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
	for (int i = 0; i < classes.size() && i < 2; i++) // i < 2 is Classes Limiter!!
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
				int attr1 = pln.dimensionX;	// X
				int attr2 = pln.dimensionY;	// Y

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
					std::vector<float> data = classData.classDataset[d];
					if (data[attr1] < xMin) // Determine xMin.
						xMin = data[attr1];
					if (data[attr1] > xMax) // Determine xMax.
						xMax = data[attr1];

					if (data[attr2] < yMin) // Determine yMin.
						yMin = data[attr2];
					if (data[attr2] > yMax) // Determine yMax.
						yMax = data[attr2];
				}

				// If xMin and xMax are equale.
				if (xMin == xMax)
				{
					xMin -= 0.05;
					xMax += 0.05;
				}
				// If yMin and yMax are equal.
				if (yMin == yMax)
				{
					yMin -= 0.05;
					yMax += 0.05;
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
			box.totalPointsInSquare++;
			if (tempPoint.className.compare(box.className) == 0)
			{
				box.classPointsInSquare++;
			}
		}
	}

	// Calculate dominant square purity.
	if (box.totalPointsInSquare > 0)
		box.purity = 100 * (box.classPointsInSquare / (double)box.totalPointsInSquare);
	else
		box.purity = 0;

	// Normalize purity based on how useful it is.
	for (int i = 0; i < classes.size() && i < 2; i++) // i < 2 is Classes Limiter.
	{
		if (box.className.compare(classes[i].className) == 0)
		{
			if (box.classPointsInSquare < (classes[i].classDataset.size() * 0.85))
				box.purity = box.purity * box.classPointsInSquare / (double)classes[i].classDataset.size();
			break;
		}
	}
}

// ********************************************************************************
// ********************************************************************************
// Using testing set of points.
void BruteForce::calculatePurity(DominantSquare &box, std::vector<MLPoint> points)
{
	float xMin = box.pointBL.x;
	float xMax = box.pointBR.x;
	float yMin = box.pointBL.y;
	float yMax = box.pointTL.y;

	// Reset the totalPointsInSquare and classPointsInSquare of the dominant square.
	box.totalPointsInSquare = 0;
	box.classPointsInSquare = 0;

	// Calculate total points in dominant square and points from class in dominant square.
	for (int p = 0; p < points.size(); p++)
	{
		MLPoint tempPoint = points[p];
		float x = tempPoint.x;
		float y = tempPoint.y;
		if (x >= xMin && x <= xMax && y >= yMin && y <= yMax)
		{
			box.totalPointsInSquare++;
			if (tempPoint.className.compare(box.className) == 0)
			{
				box.classPointsInSquare++;
			}
		}
	}

	// Calculate dominant square purity.
	if (box.totalPointsInSquare > 0)
		box.purity = 100 * (box.classPointsInSquare / (double)box.totalPointsInSquare);
	else
		box.purity = 0;

	// Normalize purity based on how useful it is.
	for (int t = 0; t < training.size() && t < 2; t++) // t < 2 is Classes Limiter.
	{
		if (box.className.compare(training[t].className) == 0)
		{
			if (box.classPointsInSquare < (training[t].classDataset.size() * 0.85))
				box.purity = box.purity * box.classPointsInSquare / (double)training[t].classDataset.size();
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

		box.className = pln.limitsOfClasses[i].className;

		int count = 0;
		calculatePurity(box, pln);

		while (box.purity < 80 && box.classPointsInSquare > 0)
		{
			std::cout << box.purity << std::endl;
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
			}

			std::cout << "Class Points In Square: " << box.classPointsInSquare << "\t\t";
			std::cout << "Total Points In Square: " << box.totalPointsInSquare << "\t\t";
			std::cout << "Total Points In Plane: " << pln.pointsInPlane.size() << std::endl;

			std::cout << "Top Left:  [" << box.pointTL.x << " , " << box.pointTL.y << "]\t\t";
			std::cout << "Top Right: [" << box.pointTR.x << " , " << box.pointTR.y << "]\n";
			std::cout << "Bot Left:  [" << box.pointBL.x << " , " << box.pointBL.y << "]\t\t";
			std::cout << "Bot Right: [" << box.pointBR.x << " , " << box.pointBR.y << "]\n\n";

		}
	}
}

// ********************************************************************************
// ********************************************************************************
// Find a dominant set of planes with the best dominant squares.
void BruteForce::findDominantPlanes()
{
	// This will hold the index of the set of Planes with the best
	// average purities (dominant squares).
	int bestPurityIndex = 0;

	// Keep track of the average purity of all the sets of planes.
	std::vector<double> avgPlanesetPurity;

	// Calculate the average purity of all planes per plane set in all setsOfPlanes.
	for (int pS = 0; pS < setOfPlanes.size(); pS++)
	{
		// This will hold the total purity of dominant squares for each individual
		// class in all planes.
		std::vector<double> totalClassPurities(classes.size());
		std::vector<double> avgClassPurity;
		double totalPurityOfAllClasses = 0;

		// Get the total purities from all classes in all planes of this Plane set.
		std::vector<MLPlane> planes = setOfPlanes[pS].planes;
		for (int p = 0; p < planes.size(); p++)
		{
			MLPlane plane = planes[p];
			for (int dS = 0; dS < plane.domSquares.size(); dS++)
				totalClassPurities[dS] += plane.domSquares[dS].purity;
		}

		// Calculate the average purities for all classes in totalClassPurities.
		for (int tCP = 0; tCP < totalClassPurities.size() && tCP < 2; tCP++) // tCP < 2 is a classes limiter1!!
			avgClassPurity.push_back(totalClassPurities[tCP] / (double)planes.size());

		// Calculate the total purity for all average class purities in plane set.
		for (int aCP = 0; aCP < avgClassPurity.size(); aCP++)
			totalPurityOfAllClasses += avgClassPurity[aCP];

		// Save the average purity of the plane set.
		avgPlanesetPurity.push_back(totalPurityOfAllClasses / (double)avgClassPurity.size());
	}

	// Determine which of the plane sets has the best purity.
	float bestPurity = 0.0;
	for (int aPSP = 0; aPSP < avgPlanesetPurity.size(); aPSP++)
	{
		std::cout << "AvgPlanesetPurity Of " << aPSP << ": " << avgPlanesetPurity[aPSP] << "\t\t";
		if (avgPlanesetPurity[aPSP] > bestPurity)
		{
			bestPurityIndex = aPSP;
			bestPurity = avgPlanesetPurity[aPSP];
		}
		std::cout << "Best Planeset Purity So Far: " << bestPurity << std::endl;
	}

	std::cout << "Planes Set " << bestPurityIndex << " has the Best Purity Dominant Squares!\n\n";
	// Set the dominant set of planes (best purity of dominant squares).
	dominantPlanes = setOfPlanes[bestPurityIndex].planes;
}

// ********************************************************************************
// ********************************************************************************
// Save the dominant squares from all planes to squares.txt
// Layout: int plane, float yMin, float yMax, float xMin, float xMax
int BruteForce::saveDominantSquaresToFile()
{
	// Create the text file.
	std::ofstream domSquaresFile("squares.txt");
	if (domSquaresFile.fail())
	{
		std::cout << "Failed to create file: " << "squares.txt" << std::endl;
		std::cout << "**** Terminating Problem Execution ****\n\n";
		return -1;
	}

	// Write to text file.
	for (int dP = 0; dP < dominantPlanes.size(); dP++)
	{
		for (int dS = 0; dS < dominantPlanes[dP].domSquares.size(); dS++)
		{
			std::string line = "";

			// Add Plane Index.
			line += std::to_string(dP) + ",";

			// Add the yMin, yMax, xMin, xMax.
			line += std::to_string(dominantPlanes[dP].domSquares[dS].pointBL.y) + ",";
			line += std::to_string(dominantPlanes[dP].domSquares[dS].pointTL.y) + ",";
			line += std::to_string(dominantPlanes[dP].domSquares[dS].pointBL.x) + ",";
			line += std::to_string(dominantPlanes[dP].domSquares[dS].pointBR.x) + "\n";

			// Save that line to file.
			domSquaresFile << line;
			std::cout << line;
		}
	}

	// Close text file.
	domSquaresFile.close();
	return 0;
}

// ********************************************************************************
// ********************************************************************************
// Save the dominant squares from each plane into a sorted list (by class) of dominant squares.
void BruteForce::saveDominantSquaresToList()
{
	// Initialize the dominantSquares list with blank lists of dominant squares.
	for (int dS = 0; dS < dominantPlanes[0].domSquares.size(); dS++)
	{
		std::vector<DominantSquare> squares;
		dominantSquares.push_back(squares);
	}

	// For every plane, save the dominant squares into the dominantSquares list in the proper order (by class).
	for (int p = 0; p < dominantPlanes.size(); p++)
	{
		for (int dS = 0; dS < dominantPlanes[p].domSquares.size(); dS++)
		{
			dominantSquares[dS].push_back(dominantPlanes[p].domSquares[dS]);
		}
	}
}

// ********************************************************************************
// ********************************************************************************
// Save the plane dimensions as combinations and keep them in a vector list.
void BruteForce::savePlaneDimensionCombinations()
{
	// For every plane, save the dimension combinations.
	for (int p = 0; p < dominantPlanes.size(); p++)
	{
		std::vector<float> dimensionCombo;
		dimensionCombo.push_back(dominantPlanes[p].dimensionX);
		dimensionCombo.push_back(dominantPlanes[p].dimensionY);

		planeDimensions.push_back(dimensionCombo);
	}
}

// ********************************************************************************
// ********************************************************************************
// Save the created rules to file rules.txt.
int BruteForce::saveRulesToFile()
{
	// Create the text file.
	std::ofstream rulesFile("rules.txt");
	if (rulesFile.fail())
	{
		std::cout << "Failed to create file: " << "squares.txt" << std::endl;
		std::cout << "**** Terminating Problem Execution ****\n\n";
		return -1;
	}

	// Write to text file.
	for (int r = 0; r < rules.size(); r++)
	{
		std::string line = "";

		// Add rule number and classname.
		line += "Rule " + std::to_string(rules[r].ruleNum) + " for class " + rules[r].className + " :\n";

		// Add the In squares.
		if (rules[r].in.size() > 0)
		{
			line += "**** If the point is in the\n";
			for (int sI = 0; sI < rules[r].in.size(); sI++)
			{
				line += rules[r].in[sI].className + " dominant square of plane " + std::to_string(rules[r].in[sI].planeIndex + 1) + "\n";
			}
		}

		// Add the Out squares.
		if (rules[r].out.size() > 0)
		{
			line += "**** And If the point is NOT in the\n";
			for (int sO = 0; sO < rules[r].out.size(); sO++)
			{
				line += rules[r].out[sO].className + " dominant square of plane " + std::to_string(rules[r].out[sO].planeIndex + 1) + "\n";
			}
		}

		line += "Then the point can be classified as part of class " + rules[r].className;
		line += "\n----------------------------------------------------------\n";

		// Save that line to file.
		rulesFile << line;
	}

	// Close text file.
	rulesFile.close();
	return 0;
}


// ********************************************************************************
// **************************** GENETIC ALGORITHM PART ****************************
// ********************************************************************************
// The main function of the Genetic Algorithm.
void BruteForce::geneticAlgorithm(int popSize)
{
	for (int sP = 0; sP < setOfPlanes.size(); sP++)
	{
		for (int p = 0; p < setOfPlanes[sP].planes.size(); p++)
		{
			for (int lim = 0; lim < setOfPlanes[sP].planes[p].limitsOfClasses.size(); lim++)
			{
				// Create the population of dominant squares based on the limits of classes.
				std::vector<DominantSquare> domSquares = generateRandomDominantSquares(popSize, setOfPlanes[sP].planes[p].limitsOfClasses[lim]);

				// Grade the original population and get its average purity.
				float avgPurity = gradeDominantSquares(domSquares, setOfPlanes[sP].planes[p]);

				// Set the generations counter;
				int generations = 0;

				// Evolve the population until the average purity is 90 or greater.
				while (avgPurity < 90 && generations < 50)
				{
					domSquares = evolveSquares(domSquares, setOfPlanes[sP].planes[p].limitsOfClasses[lim]);
					avgPurity = gradeDominantSquares(domSquares, setOfPlanes[sP].planes[p]);
					generations++;
				}

				// Sort the population of Dominant Squares based on purity.
				sortTheDomSquarePop(domSquares);

				// Set the plane index of the best dominant square in population.
				domSquares[0].planeIndex = p;

				// Set the x and y dimensions of the plane in the dominant square.
				domSquares[0].dimensionX = setOfPlanes[sP].planes[p].dimensionX;
				domSquares[0].dimensionY = setOfPlanes[sP].planes[p].dimensionY;

				// Set the xMin, xMax, yMin, yMax of the best dominant square in population.
				domSquares[0].xMin = domSquares[0].pointBL.x;
				domSquares[0].xMax = domSquares[0].pointBR.x;
				domSquares[0].yMin = domSquares[0].pointBL.y;
				domSquares[0].yMax = domSquares[0].pointTL.y;

				// Save the best dominant square into the plane.
				setOfPlanes[sP].planes[p].domSquares.push_back(domSquares[0]);
			}
		}
	}
}

// ********************************************************************************
// Generates a population of dominant squares within the limits of the squareLimits classname.
std::vector<DominantSquare> BruteForce::generateRandomDominantSquares(int numOfSquares, ClassSquareLimits squareLimits)
{
	// Dominant Square limits.
	float xMinLim = squareLimits.pointBL.x;
	float xMaxLim = squareLimits.pointBR.x;
	float yMinLim = squareLimits.pointBL.y;
	float yMaxLim = squareLimits.pointTL.y;

	// Initialize the Dominant Square Population.
	std::vector<DominantSquare> domSquares;

	// Generate a random dominant squares.
	for (int i = 0; i < numOfSquares; i++)
	{

		DominantSquare domSquare;
		domSquare.className = squareLimits.className;	// Set classname.

		// Generate random x and y min/max points for dominant square.
		float xMin = xMaxLim + 1;
		while (xMin >= (xMaxLim - 0.5))
			xMin = xMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xMaxLim - xMinLim)));

		float xMax = xMin - 1;
		while (xMax <= xMin)
			xMax = xMinLim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xMaxLim - xMinLim)));

		float yMin = yMaxLim + 1;
		while (yMin >= (yMaxLim - 0.5))
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
void BruteForce::fitness(std::vector<DominantSquare> &domSquares, MLPlane plane)
{
	for (int i = 0; i < domSquares.size(); i++)
		calculatePurity(domSquares[i], plane.trainingPointsInPlane); // Using training set of points.
}

// ********************************************************************************
// Returns the average purity of the whole population of dominant squares.
float BruteForce::gradeDominantSquares(std::vector<DominantSquare> &domSquares, MLPlane plane)
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
void BruteForce::sortTheDomSquarePop(std::vector<DominantSquare> &domSquares)
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

	// Dominant Square limits.
	float xMinLim = squareLimits.pointBL.x;
	float xMaxLim = squareLimits.pointBR.x;
	float yMinLim = squareLimits.pointBL.y;
	float yMaxLim = squareLimits.pointTL.y;

	// Transpose a random side of the dominant square by 10%.
	if (moveOrTranspose == 0)
	{
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
		else if (distToLeft > distToRight && distToBottom > distToTop)
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

			// Check limitations of square
			// If the x value goes outside dominant square limits.
			if (retainedSquare.pointBL.x < xMinLim)
			{
				retainedSquare.pointBL.x = xMinLim;
				retainedSquare.pointTL.x = xMinLim;
			}
			// If the two x values of the dominant square are the same.
			if (retainedSquare.pointBR.x == retainedSquare.pointBL.x)
			{
				retainedSquare.pointBR.x += 0.05;
				retainedSquare.pointTR.x += 0.05;
			}
			break;
			// Move Up.
		case 1:
			difference = (retainedSquare.pointTL.y - retainedSquare.pointBL.y) * 0.1;
			retainedSquare.pointTL.y += difference;
			retainedSquare.pointTR.y += difference;
			retainedSquare.pointBL.y += difference;
			retainedSquare.pointBR.y += difference;

			// Check limitations of square
			// If the y value goes outside dominant square limits.
			if (retainedSquare.pointTL.y > yMaxLim)
			{
				retainedSquare.pointTR.y = yMaxLim;
				retainedSquare.pointTL.y = yMaxLim;
			}
			// If the two y values of the dominant square are the same.
			if (retainedSquare.pointBL.y == retainedSquare.pointTL.y)
			{
				retainedSquare.pointBL.y -= 0.05;
				retainedSquare.pointBR.y -= 0.05;
			}
			break;
			// Move Right.
		case 2:
			difference = (retainedSquare.pointTR.x - retainedSquare.pointTL.x) * 0.1;
			retainedSquare.pointTL.x += difference;
			retainedSquare.pointBL.x += difference;
			retainedSquare.pointTR.x += difference;
			retainedSquare.pointBR.x += difference;

			// Check limitations of square
			// If the x value goes outside dominant square limits.
			if (retainedSquare.pointBR.x > xMaxLim)
			{
				retainedSquare.pointBR.x = xMaxLim;
				retainedSquare.pointTR.x = xMaxLim;
			}
			// If the two x values of the dominant square are the same.
			if (retainedSquare.pointBL.x == retainedSquare.pointBR.x)
			{
				retainedSquare.pointBL.x -= 0.05;
				retainedSquare.pointTL.x -= 0.05;
			}
			break;
			// Move Down.
		case 3:
			difference = (retainedSquare.pointTL.y - retainedSquare.pointBL.y) * 0.1;
			retainedSquare.pointTL.y -= difference;
			retainedSquare.pointTR.y -= difference;
			retainedSquare.pointBL.y -= difference;
			retainedSquare.pointBR.y -= difference;

			// Check limitations of square
			// If the y value goes outside dominant square limits.
			if (retainedSquare.pointBL.y < yMinLim)
			{
				retainedSquare.pointBR.y = yMinLim;
				retainedSquare.pointBL.y = yMinLim;
			}
			// If the two y values of the dominant square are the same.
			if (retainedSquare.pointTL.y == retainedSquare.pointBL.y)
			{
				retainedSquare.pointTL.y += 0.05;
				retainedSquare.pointTR.y += 0.05;
			}
			break;
		}
	}
}

// ********************************************************************************
// Crossover from the parent.
void BruteForce::crossoverFromParent(DominantSquare &retainedSquare, std::vector<DominantSquare> parents)
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
std::vector<DominantSquare> BruteForce::evolveSquares(std::vector<DominantSquare> &domSquares, ClassSquareLimits squareLimits, float retain, float select, float mutate)
{
	// Sort the dominant squares population (highest fitness first).
	sortTheDomSquarePop(domSquares);

	// How much of the original population to retain.
	int retainLength = domSquares.size() * retain;
	std::vector<DominantSquare> parents;
	std::vector<DominantSquare> retained;

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
	std::vector<DominantSquare> children;

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
void BruteForce::quicksort(std::vector<DominantSquare> &domSquares, int L, int R, bool reverse)
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
void BruteForce::swap(std::vector<DominantSquare> &domSquare, int x, int y)
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
// ********************************************************************************
//calculate the accuracy of the rules at classifying the data entries correctly.
void BruteForce::createRules()
{
	int pointsFromClass = 0;
	int totalPointsInSquare = 0;


	// ----------------------- RULE 1 For all Classes -----------------------
	//for each class
	for (int i = 0; i < dominantSquares.size(); i++)
	{
		// Declare the rule.
		Rule rule;

		DominantSquare tempSquare;
		double bestPurity = 0;
		//for each plane
		for (int d = 0; d < dominantSquares[i].size(); d++)
		{	//pick the highest accuracy dom square
			if (dominantSquares[i][d].purity > bestPurity)
			{
				bestPurity = dominantSquares[i][d].purity;
				tempSquare = dominantSquares[i][d];

			}
		}

		// Set limits of dominant square.
		float xMin = tempSquare.xMin;
		float xMax = tempSquare.xMax;
		float yMin = tempSquare.yMin;
		float yMax = tempSquare.yMax;
		int dimX = tempSquare.dimensionX;
		int dimY = tempSquare.dimensionY;

		// for each class in testing.
		for (int j = 0; j < testing.size() && j < 2; j++) // j < 2 is Classes Limiter!!
		{
			// Create Dataclass to hold all the points in the dominant square for this rule.
			DataClass tempDataClass;
			tempDataClass.className = testing[j].className;

			// for each point of that class.
			for (int p = 0; p < testing[j].classDataset.size(); p++)
			{
				// Get the point's x,y dimensions and classname it belongs to.
				float x = testing[j].classDataset[p][dimX];
				float y = testing[j].classDataset[p][dimY];
				std::string pointClassname = testing[j].className;
				
				// If the point is withing dominant square limits.
				if (xMin <= x && x <= xMax && yMin <= y && y <= yMax)
				{
					// If the point has the same classname as the dominant square.
					if (tempSquare.className.compare(pointClassname) == 0)
						pointsFromClass++; // Increment the points for the class.
					totalPointsInSquare++; // Increment the total points in the square.

					// Add the point to the list of points in dominant square rule.
					tempDataClass.classDataset.push_back(testing[j].classDataset[p]);
				}
			}

			// Push the dataclass with the points to the rule's list of points.
			if(tempDataClass.classDataset.size() > 0)
				rule.pointsInRule.push_back(tempDataClass);
		}

		// Set the rule variables.
		rule.ruleNum = 1;
		rule.className = tempSquare.className;
		rule.in.push_back(tempSquare);
		rule.accuracy = 100 * (pointsFromClass / (double)totalPointsInSquare);

		// Add the rule to the list of rules.
		rules.push_back(rule);

		// Reset points in square;
		pointsFromClass = 0;
		totalPointsInSquare = 0;
	} // ----------------------------------------------------------------------

	// ----------------------- RULE 2 For all Classes -----------------------
	for (int r = 0, ruleSize = rules.size(); r < ruleSize; r++)
	{
		std::cout << "\n********* RULE 2 ************\n";
		Rule rule = rules[r];
		rule.ruleNum = 2;
		rule.accuracy = 0;

		if (rule.pointsInRule.size() == 1) // Add more dominant squares to include!!
		{
			std::cout << "\n********* INCLUDE ************\n";
			// Get the dominant square index by classname.
			int domSquIndex;
			for (int dS = 0; dS < dominantSquares.size(); dS++)
			{
				if (dominantSquares[dS][0].className.compare(rule.className) == 0)
				{
					domSquIndex = dS;
					break;
				}
			}

			DominantSquare tempSquare;
			double bestPurity = 0;
			//for each plane
			for (int d = 0; d < dominantSquares[domSquIndex].size(); d++)
			{	//pick the highest accuracy dom square
				if (dominantSquares[domSquIndex][d].purity > bestPurity)
				{
					// Make sure this dominant square isn't already included in the rule's in vector list.
					for (int rI = 0; rI < rule.in.size(); rI++)
					{
						if (dominantSquares[domSquIndex][d].planeIndex != rule.in[rI].planeIndex)
						{
							bestPurity = dominantSquares[domSquIndex][d].purity;
							tempSquare = dominantSquares[domSquIndex][d];
						}
					}
				}
			}

			// Set limits of dominant square.
			float xMin = tempSquare.xMin;
			float xMax = tempSquare.xMax;
			float yMin = tempSquare.yMin;
			float yMax = tempSquare.yMax;
			int dimX = tempSquare.dimensionX; // Plane dimension x
			int dimY = tempSquare.dimensionY; // Plane dimension y

			// for each class in testing.
			for (int j = 0; j < testing.size() && j < 2; j++) // j < 2 is Classes Limiter!!
			{
				// Create Dataclass to hold all the points in the dominant square for this rule.
				DataClass tempDataClass;
				tempDataClass.className = testing[j].className;

				// for each point of that class.
				for (int p = 0; p < testing[j].classDataset.size(); p++)
				{
					// Get the point's x,y dimensions and classname it belongs to.
					float x = testing[j].classDataset[p][dimX];
					float y = testing[j].classDataset[p][dimY];
					std::string pointClassname = testing[j].className;

					// If the point is withing dominant square limits.
					if (xMin <= x && x <= xMax && yMin <= y && y <= yMax)
					{
						// If the point has the same classname as the dominant square.
						if (tempSquare.className.compare(pointClassname) == 0)
							pointsFromClass++; // Increment the points for the class.
						totalPointsInSquare++; // Increment the total points in the square.

						// Add the point to the list of points in dominant square rule.
						tempDataClass.classDataset.push_back(testing[j].classDataset[p]);
					}
				}

				// Push the dataclass with the points to the rule's list of points.
				if (tempDataClass.classDataset.size() > 0)
					rule.pointsInRule.push_back(tempDataClass);
			}

			// Set the rule variables.
			rule.in.push_back(tempSquare);
			rule.accuracy = 100 * (pointsFromClass / (double)totalPointsInSquare);

			// Add the rule to the list of rules.
			rules.push_back(rule);

			// Reset points in square;
			pointsFromClass = 0;
			totalPointsInSquare = 0;
		}

		else // Else Add more dominant squares to exclude!!
		{
			std::cout << "\n********* EXCLUDE ************\n";
			// For all the classes of the list of points in rule.
			for (int rP = 0; rP < rule.pointsInRule.size(); rP++)
			{
				// If the points classname isn't the same as the rule's classname
				// and the dataset with those points is NOT empty.
				if (rule.pointsInRule[rP].className.compare(rule.className) != 0 && rule.pointsInRule[rP].classDataset.size() > 0)
				{
					// Get the dominant square index by classname.
					int domSquIndex;
					for (int dS = 0; dS < dominantSquares.size(); dS++)
					{
						if (dominantSquares[dS][0].className.compare(rule.pointsInRule[rP].className) == 0)
						{
							domSquIndex = dS;
							break;
						}
					}

					// For every dominant square in that list.
					std::vector<DominantSquare> squares;
					for (int s = 0; s < dominantSquares[domSquIndex].size(); s++)
					{
						int pointsInSquare = 0;
						int totalPoints = 0;

						DominantSquare tempSquare = dominantSquares[domSquIndex][s];

						// Set limits of dominant square.
						float xMin = tempSquare.xMin;
						float xMax = tempSquare.xMax;
						float yMin = tempSquare.yMin;
						float yMax = tempSquare.yMax;
						int dimX = tempSquare.dimensionX; // Plane dimension x
						int dimY = tempSquare.dimensionY; // Plane dimension y

						// For every point in that class.
						for (int p = 0; p < rule.pointsInRule[rP].classDataset.size(); p++)
						{
							// Get the point's x,y dimensions and classname it belongs to.
							float x = rule.pointsInRule[rP].classDataset[p][dimX];
							float y = rule.pointsInRule[rP].classDataset[p][dimY];

							// If the point is withing dominant square limits.
							if (xMin <= x && x <= xMax && yMin <= y && y <= yMax)
								pointsInSquare++; // Increment the points for the class.
							totalPoints++; // Increment the total points in the square.
						}

						tempSquare.purity = 100 * (pointsInSquare / (double)totalPoints);
						squares.push_back(tempSquare);
						std::cout << "Exclude Purity: " << tempSquare.purity << "\tClass Points in Square: " << pointsInSquare;
						std::cout << "\tTotal Points: " << totalPoints << std::endl;
					}

					// Pick the dominant square with the highest purity to exclude.
					DominantSquare tempSquare;
					int domSquareIndex2;
					double bestPurity = 0;
					//for each plane
					for (int d = 0; d < squares.size(); d++)
					{	//pick the highest accuracy dom square
						if (squares[d].purity > bestPurity)
						{
							// Make sure this dominant square isn't already included in the rule's out vector list.
							for (int rO = 0; rO < rule.in.size(); rO++)
							{
								if (squares[d].planeIndex != rule.in[rO].planeIndex)
								{
									bestPurity = squares[d].purity;
									domSquareIndex2 = d;
								}
							}
						}
					}
					
					// Set the tempSquare using domSquareIndex.
					tempSquare = dominantSquares[domSquIndex][domSquareIndex2];

					// Set limits of dominant square.
					float xMin = tempSquare.xMin;
					float xMax = tempSquare.xMax;
					float yMin = tempSquare.yMin;
					float yMax = tempSquare.yMax;
					int dimX = tempSquare.dimensionX; // Plane dimension x
					int dimY = tempSquare.dimensionY; // Plane dimension y

					// For every point in that class.
					std::vector<std::vector<float>> newdataset;
					for (int p = 0; p < rule.pointsInRule[rP].classDataset.size(); p++)
					{
						// Get the point's x,y dimensions and classname it belongs to.
						float x = rule.pointsInRule[rP].classDataset[p][dimX];
						float y = rule.pointsInRule[rP].classDataset[p][dimY];

						// If the point is outside dominant square limits.
						if (x < xMin || x > xMax || y < yMin || y > yMax)
							newdataset.push_back(rule.pointsInRule[rP].classDataset[p]);
					}

					// Update the new dataset of pointsInRule in rule.
					rule.pointsInRule[rP].classDataset = newdataset;

					// Set the rule variables.
					rule.out.push_back(tempSquare);

					// Update the rule accuracy.
					pointsFromClass = 0;
					totalPointsInSquare = 0;
					for (int ps = 0; ps < rule.pointsInRule.size(); ps++)
					{
						if (rule.pointsInRule[ps].className.compare(rule.className) == 0)
							pointsFromClass += rule.pointsInRule[ps].classDataset.size();
						totalPointsInSquare += rule.pointsInRule[ps].classDataset.size();
					}
					rule.accuracy = 100 * (pointsFromClass / (double)totalPointsInSquare);
				}
			}
			// Add the rule to the list of rules.
			rules.push_back(rule);
		}
	}
	// ----------------------------------------------------------------------
}
//function to link rectangles to maximize purity by being inclusive and exclusive of other rectangles


/*
double BruteForce::testRule()
{

	Rule rule;
	rule.className = classes[0].className;
	rule.in.push_back(dominantPlanes[0].domSquares[0]);
	rule.out.push_back(dominantPlanes[0].domSquares[1]);
	int countCorrect = 0;
	int countFalse = 0;
	bool checkNext = false;
	for (int j = 0; j < testing.size(); j++)
	{
		for (int i = 0; i < testing[j].classDataset.size(); i++)
		{
			checkNext = checkIn(rule, countFalse, j, i);
			if (checkNext)
			{
				checkNext = checkOut(rule, countFalse, checkNext, j, i);
				if (checkNext)
					countCorrect++;
			}
		}
	}
	cout << "\n\n" << countCorrect << endl;
	cout << countFalse << endl;
	return countCorrect;
}

bool BruteForce::checkIn(Rule rule, int &countFalse, int j, int i)
{
	bool checkNext = false;
	for (int k = 0; k < rule.in.size(); k++)
	{
		float x = testing[j].classDataset[i][dominantPlanes[rule.in[k].planeIndex].dimensionX];
		float y = testing[j].classDataset[i][dominantPlanes[rule.in[k].planeIndex].dimensionY];
		float xMin = rule.in[k].xMin;
		float xMax = rule.in[k].xMax;
		float yMin = rule.in[k].yMin;
		float yMax = rule.in[k].yMax;
		if (xMin <= x && x <= xMax
			&& yMin <= y && y <= yMax)
		{
			checkNext = true;
		}
		else
		{
			countFalse++;
			checkNext = false;
		}
	}

	return checkNext;
}

bool BruteForce::checkOut(Rule rule, int &countFalse, bool &checkNext, int j, int i)
{
	for (int k = 0; k < rule.out.size(); k++)
	{
		float x = testing[j].classDataset[i][dominantPlanes[rule.out[k].planeIndex].dimensionX];
		float y = testing[j].classDataset[i][dominantPlanes[rule.out[k].planeIndex].dimensionY];
		float xMin = rule.out[k].xMin;
		float xMax = rule.out[k].xMax;
		float yMin = rule.out[k].yMin;
		float yMax = rule.out[k].yMax;
		if (x < xMin || x > xMax
			|| yMin < y || y > yMax) // if outside.
		{
			checkNext = true;
		}
		else
			checkNext = false;
		if (!checkNext)
		{
			countFalse++;
			break;
		}
	}

	return checkNext;
}
*/