// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"


void foo() {
	Matrix<100,100> mat;
	std::cout << "";
}

int main()
{
	int w;
	int h;
	int comp;
	unsigned char*  image = stbi_load("intput.png", &w, &h, &comp, 0);


	foo();

	float* f = new float[9]
	{
		1, 5, 3,
		4, 5, 6,
		7, 8, 9
	};

	Matrix3x3 mat(f);

	f = new float[16]
	{
		1, 2, 3, 4,
		1, 2, 3, 4,
		1, 2, 3, 4,
		1, 2, 3, 4
	};

	Matrix4x4 m(f);

	Vector<5> vec;

	std::cout << mat << std::endl;
	std::cout << mat.getInverse() << std::endl;

	return 0;
}
