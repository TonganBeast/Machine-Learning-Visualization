#pragma once

class Point {
private:
	float coordinateX, coordinateY;
public:
	//constructors
	Point();
	Point(float, float);
	Point(int, int);
	Point(double, double);
	//copy constructor
	Point(const Point&);
	//move constructor
	Point(Point&&);
	//copy assignment
	Point operator=(const Point&);
	//move assignment
	Point operator=(Point&&);
};