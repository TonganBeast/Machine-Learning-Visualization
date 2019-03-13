#pragma once

#include "Point.h"
#include <windows.h>
#include <stdlib.h>
#include <glut.h>
#include <gl/GL.h>
#include <gl/Glu.h>
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
	float planeMidpoint();
	void determineRelativeBounds(std::vector<float>, std::vector<float>);
	float getRelativeWidth();
	float getRelativeHeight();
	Plane& getPlane();
	int getXindex();
	int getYindex();
	void writeIndeces();
	void writeZero(float, float);
	void writeOne(float, float);
	void writeTwo(float, float);
	void writeThree(float, float);
	void writeFour(float, float);
	void writeFive(float, float);
	void writeSix(float, float);
	void writeSeven(float, float);
	void writeEight(float, float);
	void writeNine(float, float);
	void writeX(float, float);
	void writeComma(float, float);
};
