// RayScenes.cpp : Defines the entry point for the application.
//

#include "RayScenes.h"
#include <omp.h>
#include <chrono>
#include "json_struct.h"
#include <fstream>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

std::string inputfile = "Chess_Knights.obj";
tinyobj::ObjReaderConfig reader_config;

tinyobj::ObjReader reader;


auto& attrib = reader.GetAttrib();
auto& shapes = reader.GetShapes();
auto& materials = reader.GetMaterials();

struct dataMesh {
	std::vector<Vector3> n;
	std::vector<Vector3> v;
};

dataMesh LoadMesh() {

	if (!reader.ParseFromFile(inputfile, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}
	dataMesh d;
	
	std::vector<Vector3> vertex;
	std::vector<Vector3> normals;
	// Loop over shapes
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

				if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
					normals.push_back(Vector3(nx, ny, nz));
				
				}
		
				vertex.push_back(Vector3(vx, vy, vz));
	

			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	d.v = vertex;
	d.n = normals;


	return d;
}


void foo() {
	Vector4 v1(0, 0, 0, 0);
	Vector4 v2(0, 0, 0, 0);
	Ray4 r(v1,v2);
}



void ReadJson() {
	std::string pretty_json;

	std::ifstream myfile("scene0.json");
	if (myfile.is_open())
	{
		std::stringstream buffer;
		buffer << myfile.rdbuf(); // Lire tout le contenu du fichier dans le flux

		pretty_json = buffer.str(); // Convertir le flux en std::string

		std::cout << pretty_json << std::endl; // Afficher le contenu du fichier

		myfile.close();
	}

	JS::ParseContext context(pretty_json);
	SceneParser sp2;
	context.parseTo(sp2);
}

int main(int argc, char* argv[])
{
	std::cout << "Mesh Loading ..." << std::endl;
	dataMesh d = LoadMesh();
	std::vector<Vector3> vert = d.v;
	std::vector<Vector3> normals = d.n;

	int argH = 50;
	int argW = 50;
	int argFov = 90;
	char* sceneName;


	SceneParser sp;
	CameraParser CP;
	EntityParser EP;
	CP.position[0] = 1;
	sp.camera = CP;
	sp.entities.push_back(EP);
	sp.entities.push_back(EP);
	MaterialParser MP;
	sp.materials.push_back(MP);
	LightParser LP;
	sp.lights.push_back(LP);
	std::string pretty_json = JS::serializeStruct(sp);
	std::cout << pretty_json << std::endl;



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
	

	std::cout << "Texture Loading ..." << std::endl;
	Material* basic = new Material(Color(60, 60, 60), Color(255, 255, 255), Color(200, 200, 200), 50);

	basic->setColorMap(new Image("Materials/Floor/color.jpg"));
	basic->setNormalMap(new Image("Materials/Floor/normal.jpg"));



	Material* White = new Material(Color(60, 60, 60), Color(255, 255, 255), Color(200, 255, 255), 50);

	//White->setColorMap(new Image("Materials/Floor/color.jpg"));
	//White->setNormalMap(new Image("Materials/Floor/normal.jpg"));
	//White->setRoughnessMap(new Image("Materials/Floor/roughness.jpg"));
	
	Camera cam(argW, argH, 5, argFov, 0.1, 10000);

	cam.rotateY((180.0f-60.0f) * (M_PI / 180.0f));
	cam.rotateX(-10 * (M_PI / 180.0f));
	cam.translate(0, -2, -25);
	

	Image img(cam.getWidth(),cam.getHeight(), 3);
	

	std::vector<unsigned char*> arr = img.getImage();
	Scene scene;

	
	scene.AddToScene(dynamic_cast<Entity*>(new Plan()), basic, 0, 0, 0);
	scene.AddToScene(dynamic_cast<Entity*>(new Mesh(vert,normals)), White, 0, 0, 0);
	//scene.AddToScene(dynamic_cast<Entity*>(new Cube()), White, 0, 0, 0);
	//scene.AddToScene(dynamic_cast<Entity*>(new Triangle(Vector3(-1,-1,5), Vector3(1, -1, 1),Vector3(-1, 1, 1))), basic, 0, 0, 0);

	scene.getEntity(0)->rotateX(90 * (M_PI / 180.0f));
	scene.getEntity(0)->translate(0.0,0.0f,-5.0);

	scene.getEntity(1)->scale(1);
	scene.getEntity(1)->rotateX(180 * (M_PI / 180.0f));
	scene.getEntity(1)->translate(0, 4.5, 0);

	

	Ray4 rL(Vector4(0, 100, -2, 1), Vector4(1, 1, -1, 0).normalized());
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
				std::cout << (float)(x*100.0f) << " %" << std::endl;
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
	input = "outputt.png";
	char* path = new char[input.length() + 1];
	strcpy(path, input.c_str());



	img.WriteImage(path);



	return 0;
}



