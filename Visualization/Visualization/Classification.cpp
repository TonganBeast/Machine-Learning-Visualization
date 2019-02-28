#include "Classification.h"

//constructor takes in string argument and sets title to that given
//string. leaves color at default values.
Classification::Classification(std::string title) {
	classificationTitle = title;
}

//constructor takes in string argument and float array argument. sets
//title to given string and this classification's color to the given values
Classification::Classification(std::string title, float red, float green, float blue) {
	classificationTitle = title;
	r = red;
	g = green;
	b = blue;
}

//default constructor
Classification::Classification() {}

std::string Classification::getClassificationTitle() {
	return classificationTitle;
}

Classification::Classification(const Classification& other) {
	r = other.r;
	g = other.g;
	b = other.b;
	classificationTitle = other.classificationTitle;
}

Classification::Classification(Classification&& other) {
	r = other.r;
	g = other.g;
	b = other.b;
	classificationTitle = other.classificationTitle;
	other.r = 0;
	other.g = 0;
	other.b = 0;
	other.classificationTitle.clear();
}

Classification& Classification::operator=(const Classification& other) {
	classificationTitle = other.classificationTitle;
	r = other.r;
	g = other.g;
	b = other.b;
	return *this;
}

Classification& Classification::operator=(Classification&& other) {
	float clr[3] = { other.r, other.g, other.b };
	r = clr[0];
	g = clr[1];
	b = clr[2];
	std::string ttl = other.classificationTitle;
	other.r = 0;
	other.g = 0;
	other.b = 0;
	other.classificationTitle.clear();
	return *this;
}

void Classification::setColor(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

float Classification::getRed() {
	return r;
}

float Classification::getGreen() {
	return g;
}

float Classification::getBlue() {
	return b;
}

bool Classification::sameTitle(Classification c) {
	if (!c.getClassificationTitle().compare(this->classificationTitle))
		return true;
	else
		return false;
}

std::string Classification::getTitle() {
	return this->classificationTitle;
}

Classification& Classification::getClassification() {
	return *this;
}