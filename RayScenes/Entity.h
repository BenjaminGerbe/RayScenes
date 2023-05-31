#pragma once
#ifndef __MATH__
#define __MATH__
#include "Maths.cpp"
#endif


#include <vector>
enum Objects {
	objSphere,
	objSquare,
	objInfCylender
};

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

	virtual bool Intersect(const Ray4& ray, Vector4& impact)const;
	virtual Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;
		
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


	bool  Intersect(const Ray4& ray, Vector4& impact) const;
	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;
};

class Square : public Entity {
public :
	Square();

	bool Intersect(const Ray4& ray, Vector4& impact) const;
	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;
};

class Sphere : public Entity {
	public:
	Sphere();

	bool Intersect(const Ray4& ray, Vector4& impact) const;

	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;

};

class InfCylender : public Entity {
	public:
	InfCylender();

	bool Intersect(const Ray4& ray, Vector4& impact) const;

	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;

};

class Light {

};

class Scene {
	std::vector<Entity*> lstObject;
	std::vector<Light*> lstLights;

	public:

	Scene() {
		lstObject = std::vector<Entity*>();
		lstLights = std::vector<Light*>();
	};

	float* getPixelColor(Ray4 ray);

	void AddToScene(Entity* ent, float x, float y, float z);

	~Scene() {
		lstObject.clear();
		lstLights.clear();
	}

};