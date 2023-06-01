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

public:
	Material(Color a, Color d, Color s) :ambiante(a), diffuse(d), speculaire(s) {};

	Material();

	Color getAmbiante() {
		return ambiante;
	}

	Color getDiffuse() {
		return diffuse;
	}

};