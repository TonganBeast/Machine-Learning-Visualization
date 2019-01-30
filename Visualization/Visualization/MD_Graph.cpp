#include "MD_Graph.h"

//constructor accepts name of the file containing multi-dimensional
//data points as an argument, and then parses the file to determine
//the constraints of the multi-dimensional graph
MD_Graph::MD_Graph(std::string fileName) {
	//find the number of dimensions through reading the file
	initPoints(fileName);
	//calculate the number of planes using the number of dimensions
	numPlanes = calcNumPlanes(numDimensions);
	setupDimCombos();
	std::cout << "numPlanes: " << numPlanes << std::endl;
	//create the combinations of axes which will be plotted on planes
	combineDimensions();
	std::cout << "dimension combinations: " << std::endl;
	for (int i = 0; i < numPlanes; i++) {
		std::cout << dimCombos[i][0] << ", " << dimCombos[i][1] << std::endl;
	}

	readData(fileName);

	std::cout << "--- SAVED NUMBERS ---" << std::endl;
	for (int column = 0; column < numDimensions; column++) {
		for (int row = 0; row < numMD_Points; row++) {
			std::cout << points[column][row] << "   ";
		}
		std::cout << std::endl;
	}

	//next, generate the planes
	planes = new Plane[numPlanes];
	for (int i = 0; i < numPlanes; i++) {
		planes[i] = Plane(i, numMD_Points, dimCombos[i][0], dimCombos[i][1], points[dimCombos[i][0]], points[dimCombos[i][1]]);
		std::cout << "---printPlanePts(i)---" << std::endl;
		printPlanePts(i);
	}
	//system("pause");
	//finally, connect the points on the planes
	buildGraph();
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

void MD_Graph::setupDimCombos() {
	dimCombos = new int*[numPlanes];
	for (int i = 0; i < numPlanes; i++) {
		dimCombos[i] = new int[2];
	}
}

//function will use numPlanes to create as many combinations of
//axes which will be used to plot the given data set
void MD_Graph::combineDimensions() {
	//instantiate the dimCombos variable to hold numPlanes 
	//combinations of 2 axes
	dimCombos = new int*[numPlanes];
	for (int i = 0; i < numDimensions; i++)
		dimCombos[i] = new int[2];

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
		for (int row = 0; row < numDimensions; row++) {
			//std::cout << cValue << "   ";
			std::string strValue(cValue);
			fValue = std::stof(strValue);
			points[row][column] = fValue;
			std::cout << fValue << " saved in position [" << row << "][" << column << "]" << std::endl;
			if (!(column == numDimensions - 1))
				cValue = strtok(NULL, ", ");
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

	points = new float*[numMD_Points];
	for (int i = 0; i < numDimensions; i++)
		points[i] = new float[numDimensions];
}

void MD_Graph::findNumDimensions(std::string fileName) {
	//establish and open file
	std::ifstream in;
	std::string line;

	//starting from one, count the number of commas in the first line
	//of the file to determine the number of dimensions in the data
	numDimensions = 1;
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
}

void MD_Graph::findNumPoints(std::string fileName) {
	//establish and open file
	std::ifstream in;
	std::string line;

	//find the number of multi-dimensional points in the given
	//file by iterating through and counting the number of lines
	in.open(fileName);
	numMD_Points = 0;
	while (std::getline(in, line)) {
		++numMD_Points;
	}

	//return to the beginning of the file
	in.clear();
	in.seekg(0, std::ios::beg);

	//close file
	in.close();
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
		planes[i].drawPoints();
	}
	//next access the points on the planes and draw lines between them
	for (int i = 0; i < numPlanes - 1; i++) {
		for (int j = 0; j < numMD_Points; j++) {
			float startX = planes[i].getPoint(j).getWorldX();
			float endX = planes[i + 1].getPoint(j).getWorldX();
			float startY = planes[i].getPoint(j).getWorldY();
			float endY = planes[i + 1].getPoint(j).getWorldY();
			std::cout << "Drawing line from (" << (float)planes[i].getPoint(j).getWorldX() << ", " << planes[i].getPoint(j).getWorldY() <<
				") to (" << planes[i + 1].getPoint(j).getWorldX() << ", " << planes[i + 1].getPoint(j).getWorldY() << ")" << std::endl;
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