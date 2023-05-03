// RayScenes.cpp : Defines the entry point for the application.
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

	std::vector<unsigned char*> arr = img.getImage();

	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			for (int k = 0; k < 3; k++)
			{

				arr[j * img.getWidth() + i][k] = 255;
			}

			float x = i / img.getWidth();
			float y = j / img.getHeight();


		}
	}

	Vector3 vec(1, 2, 3);

	std::cout << mat * vec << std::endl;
	;

	stbi_write_png("output.png", img.getWidth(), img.getHeight(), img.getDim(), &img.getFlatArray()[0], img.getWidth() * img.getDim());
	return 0;
}
