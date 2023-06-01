// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"


int main()
{
	int w = 50;
	int h = 50;
	int comp = 3;
	unsigned char* image = new unsigned char[1920 * 1080];

	Camera cam(300, 300,5);
	cam.translate(0, 0, -10);
	Image img(cam.getWidth(),cam.getHeight(), 3);


	std::vector<unsigned char*> arr = img.getImage();
	Scene scene;

	scene.AddToScene(dynamic_cast<Entity*>(new Sphere()), 1, 0, 0);
	scene.AddToScene(dynamic_cast<Entity*>(new Sphere()), 0, 1, 0);
	

	int idx = 0;
	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			idx++;


			float x = (float)i / img.getWidth();
			float y = (float)j / img.getHeight();

			Ray4 r = cam.getRay(x, y);

			float* c = scene.getPixelColor(r,cam);

			arr[j * img.getWidth() + i][0] = c[0];
			arr[j * img.getWidth() + i][1] = c[1];
			arr[j * img.getWidth() + i][2] = c[2];

			
		}
	}



	stbi_write_png("output.png", img.getWidth(), img.getHeight(), img.getDim(), &img.getFlatArray()[0], img.getWidth() * img.getDim());
	return 0;
}
