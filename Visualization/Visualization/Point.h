#pragma once

class Point {
private:
	float coordinateX = 0, coordinateY = 0;
	float worldX = 0, worldY = 0;
public:
	//constructors
	Point();
	Point(float&, float&, float&, float&);
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
	//getters for X and Y, respectively
	float getCoordinateX();
	float getCoordinateY();
	float getWorldX();
	float getWorldY();
	//setters for X and Y, respectively
	void setX(float, float, float, int);
	void setY(float, float, float, int);
	Point& getPoint();
	void printPt();
};