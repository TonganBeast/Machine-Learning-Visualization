#pragma once

#include "Point.h"

#include <stdlib.h>
#include <glut.h>
#include <gl/GL.h>
#include <gl/Glu.h>
#include <windows.h>
#include <vector>

class Plane {
private:
	int realWidth = 300;
	int realHeight = 300;
	int windowOffset = 50;
	float relativeWidth, relativeHeight;
	int planeX, planeY;
	int numPoints;
	int planeIndex;
	std::vector<Point> twoD_Points_v;
public:
	Plane();
	Plane(int, int, int, int, std::vector<float>, std::vector<float>, std::vector<Classification>);
	Plane(int, int, int, int, std::vector<Point>, float, float);
	//copy assignment
	Plane& operator=(const Plane&);
	//move assignment
	Plane operator=(Plane&&);
	//copy constructor
	Plane(const Plane&);
	Point getPoint(int);
	void drawPlane(int);
	void drawPoints();
	void listPoints();
	void printDimensions();
	void determineRelativeBounds(std::vector<float>, std::vector<float>);
	float getRelativeWidth();
	float getRelativeHeight();
	Plane& getPlane();
};