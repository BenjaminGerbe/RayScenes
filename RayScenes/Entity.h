#pragma once
#ifndef __MATH__
#define __MATH__
#include "Maths.cpp"
#endif

#ifndef __MAT__
#define __MAT__
#include "Material.h"
#endif


#include <vector>
enum Objects {
	objSphere,
	objSquare,
	objInfCylender
};

class Entity {
	protected:

	Matrix<4,4> trans;
	Matrix<4,4> transInv;
	Material mat;

	public:

	void translate(float x, float y, float z);
	void scale(float f);
	void rotateX(float deg);
	void rotateY(float deg);
	void rotateZ(float deg);

	Entity();

	Vector<4> localToGlobal(const Vector<4>& v) const;
	Vector4   localToGlobal(const Vector4& v) const;
	
	Matrix<4, 4> getTrans() {
		return trans;
	}

	Matrix<4, 4> getTransInv() {
		return transInv;
	}


	Vector<4> globalToLocal(const Vector<4>& v) const;
	Vector4   globalToLocal(const Vector4& v) const;

	Ray4 localToGlobal(const Ray4& r) const;
	Ray4 globalToLocal(const Ray4& r) const;

	virtual bool Intersect(const Ray4& ray, Vector4& impact)const;
	virtual Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;

	Material GetMat();
	void SetMat(Material mat);
};

class Camera : public Entity{
	float height;
	float width;
	float focal;
	float* color;

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

	float* getBackgroundColor() {
		return color;
	}

	void setColor(float* c) {
		color = c;
	}

	Vector4 getPoistion() {
		return localToGlobal(Vector4(0, 0, 0, 1));
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

class Light : public Entity
{
	Ray4 LightRay;

	public :
	Light(Ray4 r):LightRay(r) {

	}

	Light() {
		LightRay = Ray4();
	}

	Ray4 getRay() {
		return LightRay;
	}


};

class Scene {
	std::vector<Entity*> lstObject;
	std::vector<Light*> lstLights;

	public:

	Scene() {
		lstObject = std::vector<Entity*>();
		lstLights = std::vector<Light*>();
	};

	float* getPixelColor(Ray4 ray,Camera cam);

	void AddToScene(Entity* ent, Material mat, float x, float y, float z);

	void AddLightToScene(Light* ent);

	bool isOnShadow(Vector4 point,Light l,Entity* ent);

	Entity* getEntity(int i) {
		return lstObject[i];
	}

	~Scene() {
		lstObject.clear();
		lstLights.clear();
	}

};