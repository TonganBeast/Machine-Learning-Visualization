#include "stdafx.h"
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

Plane& Plane::operator=(const Plane& other) {
	planeIndex = other.planeIndex;
	numPoints = other.numPoints;
	planeX = other.planeX;
	planeY = other.planeY;
	twoD_Points_v = other.twoD_Points_v;
	relativeHeight = other.relativeHeight;
	relativeWidth = other.relativeWidth;
	return *this;
}

Plane& Plane::operator=(Plane&& other) {
	planeIndex = other.planeIndex;
	numPoints = other.numPoints;
	planeX = other.planeX;
	planeY = other.planeY;
	twoD_Points_v = other.twoD_Points_v;
	relativeWidth = other.relativeWidth;
	relativeHeight = other.relativeHeight;
	other.planeIndex = 0;
	other.numPoints = 0;
	other.planeX = 0;
	other.planeY = 0;
	other.twoD_Points_v.clear();
	other.relativeWidth = 0;
	other.relativeHeight = 0;
	return *this;
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

	writeIndeces();
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

float Plane::planeMidpoint() {
	float x = ((windowOffset + (planeIndex * realWidth)) + ((planeIndex * realWidth) + realWidth)) / 2.0;
	return x;
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

int Plane::getXindex() {
	return planeX;
}

int Plane::getYindex() {
	return planeY;
}

void Plane::writeIndeces() {
	glLineWidth(1.0);
	float xx = this->planeMidpoint() - ((5 * 10) / 2);
	float yy = 40.0;
	writeX(xx, yy);
	xx = xx + 10;
	switch (planeX) {
	case 0:
		writeZero(xx, yy);
		break;
	case 1:
		writeOne(xx, yy);
		break;
	case 2:
		writeTwo(xx, yy);
		break;
	case 3:
		writeThree(xx, yy);
		break;
	case 4:
		writeFour(xx, yy);
		break;
	case 5:
		writeFive(xx, yy);
		break;
	case 6:
		writeSix(xx, yy);
		break;
	case 7:
		writeSeven(xx, yy);
		break;
	case 8:
		writeEight(xx, yy);
		break;
	case 9:
		writeNine(xx, yy);
		break;
	}
	xx += 10;
	writeComma(xx, yy);
	xx += 10;
	writeX(xx, yy);
	xx += 10;
	switch (planeY) {
	case 0:
		writeZero(xx, yy);
		break;
	case 1:
		writeOne(xx, yy);
		break;
	case 2:
		writeTwo(xx, yy);
		break;
	case 3:
		writeThree(xx, yy);
		break;
	case 4:
		writeFour(xx, yy);
		break;
	case 5:
		writeFive(xx, yy);
		break;
	case 6:
		writeSix(xx, yy);
		break;
	case 7:
		writeSeven(xx, yy);
		break;
	case 8:
		writeEight(xx, yy);
		break;
	case 9:
		writeNine(xx, yy);
		break;
	}
	glLineWidth(2.0);
}

//the following functions use OpenGL to output simple, boxed numbers
//to independently handle labelling planes with their MD_Point's indeces.
//All characters are drawn to fit an 8x12 pixel rectangle
//All functions have their origin at the top-left
void Plane::writeZero(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);			//start
	glVertex2f(x, y - 12);		//end
	glVertex2f(x, y - 12);		//start
	glVertex2f(x + 8, y - 12);	//end
	glVertex2f(x + 8, y - 12);	//start
	glVertex2f(x + 8, y);		//end
	glVertex2f(x + 8, y);		//start
	glVertex2f(x, y - 12);		//end
	glVertex2f(x + 8, y);		//start
	glVertex2f(x, y);			//end
	glEnd();
	glFlush();
}

void Plane::writeOne(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);			//start
	glVertex2f(x + 4, y);		//end
	glVertex2f(x + 4, y);		//start
	glVertex2f(x + 4, y - 12);	//end
	glVertex2f(x, y - 12);		//start
	glVertex2f(x + 8, y - 12);	//end
	glEnd();
	glFlush();
}

void Plane::writeTwo(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);			//start
	glVertex2f(x + 8, y);		//end
	glVertex2f(x + 8, y);		//start
	glVertex2f(x + 8, y - 6);	//end
	glVertex2f(x + 8, y - 6);	//start
	glVertex2f(x, y - 6);		//end
	glVertex2f(x, y - 6);		//start
	glVertex2f(x, y - 12);		//end
	glVertex2f(x, y - 12);		//start
	glVertex2f(x + 8, y - 12);	//end
	glEnd();
	glFlush();
}

void Plane::writeThree(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);			//start
	glVertex2f(x + 8, y);		//end
	glVertex2f(x + 8, y);		//start
	glVertex2f(x + 8, y - 12);	//end
	glVertex2f(x + 8, y - 12);	//start
	glVertex2f(x, y - 12);		//end
	glVertex2f(x + 1, y - 5);	//start
	glVertex2f(x + 8, y - 5);	//end
	glEnd();
	glFlush();
}

void Plane::writeFour(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);			//start
	glVertex2f(x, y - 6);		//end
	glVertex2f(x, y - 6);		//start
	glVertex2f(x + 8, y - 6);	//end
	glVertex2f(x + 8, y);		//start
	glVertex2f(x + 8, y - 12);	//end
	glEnd();
	glFlush();
}

void Plane::writeFive(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + 8, y);
	glVertex2f(x, y);
	glVertex2f(x, y - 6);
	glVertex2f(x, y - 6);
	glVertex2f(x + 8, y - 6);
	glVertex2f(x + 8, y - 6);
	glVertex2f(x + 8, y - 12);
	glVertex2f(x + 8, y - 12);
	glVertex2f(x, y - 12);
	glEnd();
	glFlush();
}

void Plane::writeSix(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + 8, y);
	glVertex2f(x, y);
	glVertex2f(x, y - 12);
	glVertex2f(x, y - 12);
	glVertex2f(x + 8, y - 12);
	glVertex2f(x + 8, y - 12);
	glVertex2f(x + 8, y - 6);
	glVertex2f(x + 8, y - 6);
	glVertex2f(x, y - 6);
	glEnd();
	glFlush();
}

void Plane::writeSeven(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + 7, y);
	glVertex2f(x + 7, y);
	glVertex2f(x + 7, y - 12);
	glVertex2f(x + 3, y - 4);
	glVertex2f(x + 8, y - 4);
	glEnd();
	glFlush();
}

void Plane::writeEight(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + 8, y);
	glVertex2f(x, y);
	glVertex2f(x, y - 12);
	glVertex2f(x, y - 12);
	glVertex2f(x + 8, y - 12);
	glVertex2f(x + 8, y - 12);
	glVertex2f(x + 8, y);
	glVertex2f(x, y - 5);
	glVertex2f(x + 8, y - 5);
	glEnd();
	glFlush();
}

void Plane::writeNine(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + 8, y);
	glVertex2f(x, y);
	glVertex2f(x, y - 5);
	glVertex2f(x, y - 5);
	glVertex2f(x + 8, y - 5);
	glVertex2f(x + 8, y);
	glVertex2f(x + 8, y - 12);
	glVertex2f(x + 8, y - 12);
	glVertex2f(x, y - 12);
	glEnd();
	glFlush();
}

void Plane::writeX(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + 8, y - 12);
	glVertex2f(x + 8, y);
	glVertex2f(x, y - 12);
	glEnd();
	glFlush();
}

void Plane::writeComma(float x, float y) {
	glBegin(GL_LINES);
	glVertex2f(x + 4, y - 9);
	glVertex2f(x + 4, y - 11);
	glVertex2f(x + 4, y - 11);
	glVertex2f(x + 2, y - 13);
	glEnd();
	glFlush();
}