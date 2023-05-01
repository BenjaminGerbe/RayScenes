#include "Camera.h"

Ray Camera::getRay(float x, float y) const
{
	Vector3  origin((x * 2) - 1, (y * 2) - 1, 0);
	Vector3 v(origin.x, origin.y, -this->focal);
	v = v.normalized();
	Ray r(origin, v);

	return r;
}
