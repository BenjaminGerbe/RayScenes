// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"


int main()
{
	int w;
	int h;
	int comp;
	unsigned char*  image = stbi_load("intput.png", &w, &h, &comp, 0);

	std::cout << "Hello" << std::endl;

	return 0;
}
