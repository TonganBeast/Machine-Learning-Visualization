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
	Point *twoD_Points;
	
	std::vector<Point> twoD_Points_v;
public:
	Plane();
	Plane(int, int, int, int, float[], float[]);
	Plane(int, int, int, int, std::vector<float>, std::vector<float>, std::vector<Classification>);
	Point getPoint(int);
	void drawPlane(int);
	void drawPoints(std::string*);
	void drawPoints();
	void listPoints();
	void printDimensions();
	void determineRelativeBounds(std::vector<float>, std::vector<float>);
	float getRelativeWidth();
	float getRelativeHeight();
};