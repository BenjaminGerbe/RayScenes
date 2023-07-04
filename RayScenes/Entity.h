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
	Material* mat;

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

	virtual Vector4 getTextureCoordinates(const Vector4& p)const;

	Material* GetMat();

	Material* GetMat() const;


	void SetMat(Material* mat);
};

class Camera : public Entity{
	float height;
	float width;
	float focal;
	Color color;

public:

	Camera(float W, float H, float F, float fov, float near, float far) : width(W), height(H),focal(F) {
		
		/*
		fov = (fov * M_PI) / 180.0f;

		trans = Matrix4x4();
		this->trans.setAt(0, 0, 1.0f / ((1.0) * std::tan(fov / 2.0f)));
		this->trans.setAt(1, 1, 1.0f /(std::tan(fov/2.0f)));
		this->trans.setAt(2, 2, -(far+near)/(far-near));
		this->trans.setAt(3, 2, -(2*far*near)/(far-near));

		transInv = trans.getInverse();
	*/
	};


	float getFocal() {
		return focal;
	}

	float getHeight() {
		return height;
	};

	float getWidth() {
		return width;
	}

	Color getBackgroundColor() {
		return color;
	}

	void setColor(Color c) {
		color = c;
	}

	Vector4 getPoistion() {
		return localToGlobal(Vector4(0, 0, 0, 1));
	}

	Ray4 getRay(float x, float y)const;

};





class Cube : public Entity {
public:
	Cube();

	bool  Intersect(const Ray4& ray, Vector4& impact) const;
	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;
	Vector4 getTextureCoordinates(const Vector4& p)const;
};

class Plan : public Entity{

	public:
	Plan();
	
	

	Vector4 getTextureCoordinates(const Vector4& p)const;
	bool  Intersect(const Ray4& ray, Vector4& impact) const;
	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;
};

class Square : public Entity {
public :
	Square();


	Vector4 getTextureCoordinates(const Vector4& p)const;
	bool Intersect(const Ray4& ray, Vector4& impact) const;
	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;
};

class Triangle : public Entity {
protected:
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	Vector4 normal;

public : 
	Triangle(Vector3 a, Vector3 b, Vector3 c) :p0(a), p1(b), p2(c) { normal = Vector4(); };

	bool Intersect(const Ray4& ray, Vector4& impact) const ;
	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;

	Vector4 getNormalTmp() {
		return normal;
	}

};


class Mesh : public Entity {
	
	std::vector<Triangle> Vertex;
	std::vector<Vector3> Face;
	Vector4 normal;

	public :
	Mesh(std::vector<Vector3> v);

	bool Intersect(const Ray4& ray, Vector4& impact) const;
	Ray4 getNormal(const Vector4& impact, const Vector4& observator)const;
	
	

};

class Sphere : public Entity {
	public:
	Sphere();


	Vector4 getTextureCoordinates(const Vector4& p)const;
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
	Color DiffuseColor;
	Color SpecularColor;

	public :

	Light(Ray4 r, Color c, Color s) : DiffuseColor(c), SpecularColor(s){
		LightRay = r;
	}

	Light() {
		

		DiffuseColor = Color(255, 255, 255);
		SpecularColor = Color(255, 255, 255);
	}

	Ray4 getRay() {
		return LightRay;
	}

	Color getDiffuseColor() {
		return DiffuseColor;
	}


	Color getSpecularColor() {
		return SpecularColor;
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

	Color getPixelColorLambert(Ray4 ray,Camera cam);
	Color getPixelColorPhong(Ray4 ray,Camera cam);

	void AddToScene(Entity* ent, Material* mat, float x, float y, float z);

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