#pragma once
#include "Maths.cpp"

class Camera {

	float focal;

	public :

	Camera();

	float getFocal() {
			return focal;
	}

	Ray getRay(float x,float y) const;

};