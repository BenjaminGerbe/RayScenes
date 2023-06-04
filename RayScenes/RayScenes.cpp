// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"
#include <omp.h>
#include <chrono>

int main()
{

	

	Camera cam(300, 300,5);

	cam.rotateX(-30* (M_PI / 180.0f));

	cam.translate(0, 0, -7);
	Image img(cam.getWidth(),cam.getHeight(), 3);
	

	std::vector<unsigned char*> arr = img.getImage();
	Scene scene;

	Material* basic = new Material(Color(1, 1, 1),Color(255, 255, 255),Color(200,200, 200),50);


	basic->setColorMap(new Image("Materials/Grass/colorMap.jpg"));
	basic->setNormalMap(new Image("Materials/Grass/normalMap.jpg"));


	Material* White = new Material(Color(60, 60, 60),Color(180, 180, 180),Color(255,255,255),1);

	
	scene.AddToScene(dynamic_cast<Entity*>(new Sphere()), basic, 0, 0, 0);
	scene.AddToScene(dynamic_cast<Entity*>(new Square()), White, 0, 0, 0);

	scene.getEntity(1)->rotateX(90 *(M_PI / 180.0f));
	scene.getEntity(1)->translate(0,0,-.5);
	scene.getEntity(1)->scale(.2);
	



	Ray4 rL(Vector4(0, 100, -2, 1), Vector4(0, 2, -.5, -.3).normalized());
	Light* l = new Light(rL, Color(255.0f, 255.0f, 255.0f), Color(200.0f, 200.0f, 200.0f));
	scene.AddLightToScene(l);



	Color background (32.0f,164.0f,196.0f);
	cam.setColor(background);


	int idx = 0;

	int width = img.getWidth();
	int height = img.getHeight();

	auto start = std::chrono::system_clock::now();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			idx++;

			float x = (float)i / width;
			float y = (float)j / height;

			if (idx % (height) == 0) {
				std::cout << (idx / (float)(img.getWidth() * img.getHeight())) * 100 << " %" << std::endl;
			}
			
			Ray4 r = cam.getRay(x, y);
			Color c = scene.getPixelColorPhong(r,cam);

			arr[j * width + i][0] = c.r;
			arr[j * width + i][1] = c.g;
			arr[j * width + i][2] = c.b;

			
		}
	}	
	
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << elapsed_seconds.count() << std::endl;


	img.WriteImage("output.png");


	return 0;
}
