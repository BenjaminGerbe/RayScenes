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

	cam.rotateX(-10 * (M_PI / 180.0f));
	
	Image img(cam.getWidth(),cam.getHeight(), 3);

	Vector4 campos(0,0,0,1);
	Vector3 position(50, -50,-5);
	Square plan;

	
	plan.translate(0,-1,6);
	plan.rotateX(-80 * (M_PI / 180.0f));
	plan.scale(0.5);
	Sphere sphere;

	sphere.translate(0, -1, 6);

	Vector4 impact;
	Vector4 impact2;
	Vector4 impact3;

	Vector3 lightDir(0, 0, 0);
	lightDir = position.normalized();
	Vector4 L(lightDir.x, lightDir.y, lightDir.z, 0);

	float max = 0;
	std::vector<unsigned char*> arr = img.getImage();
	int idx = 0;
	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			idx++;


			float x = (float)i / img.getWidth();
			float y = (float)j / img.getHeight();

			Ray4 r = cam.getRay(x, y);
			
		
			bool b = plan.Intersect(r,impact);

			bool b2 = sphere.Intersect(r, impact2);



			Ray4 ombre(impact, L);

			bool b3 = sphere.Intersect(ombre, impact);

			//std::cout << (idx / (float)(img.getWidth() * img.getHeight())) * 100 << std::endl;

			for (int k = 0; k < 3; k++)
			{

				if (b && !b2 || (b && b2 && impact.getNorme() < impact2.getNorme() )) {
					

					Ray4 normal = plan.getNormal(impact, campos);
					Vector3 norm(normal.getDirection().x, normal.getDirection().y, normal.getDirection().z);
					norm = norm.normalized();
			

					if (!b3) {
						float N = (lightDir.dot(norm)) * 255.0 * 10;

						N = std::clamp(N, 10.0f, 255.0f);

						arr[j * img.getWidth() + i][k] = N;
					}
					else {
						arr[j * img.getWidth() + i][k] = 10.0f;
					}

				
				}
				else if (b2) {

					Ray4 normal = sphere.getNormal(impact2, campos);
					Vector3 norm(normal.getDirection().x, normal.getDirection().y, normal.getDirection().z);
					norm = norm.normalized();
					Vector3 lightDir(0, 0, 0);
					lightDir = position.normalized();



					float N = (lightDir.dot(norm)) * 255.0 * 10;

					N = std::clamp(N, 10.0f, 255.0f);

					arr[j * img.getWidth() + i][0] = N;

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
