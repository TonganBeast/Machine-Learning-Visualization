#include "MD_Graph.h"

//constructor accepts name of the file containing multi-dimensional
//data points as an argument, and then parses the file to determine
//the constraints of the multi-dimensional graph
MD_Graph::MD_Graph(std::string fileName) {
	//find the number of dimensions in the file
	int numDimensions = findNumDimensions(fileName);
	//find the number of points in the file
	int numPoints = findNumPoints(fileName);
	//calculate the number of planes using the number of dimensions
	numPlanes = calcNumPlanes(numDimensions);
	//create the combinations of axes which will be plotted on planes
	std::vector<std::vector<int>> dimCombos;
	dimCombos = combineDimensions(numPlanes, numDimensions);
	std::cout << "dimension combinations: " << std::endl;
	for (int i = 0; i < numPlanes; i++) {
		std::cout << dimCombos[i][0] << ", " << dimCombos[i][1] << std::endl;
	}
	findClassifications(fileName, numDimensions);


	std::cout << "number of different classifications: " << uniqueClassifications.size() << std::endl;
	associateColors();
	for (int i = 0; i < numMD_Points; i++) {
		std::cout << classificationForPoints[i].getTitle() << std::endl;
	}
	//updatePtsClassification();
	//read data from file, and then parse it
	std::vector<std::vector<float>> points = parseData(readData_v(fileName), numDimensions);
	
	/*for (int i = 0; i < numDimensions; i++) {
		for (int j = 0; j < numPoints; j++) {
			std::cout << points[i][j] << "   ";
		}
		std::cout << std::endl;
	}*/

	//create a vector of planes and build them
	std::vector<Plane> planes(numPlanes);
	for (int i = 0; i < numPlanes; i++) {
		planes[i] = Plane(i, numPoints, dimCombos[i][0], dimCombos[i][1], points[dimCombos[i][0]], points[dimCombos[i][1]], classificationForPoints);
		printPlanePts(i);
		std::cout << "plane " << i << " relative bounds: (" << planes[i].getRelativeWidth() << ", " << planes[i].getRelativeHeight() << ")" << std::endl;
	}

	//build and display the multi-dimensional graph
	buildGraph(planes, numPlanes, numPoints);
}

//function calculates the number of planes needed to display the given
//data set, and determines if one dimension will need to be reused to
//consistently plot data to two-dimensional planes
int MD_Graph::calcNumPlanes(int nD) {
	//determine whether the number of dimensions is odd or even
	if (nD % 2 != 0) {
		//number of dimensions is odd
		reuseDimension = true;
		return ((nD + 1) / 2);
	}
	else
		//number of dimensions is even
		return (nD / 2);
}

//function will use numPlanes to create as many combinations of
//axes which will be used to plot the given data set
std::vector<std::vector<int>> MD_Graph::combineDimensions(int numPlanes, int numDimensions) {
	//instantiate the dimCombos variable to hold numPlanes 
	//combinations of 2 axes
	std::vector<std::vector<int>> dimCombos(numPlanes, std::vector<int> (2));

	//generate an array with all dimensions (and one more if the number
	//of dimensions is odd)
	int *dimensions;
	if (numDimensions % 2 == 0) {
		dimensions = new int[numDimensions];
		for (int i = 0; i < numDimensions; i++) {
			dimensions[i] = i;
		}
	}
	else {
		dimensions = new int[numDimensions + 1];
		for (int i = 0; i <= numDimensions; i++) {
			if (i == numDimensions) {
				srand(time(NULL));
				dimensions[i] = rand() % numDimensions;
			}
			else
				dimensions[i] = i;
		}
	}

	//generate a combination of dimensions using an in-place shuffle
	while (true) {
		if (randomizeDimensions(dimensions, numPlanes*2))
			break;
	}

	//finally, move these dimensions into the class variable dimCombos
	int d = 0;
	for(int i = 0; i < numPlanes; i++){
		dimCombos[i][0] = dimensions[d];
		d++;
		dimCombos[i][1] = dimensions[d];
		d++;
	}

	return dimCombos;
}

//function reads user-supplied .txt file and copies every line into a vector of
//strings to be parsed later. 
std::vector<std::string> MD_Graph::readData_v(std::string fileName) {
	//initialize file for reading
	std::vector<std::string> data;
	std::string line;
	std::ifstream in;
	in.open(fileName);
	while (std::getline(in, line)) {
		data.push_back(line);
	}
	return data;
}

//function takes in data read from readData function and parses the strings for the
//desired numerical values, then returns them in a 2D vector points[numDimensions][numPoints]
std::vector<std::vector<float>> MD_Graph::parseData(std::vector<std::string> data, int numDimensions) {
	//initialize variables for tokenizing data's strings
	char* cLine;
	char* cTokens;
	char* cValue;
	float fValue;
	std::string line;

	//initialize a vector matrix with a number of rows equal to numDimensions
	std::vector<std::vector<float>> points(numDimensions);

	//iterate through data, tokenizing each line and converting those tokens to floats
	while (!data.empty()) {
		line = data.back();
		data.pop_back();
		cLine = new char[line.length()];
		strcpy_s(cLine, line.length() + 1, line.c_str());
		cTokens = strtok(cLine, ", ");
		cValue = cTokens;
		for (int row = 0; row < numDimensions; row++) {
			std::string strValue(cValue);
			//std::cout << strValue << ", ";
			fValue = std::stof(strValue);
			points[row].push_back(fValue);
			cValue = strtok(NULL, ", ");
		}
		std::cout << cValue << std::endl;
		classificationForPoints.push_back(Classification(cValue));

		//std::cout << std::endl;
	}

	return points;
}

//method reads data from the user-given file and stores it
//within the points matrix
void MD_Graph::readData(std::string fileName) {
	//initialize file and variables needed for file-reading
	int elementIndex = 0;
	float* currentPoint = new float[numDimensions];
	std::string line;
	std::ifstream in;
	in.open(fileName);

	//read file
	char* cLine;
	char* cTokens;
	char* cValue;
	float fValue;
	std::cout << "--- READ NUMBERS ---" << std::endl;
	while (std::getline(in, line)) {
		static int column = 0;
		cLine = new char[line.length()];
		strcpy(cLine, line.c_str());
		cTokens = strtok(cLine, ", ");
		cValue = cTokens;
		for (int row = 0; row < numDimensions + 1; row++) {
			if (row < numDimensions) {
				//std::cout << cValue << "   ";
				std::string strValue(cValue);
				fValue = std::stof(strValue);
				points[row][column] = fValue;
				std::cout << fValue << " saved in position [" << row << "][" << column << "]" << std::endl;
				if (!(column == numDimensions - 1))
					cValue = strtok(NULL, ", ");
			}
			else {
				classificationList[column] = std::string(cTokens);
			}
		}
		column++;
		std::cout << std::endl;
	}
	std::cout << "--------------------" << std::endl;
}

//method uses numDimensions and numMD_Points to initialize the points
//matrix which will hold the user-given file's data for use
void MD_Graph::initPoints(std::string fileName) {
	findNumDimensions(fileName);
	findNumPoints(fileName);
	std::cout << "numDimensions: " << numDimensions << std::endl;
	std::cout << "numMD_Points: " << numMD_Points << std::endl;
}

int MD_Graph::findNumDimensions(std::string fileName) {
	//establish and open file
	std::ifstream in;
	std::string line;

	//starting from one, count the number of commas in the first line
	//of the file to determine the number of dimensions in the data
	int numDimensions = 0;
	in.open(fileName);
	std::getline(in, line);
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == ',') {
			numDimensions++;
		}
	}

	//return to beginning of file
	in.clear();
	in.seekg(0, std::ios::beg);

	//close file
	in.close();

	return numDimensions;
}

int MD_Graph::findNumPoints(std::string fileName) {
	//establish and open file
	std::ifstream in;
	std::string line;

	//find the number of multi-dimensional points in the given
	//file by iterating through and counting the number of lines
	in.open(fileName);
	int numPoints = 0;
	while (std::getline(in, line)) {
		++numPoints;
	}

	//return to the beginning of the file
	in.clear();
	in.seekg(0, std::ios::beg);

	//close file
	in.close();

	return numPoints;
}

void MD_Graph::dimSwap(int* p, int* q) {
	int temp = *p;
	*p = *q;
	*q = temp;
}

//create a random permutation of the passed dimensions
bool MD_Graph::randomizeDimensions(int *dimensions, int numDim) {
	//initialize seed
	srand(time(NULL));

	//starting from the last element, swap with a randomly chosen element.
	for (int i = numDim - 1; i > 0; i--) {
		//randomly select an index from 0 to i
		int rng = rand() % (i + 1);

		//swap element at index i with element at index rng
		dimSwap(&dimensions[i], &dimensions[rng]);
	}

	//if there are an odd number of dimensions (meaning one dimension has
	//been included twice) check that the same dimension will not make be
	//used twice on the same plane.
	if (numDimensions % 2 != 0) {
		for (int i = 0; i < numDimensions; i++) {
			if (dimensions[i] == dimensions[i + 1])
				return false;
			else
				i++;
		}
	}
	return true;
}

void MD_Graph::buildGraph() {
	//first draw each plane and its points
	for (int i = 0; i < numPlanes; i++) {
		planes[i].drawPlane(i);
		planes[i].drawPoints(classificationList);
	}
	//next access the points on the planes and draw lines between them
	std::string classificationCheck, currentClassification = classificationList[0];
	for (int i = 0; i < numPlanes - 1; i++) {
		for (int j = 0; j < numMD_Points; j++) {
			classificationCheck = classificationList[j];
			float startX = planes[i].getPoint(j).getWorldX();
			float endX = planes[i + 1].getPoint(j).getWorldX();
			float startY = planes[i].getPoint(j).getWorldY();
			float endY = planes[i + 1].getPoint(j).getWorldY();
			/*std::cout << "Drawing line from (" << (float)planes[i].getPoint(j).getWorldX() << ", " << planes[i].getPoint(j).getWorldY() <<
				") to (" << planes[i + 1].getPoint(j).getWorldX() << ", " << planes[i + 1].getPoint(j).getWorldY() << ")" << std::endl;*/
			glBegin(GL_LINES);
			glVertex2f(startX, startY);
			glVertex2f(endX, endY);
			glEnd();
			glFlush();
		}
	}
}

void MD_Graph::buildGraph(std::vector<Plane> planes, int numPlanes, int numPoints) {
	//draw each plane and that plane's respective points
	for (int i = 0; i < numPlanes; i++) {
		planes[i].drawPlane(i);
		planes[i].drawPoints();
	}
	//next access the points on the planes and draw lines between them
	for (int i = 0; i < numPlanes - 1; i++) {
		for (int j = 0; j < numPoints; j++) {
			if (j < 50)
				glColor3f(1.0, 0.0, 0.0);
			else if (j >= 50 && j < 100)
				glColor3f(0.0, 1.0, 0.0);
			else
				glColor3f(0.0, 0.0, 1.0);
			float startX = planes[i].getPoint(j).getWorldX();
			float endX = planes[i + 1].getPoint(j).getWorldX();
			float startY = planes[i].getPoint(j).getWorldY();
			float endY = planes[i + 1].getPoint(j).getWorldY();
			/*std::cout << "Drawing line from (" << (float)planes[i].getPoint(j).getWorldX() << ", " << planes[i].getPoint(j).getWorldY() <<
				") to (" << planes[i + 1].getPoint(j).getWorldX() << ", " << planes[i + 1].getPoint(j).getWorldY() << ")" << std::endl;*/
			glBegin(GL_LINES);
			glVertex2f(startX, startY);
			glVertex2f(endX, endY);
			glEnd();
			glFlush();
		}
	}
}

void MD_Graph::printPlanePts(int planeIndex) {
	for (int i = 0; i < numMD_Points; i++)
		planes[planeIndex].getPoint(i).printPt();
}

void MD_Graph::findClassifications(std::string fileName, int numDimensions) {
	//prepare file for reading
	std::string line;
	char* cLine;
	char* cTokens = nullptr;
	std::string temp;
	std::ifstream in;
	in.open(fileName);
	//
	while (std::getline(in, line)) {
		//tokenize the line and then get the last token
		cLine = new char[line.length()];
		strcpy_s(cLine, line.length() + 1, line.c_str());
		cTokens = strtok(cLine, ", ");
		for(int i = 0; i < numDimensions; i++)
			cTokens = strtok(NULL, ", ");

		//std::cout << "Last token: " << cTokens << std::endl;
		//check that the currently identified class does not already exist within the
		//classifications vector
		if(uniqueClassifications.size() == 0)
			uniqueClassifications.push_back(Classification(cTokens, CLASS_RED));
		else if (!alreadyClassified(std::string(cTokens))) {
			std::cout << cTokens << " is not already classified.\n";
			if (uniqueClassifications.size() < 2)
				uniqueClassifications.push_back(Classification(cTokens, CLASS_GREEN));
			else
				uniqueClassifications.push_back(Classification(cTokens, CLASS_BLUE));
		}
	}
	//return to the beginning of the file
	in.clear();
	in.seekg(0, std::ios::beg);

	//close file
	in.close();
}

//return true if the given classification string already exists within the global vector
//of classifications
bool MD_Graph::alreadyClassified(std::string newClassification) {
	for (int i = 0; i < uniqueClassifications.size(); i++) {
		//if the two strings are the same, return true
		if (newClassification == uniqueClassifications[i].getClassificationTitle()) {
			//std::cout << newClassification << " is the same as " << classifications[i].getClassificationTitle() << std::endl;
			return true;
		}
		else ;
			//std::cout << newClassification << " is not the same as " << classifications[i].getClassificationTitle() << std::endl;
	}
	//if this point is reached, classification must not already exist within the vector
	return false;
}

void MD_Graph::associateColors() {
	uniqueClassifications[0].setColor(CLASS_RED);
	uniqueClassifications[1].setColor(CLASS_GREEN);
	uniqueClassifications[2].setColor(CLASS_BLUE);
}

void MD_Graph::updatePtsClassification() {
	for (int i = 0; i < numMD_Points; i++) {
		for (int j = 0; j < uniqueClassifications.size(); j++) {
			if (classificationForPoints[i].sameTitle(uniqueClassifications[j])) {
				classificationForPoints[i] = uniqueClassifications[j];
				std::cout << "classificationForPoints[" << i << "] now has color [" << classificationForPoints[i].getRed() << ", "
					<< classificationForPoints[i].getGreen() << ", " << classificationForPoints[i].getBlue() << "]" << std::endl;
			}
			if (i < 50 && j < 1)
				std::cout << classificationForPoints[i].getTitle() << " is not the same as " << uniqueClassifications[j].getTitle() << std::endl;
		}
	}
}