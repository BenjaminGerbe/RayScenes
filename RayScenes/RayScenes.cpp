﻿// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"

int main()
{
	int w = 1920;
	int h = 1080;
	int comp = 3;
	unsigned char* image = new unsigned char[1920 * 1080];


	float* f = new float[9]
	{
		1, 5, 3,
			4, 5, 6,
			7, 8, 9
	};

	Matrix3x3 mat(f);

	Image img(1920, 1080, 3);

	Camera cam;
	Plan plan;
	plan.translate(0, 0, 10);

	std::vector<unsigned char*> arr = img.getImage();

	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			for (int k = 0; k < 3; k++)
			{

				arr[j * img.getWidth() + i][k] = 255;
			}

			float x = (float)i / img.getWidth();
			float y = (float)j / img.getHeight();

			Ray r = cam.getRay(x, y);
			Ray4 r4(r);
			Vector4 impact;

		
			bool b = plan.Intersect(r4, impact);

			if (b) {
				std::cout << "IMPACT" << std::endl;
			}

			

		}
	}

	Vector3 vec(1, 2, 3);

	std::cout << mat * vec << std::endl;
	;

	stbi_write_png("output.png", img.getWidth(), img.getHeight(), img.getDim(), &img.getFlatArray()[0], img.getWidth() * img.getDim());
	return 0;
}
