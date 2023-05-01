// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"

int main()
{
	int w;
	int h;
	int comp;
	unsigned char*  image = stbi_load("intput.png", &w, &h, &comp, 0);


	float* f = new float[9]
	{
		1, 5, 3,
		4, 5, 6,
		7, 8, 9
	};

	Matrix3x3 mat(f);


	Vector3 vec(1, 2, 3);

	std::cout << mat*vec << std::endl;



	return 0;
}
