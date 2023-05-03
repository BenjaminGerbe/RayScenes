#ifndef __CAMERA__
#define __CAMERA__

#ifndef __MATH__
#define __MATH__
#include "Maths.cpp"
#endif


class Camera {
	float height;
	float width;
	float focal;

public:

	Camera(float W, float H,int F) : width(W), height(H),focal(F) {};


	float getFocal() {
		return focal;
	}

	float getHeight() {
		return height;
	};

	float getWidth() {
		return width;
	}

	Ray getRay(float x, float y)const ;
	
};

#endif