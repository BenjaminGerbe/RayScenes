#ifndef __CAMERA__
#define __CAMERA__

#ifndef __MATH__
#define __MATH__
#include "Maths.cpp"
#endif


class Camera {

	float focal;

public:

	Camera();

	float getFocal() {
		return focal;
	}

	Ray getRay(float x, float y) const;

};

#endif