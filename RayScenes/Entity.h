#pragma once
#include "Maths.cpp"
class Entity {

	Matrix<4,4> trans;
	Matrix<4,4> transInv;

	public:
	void translate(float x, float y, float z);
	void scale(float f);
	void rotateX(float deg);
	void rotateY(float deg);
	void rotateZ(float deg);

	Vector<4> localToGlobal(const Vector<4>& v) const;
	Vector4   localToGlobal(const Vector4& v) const;


	Vector<4> globalToLocal(const Vector<4>& v) const;
	Vector4   globalToLocal(const Vector4& v) const;

	Ray4 localToGlobal(const Ray4& r) const;
	Ray4 globalToLocal(const Ray4& r) const;
		
};

class Plan : Entity{

	public:
	Plan();

	bool Intersect(const Ray4& ray, Vector4& impact) const;

};