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

	Vector4 vec(1, 2, 3,4);

	std::cout << vec << std::endl;
	std::cout << vec.x << std::endl;

	vec.setAt(0, 2);

	std::cout << vec << std::endl;
	std::cout << vec.x << std::endl;

	return 0;
}
