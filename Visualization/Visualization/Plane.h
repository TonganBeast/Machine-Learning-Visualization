#pragma once
#include "Point.h"
#include <stdlib.h>
#include "glut.h"
#include <gl/GL.h>
#include <gl/Glu.h>
#include <windows.h>

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
public:
	Plane();
	Plane(int, int, int, int, float[], float[]);
	Point getPoint(int);
	void drawPlane(int);
	void drawPoints();
	void listPoints();
};