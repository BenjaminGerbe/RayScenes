// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"
#include <omp.h>
#include <chrono>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

std::string inputfile = "Chess-pieces-Knight.obj";
tinyobj::ObjReaderConfig reader_config;

tinyobj::ObjReader reader;


auto& attrib = reader.GetAttrib();
auto& shapes = reader.GetShapes();
auto& materials = reader.GetMaterials();

std::vector<Vector3> LoadMesh() {

	if (!reader.ParseFromFile(inputfile, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}


	std::vector<Vector3> vertex;
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				vertex.push_back(Vector3(vx, vy, vz));

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	return vertex;
}

int main(int argc, char* argv[])
{

	std::vector<Vector3> vert = LoadMesh();


	int argH = 300;
	int argW = 300;
	int argFov = 90;
	char* sceneName;
	

	for (int i = 0; i < argc; i++) {	
	
		if (std::strcmp(argv[i],"-height") == 0) {
			argH = std::atoi(argv[i + 1]);
		}

		if (std::strcmp(argv[i], "-width") == 0) {
			argW = std::atoi(argv[i + 1]);
		}

		if (std::strcmp(argv[i], "-fov") == 0) {
			argFov = std::atoi(argv[i + 1]);
		}

		if (std::strcmp(argv[i], "-scene") == 0) {
			argFov = std::atoi(argv[i + 1]);
		}

		if (std::strcmp(argv[i], "-noShadow") == 0) {
			
		}

		if (std::strcmp(argv[i], "-noTexture") == 0) {

		}
	}
	

	Material* basic = new Material(Color(60, 60, 60), Color(255, 255, 255), Color(200, 200, 200), 50);


	basic->setColorMap(new Image("Materials/Floor/color2.jpg"));
	basic->setNormalMap(new Image("Materials/Floor/normal2.jpg"));



	Material* White = new Material(Color(60, 60, 60), Color(180, 180, 180), Color(200, 255, 255), 50);

	White->setColorMap(new Image("Materials/Floor/color.jpg"));
	White->setNormalMap(new Image("Materials/Floor/normal.jpg"));
	White->setRoughnessMap(new Image("Materials/Floor/roughness.jpg"));
	
	Camera cam(argW, argH, 5, argFov, 0.1, 10000);

	cam.rotateY(-30 * (M_PI / 180.0f));
	cam.rotateX(-30* (M_PI / 180.0f));
	cam.translate(0, 0, -15);

	Image img(cam.getWidth(),cam.getHeight(), 3);
	

	std::vector<unsigned char*> arr = img.getImage();
	Scene scene;


	//scene.AddToScene(dynamic_cast<Entity*>(new Sphere()), basic, 0, 0, 0);
	//scene.AddToScene(dynamic_cast<Entity*>(new Triangle(Vector3(-1,-1,0), Vector3(1, -1, 0),Vector3(-1, 1, 0))), basic, 0, 0, 0);
	scene.AddToScene(dynamic_cast<Entity*>(new Mesh(vert)), basic, 0, 0, 0);

	scene.getEntity(0)->rotateX(90.0f *(M_PI / 180.0f));
	scene.getEntity(0)->scale(1);

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



	img.WriteImage(path);



	return 0;
}



