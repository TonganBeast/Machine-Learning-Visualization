#pragma once
#include "Point.h"
#include "glut.h"
#include <gl/GL.h>
#include <gl/Glu.h>
#include <windows.h>

class Plane {
private:
	int realWidth = 300;
	int realHeight = 300;
	float relativeWidth, relativeHeight;
	int planeX, planeY;
public:
	Plane(float, float, float*, float*, int, int, int);
};