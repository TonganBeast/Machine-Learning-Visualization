#include "Point.h"

//default constructor
//set both x and y values to 0
Point::Point() {
	coordinateX = 0;
	coordinateY = 0;
}

//float-argument constructor
//assign passed values to coordinateX and coordinateY respectively
Point::Point(float x, float y) {
	coordinateX = x;
	coordinateY = y;
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
}

//move constructor
//creates new Point object by taking the values from the Point that
//gets passed as an argument
Point::Point(Point&& other) {
	coordinateX = other.coordinateX;
	coordinateY = other.coordinateY;
	other.coordinateX = 0;
	other.coordinateY = 0;
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
	other.coordinateX = 0;
	other.coordinateY = 0;
	return Point(x, y);
}