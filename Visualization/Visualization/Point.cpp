#include "Point.h"
#include <iostream>
#include <string>

//default constructor
Point::Point() {}

//float-argument constructor
//assign passed values to coordinateX and coordinateY respectively
Point::Point(float &px, float &py, float &wx, float &wy) {
	coordinateX = px;
	coordinateY = py;
	worldX = wx;
	worldY = wy;
}

//integer-argument constructor
//casts integer arguments as floats and assigns them to coordinateX
//and coordinateY respectively
Point::Point(int x, int y) {
	coordinateX = (float)x;
	coordinateY = (float)y;
}

//double-argument constructor
//casts double arguments as floats and assigns them to coordinateX
//and coordinateY respectively
Point::Point(double x, double y) {
	coordinateX = float(x);
	coordinateY = float(y);
}

//copy constructor
//creates new Point object with the same values as another Point that
//gets passed as an argument
Point::Point(const Point& other) {
	coordinateX = other.coordinateX;
	coordinateY = other.coordinateY;
	worldX = other.worldX;
	worldY = other.worldY;
}

//move constructor
//creates new Point object by taking the values from the Point that
//gets passed as an argument
Point::Point(Point&& other) {
	coordinateX = other.coordinateX;
	coordinateY = other.coordinateY;
	worldX = other.worldX;
	worldY = other.worldY;
	other.coordinateX = 0;
	other.coordinateY = 0;
	other.worldX = 0;
	other.worldY = 0;
}

//copy assignment
//creates and returns a new Point object with the same values as
//another Point that gets passed as an argument
Point Point::operator=(const Point& other) {
	return Point(other.coordinateX, other.coordinateY);
}

//move assignment
//creates and returns a new Point object by taking values from the
//Point that gets passed as an argument.
Point Point::operator=(Point&& other) {
	float x = other.coordinateX;
	float y = other.coordinateY;
	float wx = other.worldX;
	float wy = other.worldY;
	other.coordinateX = 0;
	other.coordinateY = 0;
	other.worldX = 0;
	other.worldY = 0;
	return Point(x, y);
}

float Point::getCoordinateX() {
	return this->coordinateX;
}

float Point::getCoordinateY() {
	return this->coordinateY;
}

float Point::getWorldX() {
	return this->worldX;
}

float Point::getWorldY() {
	return this->worldY;
}

void Point::setX(float newX, float initialOffset, float offset, int planeIndex) {
	coordinateX = newX;
	worldX = coordinateX + initialOffset + (offset * planeIndex);
	std::cout << "Point's X value set to " << coordinateX << std::endl;
}

void Point::setY(float newY, float initialOffset, float offset, int planeIndex) {
	coordinateY = newY;
	worldY = newY + initialOffset;
	std::cout << "Point's Y value set to " << coordinateY << std::endl;
}

Point& Point::getPoint() {
	return *this;
}

void Point::printPt() {
	std::cout << "(" << this->worldX << ", " << this->worldY << ")" << std::endl;
}