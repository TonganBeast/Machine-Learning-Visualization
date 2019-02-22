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
Point::Point(int px, int py, int wx, int wy) {
	coordinateX = (float)px;
	coordinateY = (float)py;
	worldX = (float)wx;
	worldY = (float)wy;
}

//double-argument constructor
//casts double arguments as floats and assigns them to coordinateX
//and coordinateY respectively
Point::Point(double px, double py, double wx, double wy) {
	coordinateX = (float)px;
	coordinateY = (float)py;
	worldX = (float)wx;
	worldY = (float)wy;
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
	return Point(other.coordinateX, other.coordinateY, other.worldX, other.worldY);
}

//move assignment
//creates and returns a new Point object by taking values from the
//Point that gets passed as an argument.
Point Point::operator=(Point&& other) {
	float px = other.coordinateX;
	float py = other.coordinateY;
	float wx = other.worldX;
	float wy = other.worldY;
	other.coordinateX = 0;
	other.coordinateY = 0;
	other.worldX = 0;
	other.worldY = 0;
	return Point(px, py, wx, wy);
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

void Point::setCoordX(float newX) {
	coordinateX = newX;
	//worldX = coordinateX + initialOffset + (offset * planeIndex);
	//std::cout << "Point's X value set to " << coordinateX << std::endl;
}

void Point::setCoordY(float newY) {
	coordinateY = newY;
	//worldY = newY + initialOffset;
	//std::cout << "Point's Y value set to " << coordinateY << std::endl;
}

Point& Point::getPoint() {
	return *this;
}

void Point::printPt() {
	std::cout << "(" << this->worldX << ", " << this->worldY << ")" << std::endl;
}

//setter for classification instantiates classification from string
void Point::setClassification(std::string title) {
	classification = Classification(title);
}

//setter for classification instantiates classification from string and 3-float array to set color
void Point::setClassification(std::string title, float* color) {
	classification = Classification(title, color[0], color[1], color[2]);
}

//setter for classification copies given classification to this
void Point::setClassification(Classification givenClassification) {
	classification = givenClassification;
}

//getter for classification
Classification Point::getClassification() {
	return classification;
}

//wrapper method for setting color which is tied to classification
void Point::setClassColor(float* rgb) {
	this->classification.setColor(rgb[0], rgb[1], rgb[2]);
}

//method calculates this point world position
void Point::calcWorldPosition(float relativeMaxWidth, float relativeMaxHeight, float planeSize, float planeOffset, int planePosition) {
	worldX = ((coordinateX * planeSize) / relativeMaxWidth) + (planePosition * planeSize) + planeOffset;
	worldY = ((coordinateY * planeSize) / relativeMaxHeight) + planeOffset;
}
