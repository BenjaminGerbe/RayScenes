#include "Entity.h"
#include <algorithm>

Ray4 Camera::getRay(float x, float y) const
{

	float ratio = width / height;

	Vector3  origin(((x * 2) - 1) * ratio, (y * 2) - 1, 0);
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
	Ray4 p;

	Vector4 origin = globalToLocal(r.getOrigin());
	Vector4 direction = globalToLocal(r.getDirection());
	
	p.setDirection(direction);
	p.setOrigin(origin);

	return p;
}

bool Entity::Intersect(const Ray4& ray, Vector4& impact) const
{
	return true;
}

Ray4 Entity::getNormal(const Vector4& impact, const Vector4& observator) const
{
	return Ray4();
}

Material Entity::GetMat()
{
	return mat;
}


void Entity::SetMat(Material mat)
{
	this->mat = mat;
}

Plan::Plan()
{

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

	Vector4 vec(0, 0, 1,0);

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
		vec.setAt(2, 1);
	}

	Ray4 r(impact, localToGlobal(vec));

	return r;
}

Sphere::Sphere()
{
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
	Vector4 p = globalToLocal(impact);
	Vector4 obs = globalToLocal(observator);


	Vector4 vec;
	vec = p;

	if (obs[0] * obs[0] + obs[1] * obs[1] + obs[2] * obs[2] <= 1) {
		vec = -vec;
	}

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


float* Scene:: getPixelColor(Ray4 ray,Camera cam) {
	float* color = new float[3]{ cam.getBackgroundColor()[0], cam.getBackgroundColor()[1], cam.getBackgroundColor()[2] };
	Vector4 impact;
	float depth = 100000;
	for (int i = 0; i < lstObject.size(); i++)
	{

		if (lstObject[i]->Intersect(ray, impact) ) {


			float tmp = cam.globalToLocal(impact).getNorme();
			if (tmp >= depth) continue;
			float n = 0;
			Color src( lstObject[i]->GetMat().getAmbiante().r,  lstObject[i]->GetMat().getAmbiante().g,  lstObject[i]->GetMat().getAmbiante().b);
			for (int j = 0; j < lstLights.size(); j++)
			{

				Ray4 vec = lstObject[i]->globalToLocal( lstObject[i]->getNormal(impact, cam.getPoistion()));
				Vector4 normal = (vec.getDirection().normalized());
				
			
				Vector4 dir = lstObject[i]->globalToLocal( lstLights[j]->getRay().getDirection()).normalized();
				
				float N = normal.dot(dir) ;
				N = std::clamp(N, 0.0f, 255.0f);

				depth = tmp;
				
				if (isOnShadow(impact, *lstLights[j] ,lstObject[i])) N = 0;

				n += N;
				

			}

			Color diffuse = lstObject[i]->GetMat().getDiffuse();


			color[0] = std::clamp(src.r + diffuse.r*n , 0.0f,255.0f);
			color[1] = std::clamp(src.g + diffuse.g * n, 0.0f, 255.0f);
			color[2] = std::clamp(src.b + diffuse.b * n, 0.0f, 255.0f);

		}
	}

	return color;
}


bool Scene::isOnShadow(Vector4 point,Light l, Entity* ent) {
	bool result = false;
	for (int i = 0; i < lstObject.size(); i++)
	{
		if (ent == lstObject[i]) continue;
		Ray4 r(point, l.getRay().getDirection());
		Vector4 vec;
		if (lstObject[i]->Intersect(r, vec)) {
			result = true;
		}
		
	}

	return result;
}

void Scene::AddToScene(Entity* ent, Material mat, float x, float y, float z) {
	ent->translate(x, y, z);
	ent->SetMat(mat);
	lstObject.push_back(ent);
}

void Scene::AddLightToScene(Light* l) {
	lstLights.push_back(l);
}