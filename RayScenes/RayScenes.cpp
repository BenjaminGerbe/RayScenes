// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"


int main()
{
	int w;
	int h;
	int comp;
	unsigned char*  image = stbi_load("intput.png", &w, &h, &comp, 0);


	float* f = new float[16]
	{
		1, 2, 3, 4,
		1, 2, 3, 4,
		1, 2, 3, 4,
		1, 2, 3, 4
	};

	Matrix4x4 mat(f);

	f = new float[16]
	{
			1, 2, 3, 4,
			1, 2, 3, 4,
			1, 2, 3, 4,
			1, 2, 3, 4
	};

	Matrix4x4 m(f);

	std::cout << m*mat << std::endl;

	return 0;
}
