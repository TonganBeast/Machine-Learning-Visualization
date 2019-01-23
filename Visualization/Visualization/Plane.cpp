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
	std::cout << "---Points in plane X" << xDim << ", X" << yDim << "---" << std::endl;
	for (int i = 0; i < numPoints; i++) {
		twoD_Points[i].setX(xPts[i], windowOffset, realWidth, planeIndex);
		twoD_Points[i].setY(yPts[i], windowOffset, realWidth, planeIndex);
		std::cout << "point " << i << ": " << xPts[i] << ", " << yPts[i] << std::endl;
	}
	this->listPoints();
}

void Plane::listPoints() {
	for (int i = 0; i < numPoints; i++) {
		std::cout << "(" << twoD_Points[i].getPoint().getWorldX() << ", " << twoD_Points[i].getPoint().getWorldY() << ")" << std::endl;
	}
}

void Plane::drawPlane(int planeIndex) {
	std::cout << "Building plane with 0, 0 at: " << (windowOffset + (planeIndex * realWidth)) << ", 100" << std::endl;
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
		twoD_Points[i].getPoint();
		twoD_Points[i].getPoint().getWorldX();
		twoD_Points[i].getPoint().getWorldY();
		std::cout << "Drawing point in its plane at " << twoD_Points[i].getPoint().getCoordinateX() << ", " << twoD_Points[i].getPoint().getCoordinateY() << std::endl;
		std::cout << "Drawing point in world window at " << twoD_Points[i].getPoint().getWorldX() << ", " << twoD_Points[i].getPoint().getWorldY() << std::endl;
		glVertex2f(twoD_Points[i].getWorldX(), twoD_Points[i].getWorldY());
	}
	glEnd();
	glFlush();
}

Point Plane::getPoint(int index) {
	return this->twoD_Points[index];
}