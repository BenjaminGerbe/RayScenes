#pragma once

class Color {

public:

	 float r;
	 float g;
	 float b;

	 

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

};

class Material {
	Color ambiante;
	Color diffuse;
	Color speculaire;
	float shininess;

public:
	Material(Color a, Color d, Color s,float si) :ambiante(a), diffuse(d), speculaire(s),shininess(si) {};

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

};