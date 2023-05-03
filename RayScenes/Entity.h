#pragma once
#ifndef __MATH__
#define __MATH__
#include "Maths.cpp"
#endif

class Entity {

	float x;
	float y;
	float z;

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

class Camera : public Entity{
	float height;
	float width;
	float focal;

public:

	Camera(float W, float H, int F) : width(W), height(H), focal(F) {};


	float getFocal() {
		return focal;
	}

	float getHeight() {
		return height;
	};

	float getWidth() {
		return width;
	}

	Ray4 getRay(float x, float y)const;

};


class Plan : public Entity{

	public:
	Plan();

	bool Intersect(const Ray4& ray, Vector4& impact) const;

};

class Square : public Entity {
public :
	Square();

	bool Intersect(const Ray4& ray, Vector4& impact) const;

};

class Sphere : public Entity {
	public:
	Sphere();

	bool Intersect(const Ray4& ray, Vector4& impact) const;

	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;

};