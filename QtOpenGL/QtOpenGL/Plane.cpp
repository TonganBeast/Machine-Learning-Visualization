#include "Plane.h"
#include <iostream>

Plane::Plane() {}

Plane::Plane(int planePosition, int numPoints, int xDimension, int yDimension, std::vector<float> xPoints,
	std::vector<float> yPoints, std::vector<Classification> pointClassifications) {
	this->numPoints = numPoints;
	this->planeX = xDimension;
	this->planeY = yDimension;
	int a = planePosition;
	this->planeIndex = a;
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

Plane::Plane(int planePosition, int numPoints, int xDimension, int yDimension, std::vector<Point> points, float relativeHeight, float relativeWidth) {
	this->numPoints = numPoints;
	this->planeX = xDimension;
	this->planeY = yDimension;
	this->planeIndex = planePosition;
	this->twoD_Points_v = points;
	this->relativeHeight = relativeHeight;
	this->relativeWidth = relativeWidth;
}

Plane::Plane(const Plane& other) {
	this->planeIndex = other.planeIndex;
	this->numPoints = other.numPoints;
	this->planeX = other.planeX;
	this->planeY = other.planeY;
	this->twoD_Points_v = other.twoD_Points_v;
	this->relativeHeight = other.relativeHeight;
	this->relativeWidth = other.relativeWidth;
}

void Plane::listPoints() {}

Plane& Plane::operator=(const Plane& other){
	planeIndex = other.planeIndex;
	numPoints = other.numPoints;
	planeX = other.planeX;
	planeY = other.planeY;
	twoD_Points_v = other.twoD_Points_v;
	relativeHeight = other.relativeHeight;
	relativeWidth = other.relativeWidth;
	return *this;
}

Plane Plane::operator=(Plane&& other) {
	int pix, numpts, px, py;
	std::vector<Point> pts;
	float rw, rh;
	pix = other.planeIndex;
	numpts = other.numPoints;
	px = other.planeX;
	py = other.planeY;
	pts = other.twoD_Points_v;
	rw = other.relativeWidth;
	rh = other.relativeHeight;
	other.planeIndex = 0;
	other.numPoints = 0;
	other.planeX = 0;
	other.planeY = 0;
	other.twoD_Points_v.clear();
	other.relativeWidth = 0;
	other.relativeHeight = 0;
	return Plane(pix, numpts, px, py, pts, rh, rw);
}

void Plane::drawPlane(int planeIndex) {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f((float)(windowOffset + (planeIndex * realWidth)), (float)50);
	glVertex2f((float)(windowOffset + (planeIndex * realWidth) + realWidth), (float)50);
	glVertex2f((float)(windowOffset + (planeIndex * realWidth)), (float)50);
	glVertex2f((float)(windowOffset + (planeIndex * realWidth)), (float)(50 + realHeight));
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
	float x = ((windowOffset + (planeIndex * realWidth)) + ((planeIndex * realWidth) + realWidth)) / 2.0;
	float y = (float)30.0;
	glRasterPos2f(x, y);
	unsigned char* str = new unsigned char[6];
	str[0] = 'X'; str[1] = digits[planeX]; str[2] = ','; str[3] = char(32); str[4] = 'X'; str[5] = digits[planeY];
	for (int i = 0; i < 6; i++) {
		//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
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
	this->relativeWidth = largestX + (largestX * 0.1);
	this->relativeHeight = largestY + (largestY * 0.1);
}

float Plane::getRelativeWidth() {
	return relativeWidth;
}

float Plane::getRelativeHeight() {
	return relativeHeight;
}

Plane& Plane::getPlane() {
	return *this;
}