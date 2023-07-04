#include "Entity.h"
#include <limits>
#include <algorithm>
#include <omp.h>
#include <utility>


Ray4 Camera::getRay(float x, float y) const
{

	float ratio = width / height;

	Vector3  origin(((x * 2) - 1)*ratio , (y * 2) - 1, 0);
	Vector3 v(origin.x, origin.y, -focal);
	v = v.normalized();

	Ray r(origin, v);

	Ray4 ray(r);


	ray = localToGlobal(Ray4(r));
	return ray;
}


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


	trans = rot * trans;
	transInv = trans.getInverse();
}

void Entity::rotateY(float deg) {
	Matrix4x4 rot;
	rot.setAt(0, 0, std::cosf(deg));
	rot.setAt(0, 2, std::sinf(deg));

	rot.setAt(2, 0, -std::sin(deg));
	rot.setAt(2, 2, std::cos(deg));


	trans = rot * trans;

	transInv = trans.getInverse();
}


void Entity::rotateZ(float deg) {
	Matrix4x4 rot;
	rot.setAt(0, 0, std::cosf(deg));
	rot.setAt(0, 1, -std::sinf(deg));

	rot.setAt(1, 0, std::sin(deg));
	rot.setAt(1, 1, std::cos(deg));


	trans = rot * trans;

	transInv = trans.getInverse();
}

Entity::Entity()
{
}

Vector<4> Entity::localToGlobal(const Vector<4>& v) const
{

	return this->transInv * v;
}

Vector4 Entity::localToGlobal(const Vector4& v) const
{
	
	return this->transInv * v;
}
Vector<4> Entity::globalToLocal(const Vector<4>& v) const
{
	
	return this->trans * v;
}


Vector4 Entity::globalToLocal(const Vector4& v) const
{
	Vector4 vec = this->trans * v;
	return vec;
}



Ray4 Entity::localToGlobal(const Ray4& r) const
{
	Ray4 p;

	Vector4 origin = localToGlobal(r.getOrigin());
	Vector4 direction = localToGlobal(r.getDirection());


	p.setDirection(direction);
	p.setOrigin(origin);

	return p;
}




Ray4 Entity::globalToLocal(const Ray4& r) const
{

	Vector4 origin( globalToLocal(r.getOrigin()));
	Vector4 direction(globalToLocal(r.getDirection()));
	
	return Ray4(origin,direction);
}

bool Entity::Intersect(const Ray4& ray, Vector4& impact) const
{
	return true;
}

Ray4 Entity::getNormal(const Vector4& impact, const Vector4& observator) const
{
	return Ray4();
}

Vector4 Entity::getTextureCoordinates(const Vector4& p) const
{
	return Vector4();
}

Material* Entity::GetMat()
{
	return mat;
}

Material* Entity::GetMat() const
{
	return this->mat;
}




void Entity::SetMat(Material* mat)
{
	this->mat = mat;
}

Plan::Plan()
{

}

Vector4 Square::getTextureCoordinates(const Vector4& p) const
{
	Vector4 point = globalToLocal(p);
	Vector4 uv((point.x + 1.f) / 2.f, (point.y + 1.f) / 2.f, 0,1.0);
	return uv;
}

Vector4 Plan::getTextureCoordinates(const Vector4& p) const
{
	Vector4 lp = globalToLocal(p);
	float x = lp[0] - (int)lp[0];
	float y = lp[1] - (int)lp[1];
	if (x < 0)x += 1;
	if (y < 0)y += 1;

	return Vector4(x, y, 0,1.0);
}

bool Plan::Intersect(const Ray4& ray, Vector4& impact) const
{

	Ray4 r = globalToLocal(ray);
	float t = -r.getOrigin().z / r.getDirection().z;
	impact = localToGlobal(r.getOrigin() + ( r.getDirection()*t));

	return t > 0;
}

Ray4 Plan::getNormal(const Vector4& impact, const Vector4& observator) const
{
	Vector4 p = globalToLocal(impact);
	Vector4 obs = globalToLocal(observator);

	Vector4 vec(0, 0, -1,0);

	if (p[2] < obs[2]) {
		vec = -vec;
	}

	Ray4 r(impact, localToGlobal(vec));

	return r;
}

Square::Square()
{
	
}

bool Square::Intersect(const Ray4& ray, Vector4& impact) const
{
	Ray4 r = globalToLocal(ray);
	float t = -r.getOrigin().z / r.getDirection().z;
	impact = localToGlobal(r.getOrigin() +( r.getDirection()*t));
	Vector4 localimpact = r.getOrigin() + ( r.getDirection()*t);

	if (t > 0) {

		return (localimpact[0] >= -1 && localimpact[0] <= 1 && localimpact[1] >= -1 && localimpact[1] <= 1);
	}



	return false;
}

Ray4 Square::getNormal(const Vector4& impact, const Vector4& observator) const
{
	Vector4 p = globalToLocal(impact);
	Vector4 obs = globalToLocal(observator);

	Vector4 vec(0, 0, -1, 0);

	if (p[2] < obs[2]) {
		vec = -vec;
	}

	Ray4 r(impact, localToGlobal(vec));

	return r;
}

Sphere::Sphere()
{

}

Vector4 Sphere::getTextureCoordinates(const Vector4& p) const
{
	Vector4 posLocal = globalToLocal(p);
	float u = 0.5 + atan2(posLocal.z, posLocal.x) / (2 * 3.1415926);
	float v = 0.5 - asin(posLocal.y) / 3.1415926;

	return Vector4(u, v,0.0,1.0);
}

bool Sphere::Intersect(const Ray4& ray, Vector4& impact) const
{
	Ray4 r = globalToLocal(ray);

	Vector4 vector = r.getDirection();
	Vector4 origin = r.getOrigin();
	
	float a = pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2);
	float b = 2 * (origin.x * vector.x + origin.y * vector.y +origin.z * vector.z);
	float c = (pow(origin.x, 2) + pow(origin.y, 2) + pow(origin.z, 2)) - 1;

	float alpha = pow(b, 2) - 4.0 * a * c;


	float x1 = (-b + sqrt(alpha)) / (2 * a);
	float x2 = (-b - sqrt(alpha)) / (2 * a);

	if (x1 < 0 && x2 < 0) {
		return false;
	}

	float t = x2;

	if (x1 >= 0 && x2 < 0) {
		t = x1;
	}

	if (t >= 0) {
		impact = localToGlobal(origin +  t*vector);

		return true;
	}

	return false;
}

Ray4 Sphere::getNormal(const Vector4& impact, const Vector4& observator) const
{
	Material* mat = GetMat();
	Vector4 vec;
	Vector4 p = globalToLocal(impact);
	Vector4 obs = globalToLocal(observator);

	vec = p;



		Vector4 coord =getTextureCoordinates(impact);
		Image* texture = mat->getNormalMap();
		unsigned char* us = (*texture).getColor(coord.x * texture->getWidth(), coord.y * texture->getWidth());
		Vector4 v(us[0]/255.0f, us[1] / 255.0f, us[2] / 255.0f, 0.0);

		vec.setAt(0, vec.x * v.x);
		vec.setAt(1, vec.y * v.y);
		vec.setAt(2, vec.z * v.z);

		vec = vec.normalized();



	if (obs[0] * obs[0] + obs[1] * obs[1] + obs[2] * obs[2] <= 1) {
		vec = -vec;
	}

	vec.setAt(3, 0);
	vec = localToGlobal(vec);
	vec = vec.normalized();



	Ray4 r(impact, vec);
	return r;
}

InfCylender::InfCylender()
{
}

bool InfCylender::Intersect(const Ray4& ray, Vector4& impact) const
{
	Ray4 r = globalToLocal(ray);

	Vector4 vector = r.getDirection();
	Vector4 origin = r.getOrigin();
	float a = pow(vector.x, 2) + pow(vector.z, 2);
	float b = 2 * (origin.x * vector.x + origin.z * vector.z);
	float c = (pow(origin.x, 2) + pow(origin.z, 2)) - 1;

	float alpha = pow(b, 2) - 4.0 * a * c;


	float x1 = (-b + sqrt(alpha)) / (2 * a);
	float x2 = (-b - sqrt(alpha)) / (2 * a);

	if (x1 < 0 && x2 < 0) {
		return false;
	}

	float t = x2;

	if (x1 >= 0 && x2 < 0) {
		t = x1;
	}

	if (t >= 0) {
		impact = localToGlobal(origin + t * vector);
		return true;
	}


	return false;
}

Ray4 InfCylender::getNormal(const Vector4& impact, const Vector4& observator) const
{
	Vector4 lp = globalToLocal(impact);
	Vector4 lo = globalToLocal(observator);
	if ((lo - Vector4(0, lo[1], 0,0)).getNorme() > 1)
		return localToGlobal(Ray4(lp, Vector4(lp[0], 0, lp[2],0))).normalized();
	return localToGlobal(Ray4(lp, Vector4(-lp[0], 0, -lp[2],0))).normalized();
}


Color Scene:: getPixelColorLambert(Ray4 ray,Camera cam) {
	Color src(1,1,1);
	Vector4 impact;
	float depth = std::numeric_limits<float>::max();
	bool Itr = false;

	for (int i = 0; i < lstObject.size(); i++)
	{
		
		if (lstObject[i]->Intersect(ray, impact) ) {

			Itr = true;
			float tmp = cam.globalToLocal(impact).getNorme();
			
			if (tmp > depth) {
				continue;
			}
			
			
			float NL = 0;
			Vector4 N = lstObject[i]->globalToLocal(lstObject[i]->getNormal(impact, cam.getPoistion()).getDirection()).normalized();
			std::cout << N << std::endl;
			Material* mat = lstObject[i]->GetMat();
			depth = tmp;
			src = mat->getAmbiante();
			for (int j = 0; j < lstLights.size(); j++)
			{
				Light light = *lstLights[j];
				Vector4 L = -lstObject[i]->globalToLocal(light.getRay().getDirection()).normalized();
				NL += N.dot(L);
				NL = std::clamp(NL, 0.0f, 1.0f);

				if (isOnShadow(impact, *lstLights[j], lstObject[i])) NL = 0;

				Color cNL(NL, NL, NL);
				cNL = cNL * 255.0f;
		
				Color nC(N.x, N.y, N.z);
				nC = nC * 255.0f;

				
				src = src + ( mat->getDiffuse() * cNL * light.getDiffuseColor()) ;
			}
		}
		

		
	}

	if (!Itr) {
		return cam.getBackgroundColor();
	}

	return src;
}



Color Scene::getPixelColorPhong(Ray4 ray, Camera cam) {
	
	Color src(1, 1, 1);


	Vector4 impact;
	float depth = std::numeric_limits<float>::max();
	bool Itr = false;

	float r = 1;
	float g = 1;
	float b = 1;

	float nMax = 0;
	std::vector<Entity*> lstObject;

	lstObject = std::vector<Entity*>(this->lstObject);

	for (int i = 0; i < lstObject.size(); i++)
	{
		if (lstObject[i]->Intersect(ray, impact)) {

			Itr = true;
			float tmp = cam.globalToLocal(impact).getNorme();

			if (tmp > depth) {
				continue;
			}


			float NL = 0;
			Vector4 N = lstObject[i]->globalToLocal(lstObject[i]->getNormal(impact, cam.getPoistion()).getDirection()).normalized();
			Material* mat = lstObject[i]->GetMat();
			depth = tmp;


			Vector4 coord = lstObject[i]->getTextureCoordinates(impact);
			Image* texture = mat->getColorMap();
			unsigned char* us = (*texture).getColor(coord.x * texture->getWidth(), coord.y * texture->getWidth());
			Color cTexture(us[0], us[1], us[2]);


			Image* rougNessTexture = mat->getRoughnessMap();
			unsigned char* rg = (*rougNessTexture).getColor(coord.x *(rougNessTexture->getWidth()-1), coord.y *( rougNessTexture->getHeight()-1));
			Color roughTexture(rg[0], rg[1], rg[2]);

			src = mat->getAmbiante() * cTexture;
		

			for (int j = 0; j < lstLights.size(); j++)
			{
				Light light = *lstLights[j];
				Vector4 L = -lstObject[i]->globalToLocal(light.getRay().getDirection()).normalized();
				NL = N.dot(L);
				NL = std::clamp(NL, 0.0f, 1.0f);


				if (isOnShadow(impact, *lstLights[j], lstObject[i])) NL = 0;

				Vector4 R = (N * 2.0f * NL) - L;
				Vector4 V = lstObject[i]->globalToLocal(cam.getPoistion() - impact).normalized();
				float specular = pow(R.dot(V), mat->getShininess());

				Color specColor = mat->getSpeculaire() * specular * light.getSpecularColor() * roughTexture;


				Color temp = (cTexture * mat->getDiffuse() * NL * light.getDiffuseColor()) + specColor;
				src = src + temp;

			}
		}
	}
	


	if (!Itr) {
		return cam.getBackgroundColor();
	}

	return src;
}



bool Scene::isOnShadow(Vector4 point,Light l, Entity* ent) {
	bool result = false;
	
	for (int i = 0; i < lstObject.size(); i++)
	{
		if (ent == lstObject[i]) continue;
		Ray4 r(point,-l.getRay().getDirection());
		Vector4 vec;
		if (lstObject[i]->Intersect(r, vec)) {
			result = true;
		}
		
	}

	return result;
}

void Scene::AddToScene(Entity* ent, Material* mat, float x, float y, float z) {
	ent->translate(x, y, z);
	ent->SetMat(mat);
	lstObject.push_back(ent);
}

void Scene::AddLightToScene(Light* l) {
	lstLights.push_back(l);
}

Cube::Cube()
{
}

bool Cube::Intersect(const Ray4& ray, Vector4& impact) const
{
	Ray4 r = globalToLocal(ray);


	bool result = false;
	float localF = std::numeric_limits<float>::max();
	float b = 1.f;

	for (int i = 0; i < 6; i++) {
		if (i == 3)  b = -b;
		int j = i % 3;
		float t = (b - r.getOrigin()[j]) / r.getDirection()[j];
		Vector4 localimpact = r.getOrigin() + (r.getDirection() * t );
		impact = localToGlobal(r.getOrigin() +  (r.getDirection()*t));
		if (t > 0) {
			if (localimpact[(j - 1) < 0 ? 2 : (j - 1)] >= -1 && localimpact[(j - 1) < 0 ? 2 : (j - 1)] <= 1 && localimpact[(j + 1) % 3] >= -1 && localimpact[(j + 1) % 3] <= 1) {
				result = true;
				localF = std::min(localF, t);
			}
		}
	}

	if (!result) return false;
	impact = localToGlobal(r.getOrigin() + ( r.getDirection()*localF));

	return true;
}

Ray4 Cube::getNormal(const Vector4& impact, const Vector4& observator) const
{
	Vector4 lp = globalToLocal(impact);
	Vector4 lo = globalToLocal(observator);
	Vector4 v(0, 0, 0,0);
	if (lp.x > 0.999) v.setAt(0, 1.0);
	else if (lp[0] < -0.999) v.setAt(0, -1.0);
	else if (lp[1] > 0.999) v.setAt(1, 1.0);
	else if (lp[1] < -0.999) v.setAt(1, -1.0);
	else if (lp[2] > 0.999) v.setAt(2, 1.0);
	else if (lp[2] < -0.999) v.setAt(2, -1.0);
	if (lo[0]<1 && lo[0]>-1 && lo[1]<1 && lo[1]>-1 && lo[2]<1 && lo[2]>-1)
		return localToGlobal(Ray4(lp, -v)).normalized();
	return localToGlobal(Ray4(lp, v)).normalized();
}

Vector4 Cube::getTextureCoordinates(const Vector4& p) const
{
	Vector4 lp = globalToLocal(p);
	if (lp[0] > 0.999 || lp[0] < -0.999)return Vector4(lp[2] / 2 + 0.5, lp[1] / 2 + 0.5, 0,1);
	if (lp[1] > 0.999 || lp[1] < -0.999)return Vector4(lp[0] / 2 + 0.5, lp[2] / 2 + 0.5, 0,1);
	if (lp[2] > 0.999 || lp[2] < -0.999)return Vector4(lp[0] / 2 + 0.5, lp[1] / 2 + 0.5, 0,1);
	return Vector4(0, 0, 0,1);
}




Ray4 Triangle::getNormal(const Vector4& impact, const Vector4& observator) const
{
	Vector3 v1(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
	Vector3 v2(p2.x - p0.x, p2.y - p0.y, p2.z - p0.z);
	Vector3 tmpN = v1.crossProduct(v2);

	Vector4 N(tmpN.x, tmpN.y, tmpN.z, 0.0);

	Ray4 r(impact,-N);

	return r;
}


bool Triangle::Intersect(const Ray4& ray, Vector4& impact) const
{
	Ray4 r = ray;

	Vector4 _p0 = Vector4(p0.x, p0.y, p0.z, 1.0);
	Vector4 _p1 = Vector4(p1.x, p1.y, p1.z, 1.0);
	Vector4 _p2 = Vector4(p2.x, p2.y, p2.z, 1.0);
	

	

	Vector3 v1(_p1.x - _p0.x, _p1.y- _p0.y, _p1.z - _p0.z);
	Vector3 v2(_p2.x - _p0.x, _p2.y - _p0.y, _p2.z - _p0.z);



	Vector3 tmpN = v1.crossProduct(v2); 
	
	Vector4 N(tmpN.x, tmpN.y, tmpN.z, 0.0);
	float n = N.getNorme();

	float ND = N.dot(r.getDirection());
	if (std::abs(ND) < std::numeric_limits<float>::epsilon()) {
		return false;
	}
		

	float d = -N.dot(_p0);
	float t = -(N.dot(r.getOrigin()) + d) / ND;

	if (t < 0) {
		return false;
	}
	
	impact = (r.getOrigin() + (r.getDirection() * t));
	Vector4 localimpact = r.getOrigin() + (r.getDirection() * t);

	Vector3 p(localimpact[0], localimpact[1], localimpact[2]);

	Vector3 vp0( p.x - _p0.x, p.y - _p0.y, p.z - _p0.z);
	Vector3 _C = v1.crossProduct(vp0);
	Vector4 C(_C.x, _C.y, _C.z, 0.0);
	if (N.dot(C) < 0) { 
		return false;
	}

	Vector3 v3(_p2.x - _p1.x, _p2.y - _p1.y, _p2.z - _p1.z);
	Vector3 vp1( p.x - _p1.x, p.y - _p1.y, p.z - _p1.z);
	_C = v3.crossProduct(vp1);
	C = Vector4(_C.x, _C.y, _C.z, 0.0);
	if (N.dot(C) < 0) {
		return false;
	}

	Vector3 v4(_p0.x - _p2.x, _p0.y - _p2.y, _p0.z - _p2.z);
	Vector3 vp2(p.x - _p2.x, p.y - _p2.y, p.z - _p2.z);
	_C = v4.crossProduct(vp2);
	C = Vector4(_C.x, _C.y, _C.z, 0.0);
	if (N.dot(C) < 0) {
		return false;
	}

	return true;
}



Mesh::Mesh(std::vector<Vector3> v)
{
	
	
	for (int i = 0; i <= v.size()-3; i+=3)
	{

		Vector4 p1(v[i].x, v[i].y, v[i].z,1.0);
		Vector4 p2(v[i+1].x, v[i+1].y, v[i+1].z, 1.0);
		Vector4 p3(v[i + 2].x, v[i + 2].y, v[i + 2].z, 1.0);;

		Triangle tr(Vector3(p1.x,p1.y,p1.z), Vector3(p3.x, p3.y, p3.z),Vector3(p2.x, p2.y, p2.z));
		Vertex.push_back(tr);
	}
	
}



bool Mesh::Intersect(const Ray4& ray, Vector4& impact) const
{
	Ray4 r = globalToLocal(ray);

	for (int i = 0; i < Vertex.size(); i++)
	{
	
		if (Vertex[i].Intersect(r, impact)) {
			impact = localToGlobal(impact);
			return true;
		}
	}

	return false;
}

Ray4 Mesh::getNormal(const Vector4& impact, const Vector4& observator) const
{
	Vector4 lp = globalToLocal(impact);
	Vector4 lo = globalToLocal(observator);

	Ray4 r = (Ray4(lo, (lp-lo).normalized()));
	Vector4 im;
	for (int i = 0; i < Vertex.size(); i++)
	{

		if (Vertex[i].Intersect(r, im)) {

			return localToGlobal( Vertex[i].getNormal(lp, lo));
		}
	}
	
	return Ray4(impact,Vector4(0,0,0,0));
}
