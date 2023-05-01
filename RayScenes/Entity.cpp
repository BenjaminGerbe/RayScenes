#include "Entity.h"

void Entity::translate(float x, float y, float z) {
	trans.setAt(0, 3, trans.getAt(0, 3) + x);
	trans.setAt(1, 3, trans.getAt(1, 3) + y);
	trans.setAt(2, 3, trans.getAt(2, 3) + z);

	transInv = trans.getInverse();
}


void Entity::scale(float f) {
	Matrix4x4 scale;
	scale.setAt(0, 0, f);
	scale.setAt(1, 1, f);
	scale.setAt(2, 2, f);


	trans = scale * trans;
	transInv = trans.getInverse();
}

void Entity::rotateX(float deg) {
	Matrix4x4 rot;
	rot.setAt(1, 1, std::cosf(deg));
	rot.setAt(1, 2, -std::sinf(deg));

	rot.setAt(2, 1, std::sin(deg));
	rot.setAt(2, 2, std::cos(deg));


	this->trans = rot * this->trans;

	this->transInv = trans.getInverse();
}

void Entity::rotateY(float deg) {
	Matrix4x4 rot;
	rot.setAt(0, 0, std::cosf(deg));
	rot.setAt(0, 2, std::sinf(deg));

	rot.setAt(2, 0, -std::sin(deg));
	rot.setAt(2, 2, std::cos(deg));


	this->trans = rot * this->trans;

	this->transInv = trans.getInverse();
}


void Entity::rotateZ(float deg) {
	Matrix4x4 rot;
	rot.setAt(0, 0, std::cosf(deg));
	rot.setAt(0, 1, -std::sinf(deg));

	rot.setAt(1, 0, std::sin(deg));
	rot.setAt(1, 1, std::cos(deg));


	this->trans = rot * this->trans;

	this->transInv = trans.getInverse();
}

Vector<4> Entity::localToGlobal(const Vector<4>& v) const
{
	return this->transInv * v;
}

Vector4 Entity::localToGlobal(const Vector4& v) const
{
	return this->transInv * v;
}

Ray4 Entity::localToGlobal(const Ray4& r) const
{
	Ray4 p;

	Vector4 origin = localToGlobal(p.getOrigin());
	Vector4 direction = localToGlobal(p.getDirection());
	
	p.setDirection(direction);
	p.setOrigin(origin);

	return p;
}

