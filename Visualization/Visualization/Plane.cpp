#include "Plane.h"
#include <iostream>

Plane::Plane() {}

Plane::Plane(int planePosition, int numMD_Points, int xDim, int yDim, float xPts[], float yPts[]) {
	numPoints = numMD_Points;
	planeX = xDim;
	planeY = yDim;
	planeIndex = planePosition;
	//generate two-dimensional points using the given arrays
	twoD_Points = new Point[numPoints];
	/*std::cout << "---Points in plane X" << xDim << ", X" << yDim << "---" << std::endl;
	for (int i = 0; i < numPoints; i++) {
		twoD_Points[i].setX(xPts[i], windowOffset, realWidth, planeIndex);
		twoD_Points[i].setY(yPts[i], windowOffset, realWidth, planeIndex);
		std::cout << "point " << i << ": " << xPts[i] << ", " << yPts[i] << std::endl;
	}*/
	this->listPoints();
}

Plane::Plane(int planePosition, int numPoints, int xDimension, int yDimension, std::vector<float> xPoints, 
	std::vector<float> yPoints, std::vector<Classification> pointClassifications) {
	this->numPoints = numPoints;
	this->planeX = xDimension;
	this->planeY = yDimension;
	this->planeIndex = planePosition;
	//copy the pointClassifications vector so as to not destroy the original
	std::vector<Classification> localPtClassification(pointClassifications);
	//determine this plane's relativeWidth and relativeHeight
	determineRelativeBounds(xPoints, yPoints);
	//create and initialize a vector to hold the given points
	twoD_Points_v = std::vector<Point>(numPoints);
	//generate two-dimensional points using the given vectors
	int pointIndex = 0;
	while (!(xPoints.empty()) && !(yPoints.empty())) {
		//get points at back of x & y vectors
		float x = xPoints.back();
		float y = yPoints.back();
		//remove points from those vectors
		xPoints.pop_back();
		yPoints.pop_back();
		//get that point's classification
		Classification c = localPtClassification.back();
		//remove that classification from the vector
		localPtClassification.pop_back();
		//set points' values
		twoD_Points_v[pointIndex].setCoordX(x);
		twoD_Points_v[pointIndex].setCoordY(y);
		//set point's classification
		twoD_Points_v[pointIndex].setClassification(c);
		//set the point's world coordinates
		twoD_Points_v[pointIndex].calcWorldPosition(relativeWidth, relativeHeight, realWidth, windowOffset, planeIndex);
		//increment pointIndex
		pointIndex++;
	}
}

void Plane::listPoints() {
	for (int i = 0; i < numPoints; i++) {
		//std::cout << "(" << twoD_Points_v[i].getPoint().getWorldX() << ", " << twoD_Points_v[i].getPoint().getWorldY() << ")" << std::endl;
	}
}

void Plane::drawPlane(int planeIndex) {
	std::cout << "Building plane with 0, 0 at: " << (windowOffset + (planeIndex * this->realWidth)) << ", 100" << std::endl;
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f((float)(windowOffset + (planeIndex * realWidth)), (float)50);
	glVertex2f((float)(windowOffset + (planeIndex * realWidth) + realWidth), (float)50);
	glVertex2f((float)(windowOffset + (planeIndex * realWidth)), (float)50);
	glVertex2f((float)(windowOffset + (planeIndex * realWidth)), (float)(50 + realHeight));
	glEnd();
	glFlush();
	printDimensions();
}

void Plane::drawPoints(std::string *classificationList) {
	std::string currentClassification = classificationList[0];
	std::string classificationCheck;
	glBegin(GL_POINTS);
	for (int i = 0; i < numPoints; i++) {
		classificationCheck = classificationList[i];
		if(i < 50)
			glColor3f(1.0, 0.0, 0.0);
		if (i >= 50 && i < 100)
			glColor3f(0.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.0, 1.0);

		twoD_Points[i].getPoint();
		twoD_Points[i].getPoint().getWorldX();
		twoD_Points[i].getPoint().getWorldY();
		glVertex2f(twoD_Points[i].getWorldX(), twoD_Points[i].getWorldY());
	}
	glEnd();
	glFlush();
}

void Plane::drawPoints() {
	glBegin(GL_POINTS);
	for (int i = 0; i < numPoints; i++) {
		glColor3f(twoD_Points_v[i].getPoint().getClassification().r, twoD_Points_v[i].getPoint().getClassification().g, twoD_Points_v[i].getPoint().getClassification().b);
		twoD_Points_v[i].getPoint();
		twoD_Points_v[i].getPoint().getWorldX();
		twoD_Points_v[i].getPoint().getWorldY();
		glVertex2f(twoD_Points_v[i].getWorldX(), twoD_Points_v[i].getWorldY());
	}
	glEnd();
	glFlush();
}

Point Plane::getPoint(int index) {
	return this->twoD_Points_v[index];
}

void Plane::printDimensions() {
	unsigned char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	std::cout << "X" << planeX << ", X" << planeY << std::endl;
	float x = ((windowOffset + (planeIndex * realWidth)) + ((planeIndex * realWidth) + realWidth)) / 2.0;
	float y = (float)30.0;
	glRasterPos2f(x, y);
	unsigned char* str = new unsigned char[6];
	str[0] = 'X'; str[1] = digits[planeX]; str[2] = ','; str[3] = char(32); str[4] = 'X'; str[5] = digits[planeY];
	for (int i = 0; i < 6; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
	}
}

/* function will read through both vectors, find the maximum value for the x and y
   values, and set the relativeWidth and relativeHeight so that the plane may scale
   all of its points' worldX and worldY values accordingly. */
void Plane::determineRelativeBounds(std::vector<float> x, std::vector<float> y) {
	float largestX = 0, largestY = 0;
	//read through x and y values, saving the largest of each
	for (int i = 0; i < x.size(); i++) {
		if (x[i] > largestX)
			largestX = x[i];
		if (y[i] > largestY)
			largestY = y[i];
	}
	//set the relativeWidth and relativeHeight to 110% of their corresponding largest values
	relativeWidth = largestX + (largestX * 0.1);
	relativeHeight = largestY + (largestY * 0.1);
}

float Plane::getRelativeWidth() {
	return relativeWidth;
}

float Plane::getRelativeHeight() {
	return relativeHeight;
}