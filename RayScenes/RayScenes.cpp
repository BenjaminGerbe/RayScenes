// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"
#include <omp.h>
#include <chrono>

int main()
{

	Material* basic = new Material(Color(60, 60, 60), Color(255, 255, 255), Color(200, 200, 200), 50);


	basic->setColorMap(new Image("Materials/Floor/color2.jpg"));
	basic->setNormalMap(new Image("Materials/Floor/normal2.jpg"));



	Material* White = new Material(Color(60, 60, 60), Color(180, 180, 180), Color(200, 255, 255), 50);

	White->setColorMap(new Image("Materials/Floor/color.jpg"));
	White->setNormalMap(new Image("Materials/Floor/normal.jpg"));
	White->setRoughnessMap(new Image("Materials/Floor/roughness.jpg"));
	
	Camera cam(1920, 1080, 5, 90, 0.1, 10000);

	cam.rotateY(-30 * (M_PI / 180.0f));
	cam.rotateX(-30* (M_PI / 180.0f));
	cam.translate(0, 0, -15);

	Image img(cam.getWidth(),cam.getHeight(), 3);
	

	std::vector<unsigned char*> arr = img.getImage();
	Scene scene;



	
	scene.AddToScene(dynamic_cast<Entity*>(new Sphere()), basic, 0, 0, 0);
	scene.AddToScene(dynamic_cast<Entity*>(new Plan()), White, 0, 0, 0);

	scene.getEntity(1)->rotateX(90 *(M_PI / 180.0f));
	scene.getEntity(1)->translate(0,0,-.5);
	scene.getEntity(1)->scale(.2);
	



	Ray4 rL(Vector4(0, 100, -2, 1), Vector4(0, 1, -.3, -.3).normalized());
	Light* l = new Light(rL, Color(255.0f, 255.0f, 255.0f), Color(150, 150, 150));
	scene.AddLightToScene(l);

	Ray4 rL2(Vector4(0, 100, -2, 1), Vector4(-1, 0, 0, 0).normalized());
	Light* l2 = new Light(rL2, Color(50.0f, 50.0f, 255.0f), Color(200.0f, 150.0F, 150.0F));
	scene.AddLightToScene(l2);



	Color background (32.0f,164.0f,196.0f);
	cam.setColor(background);




	int width = img.getWidth();
	int height = img.getHeight();

	auto start = std::chrono::system_clock::now();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			

			float x = (float)i / width;
			float y = (float)j / height;

			if ( j ==  0) {
				std::cout << (int)(x*100) << " %" << std::endl;
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

	
	std::string input;
	input = "output.png";
	char* path = new char[input.length() + 1];
	strcpy(path, input.c_str());



	img = img.blur(img);

	img.WriteImage(path);



	return 0;
}
