#pragma once

#include <string>
#include "Classification.h"

class Point {
private:
	float coordinateX = 0, coordinateY = 0;
	float worldX = 0, worldY = 0;
	Classification classification;
public:
	//constructors
	Point();
	Point(float&, float&, float&, float&);
	Point(int, int, int, int);
	Point(double, double, double, double);
	//copy constructor
	Point(const Point&);
	//move constructor
	Point(Point&&);
	//copy assignment
	Point operator=(const Point&);
	//move assignment
	Point operator=(Point&&);
	//getters for X and Y, respectively
	float getCoordinateX();
	float getCoordinateY();
	float getWorldX();
	float getWorldY();
	//setters for X and Y, respectively
	void setCoordX(float);
	void setCoordY(float);
	//setters for classification
	void setClassification(std::string);
	void setClassification(Classification);
	void setClassification(std::string, float*);
	//getter for classification
	Classification getClassification();
	//wrapper for setting color
	void setClassColor(float*);
	void calcWorldPosition(float, float, float, float, int);
	Point& getPoint();
	void printPt();
};