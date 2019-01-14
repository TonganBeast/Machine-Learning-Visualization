#include "MD_Graph.h"

//constructor accepts name of the file containing multi-dimensional
//data points as an argument, and then parses the file to determine
//the constraints of the multi-dimensional graph
MD_Graph::MD_Graph(std::string fileName) {
	//find the number of dimensions through reading the file
	initPoints(fileName);
	//calculate the number of planes using the number of dimensions
	numPlanes = calcNumPlanes(numDimensions);
	//create the combinations of axes which will be plotted on planes
	combineDimensions();
	//
}

//function calculates the number of planes needed to display the given
//data set, and determines if one dimension will need to be reused to
//consistently plot data to two-dimensional planes
int MD_Graph::calcNumPlanes(int nD) {
	//determine whether the number of dimensions is odd or even
	if (nD % 2 != 0) {
		//number of dimensions is odd
		reuseDimension = true;
		return ((nD - 1) / 2);
	}
	else
		//number of dimensions is even
		return (nD / 2);
}

//function will use numPlanes to create as many combinations of
//axes which will be used to plot the given data set
void MD_Graph::combineDimensions() {
	//instantiate the dimCombos variable to hold numPlanes 
	//combinations of 2 axes
	dimCombos = new int*[numPlanes];
	for (int i = 0; i < numDimensions; i++)
		dimCombos[i] = new int[2];

	//initialize rand
	srand(time(NULL));

	//initialize all elements of dimCombos to -1 in order to
	//compare the available axes later on
	for (int i = 0; i < numPlanes; i++) {
		dimCombos[i][0] = -1;
		dimCombos[i][1] = -1;
	}

	//create planes by sequence *CHANGE TO RANDOM LATER*
	int j = 0;
	for (int i = 0; i < numPlanes; i++) {
		dimCombos[i][0] = j;
		j++;
		dimCombos[i][1] = j;
		j++;
	}
}

//function checks that axis chosen by combineDimensions function
//have not already been paired with another axis for representation
//on a plane (unless there are an odd number of dimensions)
bool MD_Graph::axisIsAvailable(int axis) {
	//iterate through dimCombos to check that an axis has not
	//already been used
	for (int i = 0; i < numDimensions; i++) {
		//return true if a -1 has been reached as there will be no
		//further generated combinations of axes
		if (dimCombos[i][0] == -1)
			return true;
		//if the axis already exists within dimCombos, return false
		if (dimCombos[i][0] == axis || dimCombos[i][1] == axis) {
			return false;
		}
	}
	//if this point is reached for any reason, return true
	return true;
}

//method reads data from the user-given file and stores it
//within the points matrix
void MD_Graph::readData(std::string fileName) {
	//initialize file and variables needed for file-reading
	int lineIndex = 0, elementIndex = 0;
	float* currentPoint = new float[numDimensions];
	std::string line;
	std::ifstream inFile;
	inFile.open(fileName);

	//read file
	while (inFile.good()) {
		std::getline(inFile, line);
		int numStart = 0, numEnd;
		for (int i = 0; i < line.length(); i++) {
			if (line[i] == ',') {
				numEnd = i;
				currentPoint[elementIndex] = stof(line.substr(numStart, numEnd - numStart));
			}
		}
	}

	delete[] currentPoint;
}

//method uses numDimensions and numMD_Points to initialize the points
//matrix which will hold the user-given file's data for use
void MD_Graph::initPoints(std::string fileName) {
	findNumDimensions(fileName);
	findNumPoints(fileName);

	points = new float*[numDimensions];
	for (int i = 0; i < numDimensions; i++)
		points[i] = new float[numMD_Points];
}

void MD_Graph::findNumDimensions(std::string fileName) {
	//establish and open file
	std::ifstream inFile;
	std::string line;

	//starting from one, count the number of commas in the first line
	//of the file to determine the number of dimensions in the data
	numDimensions = 1;
	if (inFile.good()) {
		std::getline(inFile, line);
		for (int i = 0; i < line.length; i++)
			if (line[i] == ',')
				numDimensions++;
	}
	//return to beginning of file
	inFile.clear();
	inFile.seekg(0, std::ios::beg);

	//close file
	inFile.close();
}

void MD_Graph::findNumPoints(std::string fileName) {
	//establish and open file
	std::ifstream inFile;
	std::string line;

	//establish and open file
	std::ifstream inFile;
	std::string line;

	//find the number of multi-dimensional points in the given
	//file by iterating through and counting the number of lines
	inFile.open(fileName);
	numMD_Points = 0;
	while (std::getline(inFile, line))
		++numMD_Points;

	//return to the beginning of the file
	inFile.clear();
	inFile.seekg(0, std::ios::beg);

	//close file
	inFile.close();
}