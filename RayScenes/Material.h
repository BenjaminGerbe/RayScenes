#pragma once
#include <iostream>
#include <algorithm>

#ifndef __IMAGE__
#define __IMAGE__
#include "Image.h"
#endif

 
class Color {

public:

	 float r;
	 float g;
	 float b;

	 


	 void setR(float _r) {
		 r = _r;
	 };

	 void setG(float _g) {
		 g = _g;
	 };

	 void setB(float _b) {
		 b = _b;
	 };


	float getR() {
		return r;
	};

	float getG() {
		return g;
	};

	float getB() {
		return b;
	};

	Color(float x, float y, float z) :r(x), g(y), b(z) {};
	Color() {
		r = 0;
		g = 0;
		b = 0;
	};

	Color operator*(const Color c)  {
		r = r / 255.0f;
		g = g / 255.0f;
		b = b / 255.0f;


		Color temp = c;
		temp.r = c.r / 255.0f;
		temp.g = c.g / 255.0f;
		temp.b = c.b / 255.0f;

		r = std::clamp((r * temp.r), 0.0f, 1.0f);
		g = std::clamp((g * temp.g), 0.0f, 1.0f);
		b = std::clamp((b * temp.b), 0.0f, 1.0f);

		return (*this)*255.0;
	}

	Color operator+(const Color c) {
		r = std::clamp((r + c.r), 0.0f, 255.0f);
		g = std::clamp((g + c.g), 0.0f, 255.0f);
		b = std::clamp((b + c.b), 0.0f, 255.0f);

		return (*this);
	}

	Color operator*(float s) {
		r = std::clamp((r * s),0.0f,255.0f);
		g = std::clamp( (g * s),0.0f,255.0f);
		b = std::clamp( (b * s),0.0f,255.0f);


		return (*this);
	}

	friend std::ostream& operator<<(std::ostream& os, const Color& c) {
		os << "[ ";
		os << c.r << "; ";
		os << c.g << "; ";
		os << c.b;
		os << " ]";
		os << std::endl;

		return os;
	};

};

class Material {
	Color ambiante;
	Color diffuse;
	Color speculaire;
	float shininess;

	Image* colorMap;
	Image* normalMap;

	bool hasNormalMap = false;



public:
	Material(Color a, Color d, Color s,float si) :ambiante(a), diffuse(d), speculaire(s),shininess(si),colorMap(new Image(1, 1, 3, new unsigned char[3] {static_cast<unsigned char>(255), static_cast<unsigned char>(255), static_cast<unsigned char>(255)})),normalMap(new Image) {};

	Material();

	Color getAmbiante() {
		return ambiante;
	}

	Color getSpeculaire() {
		return speculaire;
	}

	Color getDiffuse() {
		return diffuse;
	}

	float getShininess() {
		return shininess;
	}

	void setColorMap(Image* image) {
		this->colorMap = image;
	}

	void setNormalMap(Image* image) {
		hasNormalMap = true;
		this->normalMap = image;
	}

	Image* getColorMap() {
		return this->colorMap;
	}

	Image* getNormalMap() {
		return this->normalMap;
	}


	bool getHasNormalMap() {
		return hasNormalMap;
	}

};