#pragma once
#include <string>

class Classification {
private:
	float r = 0.0, g = 0.0, b = 0.0;
	std::string classificationTitle;
public:
	Classification(std::string);
	Classification(std::string, float, float, float);
	Classification();
	Classification(const Classification&);
	Classification(Classification&&);
	Classification operator=(const Classification&);
	Classification operator=(Classification&&);

	std::string getClassificationTitle();
	//setters for this classification's color
	void setRed(float);
	void setGreen(float);
	void setBlue(float);
	void setColor(float, float, float);
	//getters for this classification's color
	float getRed();
	float getGreen();
	float getBlue();
	//getter for this classification's name
	std::string getTitle();
	
	bool equals(Classification &);

	bool sameTitle(Classification);
};