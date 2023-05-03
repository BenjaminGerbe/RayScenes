#include "Camera.h"



Ray Camera::getRay(float x, float y) const
{

	float ratio = width / height;

	Vector3  origin( ((x * 2) - 1) * ratio, (y * 2) - 1, 0);
	Vector3 v(origin.x, origin.y, -focal);
	v = v.normalized();

	Ray r(origin, v);

	return r;
}
