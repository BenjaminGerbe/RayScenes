// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"


int main()
{

	Camera cam(500, 500,5);
	cam.rotateX(-30 * ( M_PI/180.0f));


	cam.translate(0, 0, -20);
	Image img(cam.getWidth(),cam.getHeight(), 3);
	

	std::vector<unsigned char*> arr = img.getImage();
	Scene scene;

	Material basic(Color(100,10,10),Color(180,50,50),Color(255,255,255),1000);

	Material White(Color(60, 60, 100),Color(255, 255, 255),Color(255,255,255),1);

	scene.AddToScene(dynamic_cast<Entity*>(new Sphere()), basic, 1, .5, 0);
	scene.AddToScene(dynamic_cast<Entity*>(new Sphere()), basic, 0, 1.8, .5);
	scene.AddToScene(dynamic_cast<Entity*>(new Square()), White, 0, 0, 0);

	scene.getEntity(2)->rotateX(90 *(M_PI / 180.0f));
	scene.getEntity(2)->translate(0,0,-.5);
	scene.getEntity(2)->scale(.2);




	Ray4 rL2(Vector4(0, 1, 1, 1), Vector4(2, -1, 3, 0).normalized());
	Light* l2 = new Light(rL2, Color(255, 255, 255));
	scene.AddLightToScene(l2);


	float* color = new float[3]{ 32.0f,164.0f,196.0f };
	cam.setColor(color);


	int idx = 0;
	for (int i = 0; i < img.getWidth(); i++) {
		for (int j = 0; j < img.getHeight(); j++) {
			idx++;


			float x = (float)i / img.getWidth();
			float y = (float)j / img.getHeight();


			if (idx % (img.getHeight()*10) == 0) {
				std::cout << (idx / (float)(img.getWidth() * img.getHeight())) * 100 << " %" << std::endl;
			}
			


			Ray4 r = cam.getRay(x, y);
			float* c = scene.getPixelColorPhong(r,cam);

			arr[j * img.getWidth() + i][0] = c[0];
			arr[j * img.getWidth() + i][1] = c[1];
			arr[j * img.getWidth() + i][2] = c[2];

			
		}
	}


	stbi_write_png("output.png", img.getWidth(), img.getHeight(), img.getDim(), &img.getFlatArray()[0], img.getWidth() * img.getDim());
	return 0;
}
