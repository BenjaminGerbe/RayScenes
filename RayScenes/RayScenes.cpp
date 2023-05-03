// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"

int main()
{
	int w = 1920;
	int h = 1080;
	int comp = 3;
	unsigned char* image = new unsigned char[1920 * 1080];


	float* f = new float[16]
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	Matrix4x4 mat(f);

	Camera cam(1920, 1080,5);
	
	cam.rotateY(10*(M_PI/180.0f));
	Image img(cam.getWidth(),cam.getHeight(), 3);

	Vector4 position(0, 0, 0, 1);
	Sphere sphere;
	sphere.translate(0,0,10);
	float max = 0;
	std::vector<unsigned char*> arr = img.getImage();
	int idx = 0;
	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			idx++;


			float x = (float)i / img.getWidth();
			float y = (float)j / img.getHeight();

			Ray4 r = cam.getRay(x, y);
			Vector4 impact;

		
			bool b = sphere.Intersect(r, impact);

			//std::cout << (idx / (float)(img.getWidth() * img.getHeight())) * 100 << std::endl;

			for (int k = 0; k < 3; k++)
			{

				if (b) {
					

					Ray4 normal = sphere.getNormal(impact, position);
					Vector3 norm(normal.getDirection().x, normal.getDirection().y, normal.getDirection().z);
					norm = norm.normalized();
					Vector3 camDir(-1,0,0);
					
					float N = (camDir.dot(norm)) * 255.0*10;

					 N = std::clamp(N,10.0f, 255.0f);
					
					arr[j * img.getWidth() + i][k] = N;


				}
				else {
					arr[j * img.getWidth() + i][k] = 0;
				}
			}

		}
	}



	stbi_write_png("output.png", img.getWidth(), img.getHeight(), img.getDim(), &img.getFlatArray()[0], img.getWidth() * img.getDim());
	return 0;
}
