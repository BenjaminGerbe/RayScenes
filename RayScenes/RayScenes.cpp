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

dataMesh LoadMesh(std::string path) {

	if (!reader.ParseFromFile(path, reader_config)) {
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


Scene LoadScene(char* scaneName, Camera& cam, std::vector<Material*>& mats) {
	std::string pretty_json;

	std::ifstream myfile(scaneName);
	if (myfile.is_open())
	{
		std::stringstream buffer;
		buffer << myfile.rdbuf(); // Lire tout le contenu du fichier dans le flux

		pretty_json = buffer.str(); // Convertir le flux en std::string

		myfile.close();
	}

	JS::ParseContext context(pretty_json);
	SceneParser sp;
	context.parseTo(sp);


	Scene scene;

	// Load Camera
	Color background(sp.camera.backgroundColor[0], sp.camera.backgroundColor[1], sp.camera.backgroundColor[2]);
	cam.setColor(background);
	cam.rotateY(sp.camera.angle[1]);
	cam.rotateX(sp.camera.angle[0]);
	cam.rotateZ(sp.camera.angle[2]);

	cam.translate(sp.camera.position[0], sp.camera.position[1], sp.camera.position[2]);

	// Load material
	std::cout << "Texture Loading ..." << std::endl;
	for (int i = 0; i < sp.materials.size(); i++)
	{
		float* a = sp.materials[i].ambiante;
		float* d = sp.materials[i].diffuse;
		float* s = sp.materials[i].specular;
		Material* mat = new Material(Color(a[0], a[1], a[2]), Color(d[0], d[1], d[2]), Color(s[0], s[1], s[2]), sp.materials[i].si);

		if (sp.materials[i].colorMapPath != "") {
			const int length = sp.materials[i].colorMapPath.length();
			char* char_array = new char[length + 1];
			strcpy(char_array, sp.materials[i].colorMapPath.c_str());
			mat->setColorMap(new Image(char_array));
		}

		if (sp.materials[i].normalMapPath != "") {
			const int length = sp.materials[i].normalMapPath.length();
			char* char_array = new char[length + 1];
			strcpy(char_array, sp.materials[i].normalMapPath.c_str());
			mat->setNormalMap(new Image(char_array));
		}

		mats.push_back(mat);
	}

	// Load entity
	for (int i = 0; i < sp.entities.size(); i++)
	{
		std::string type = sp.entities[i].type;
		Entity* entity;
		if (type == "Plan") {
			entity = new Plan();
			scene.AddToScene(entity, mats[sp.entities[i].idMaterial], 0, 0, 0);
		}

		if (type == "Cube") {
			entity = new Cube();
			scene.AddToScene(entity, mats[sp.entities[i].idMaterial], 0, 0, 0);
		}

		if (type == "Sphere") {
			entity = new Sphere();
			scene.AddToScene(entity, mats[sp.entities[i].idMaterial], 0, 0, 0);
		}

		if (type == "Mesh") {
			std::cout << "Mesh Loading ..." << std::endl;
			dataMesh d = LoadMesh(sp.entities[i].meshPath);
			entity = new Mesh(d.v, d.n);
			scene.AddToScene(entity, mats[sp.entities[i].idMaterial], 0, 0, 0);
		}

		if (type == "Square") {
			entity = new Square();
			scene.AddToScene(entity, mats[sp.entities[i].idMaterial], 0, 0, 0);
		}

		if (type == "InfCylender") {
			entity = new InfCylender();
			scene.AddToScene(entity, mats[sp.entities[i].idMaterial], 0, 0, 0);
		}

		entity->translate(sp.entities[i].position[0], sp.entities[i].position[1], sp.entities[i].position[2]);

		entity->rotateX(sp.entities[i].angle[0]);
		entity->rotateY(sp.entities[i].angle[1]);
		entity->rotateZ(sp.entities[i].angle[2]);

		entity->scale(sp.entities[i].scale[0], sp.entities[i].scale[1], sp.entities[i].scale[2]);

	}

	for (int i = 0; i < sp.lights.size(); i++)
	{
		Ray4 rL(Vector4(0,0,0,1.0), Vector4(sp.lights[i].direction[0], sp.lights[i].direction[1], sp.lights[i].direction[2], 0.0f).normalized());
		Light* l;
		if (sp.lights[i].type == "Pointer") {
			 l = new Light(rL, Color(sp.lights[i].color[0], sp.lights[i].color[1], sp.lights[i].color[2]), Color(sp.lights[i].specular[0], sp.lights[i].specular[1], sp.lights[i].specular[2]), Point);
		}
		else {
			l = new Light(rL, Color(sp.lights[i].color[0], sp.lights[i].color[1], sp.lights[i].color[2]), Color(sp.lights[i].specular[0], sp.lights[i].specular[1], sp.lights[i].specular[2]));
		}
		
		
		l->translate(sp.lights[i].position[0], sp.lights[i].position[1], sp.lights[i].position[2]);
		scene.AddLightToScene(l);
	}

	return scene;
}


int main(int argc, char* argv[])
{
	float* f = new float[16] {
		1, 2, 3, 4,
			1, 2, 3, 4,
			1, 2, 3, 4,
			1, 2, 3, 4
	};

	Matrix4x4 mat(f);

	int argH = 300;
	int argW = 500;
	int argFov = 90;
	bool shadow = true;
	std::string input;
	std::string name = "scene1.json";
	input = "output.jpg";
	char* path = new char[input.length() + 1];
	char* sceneName = new char[name.length() + 1];
	strcpy(path, input.c_str());
	strcpy(sceneName, name.c_str());
	bool aa = false;
	float sample = 0.01f;

	for (int i = 0; i < argc; i++) {

		if (std::strcmp(argv[i], "-h") == 0) {
			argH = std::atoi(argv[i + 1]);
		}

		if (std::strcmp(argv[i], "-w") == 0) {
			argW = std::atoi(argv[i + 1]);
		}

		if (std::strcmp(argv[i], "-f") == 0) {
			argFov = std::atoi(argv[i + 1]);
		}

		if (std::strcmp(argv[i], "-s") == 0) {
			sceneName = (argv[i + 1]);
		}

		if (std::strcmp(argv[i], "-ns") == 0) {
			shadow = false;
		}

		if (std::strcmp(argv[i], "-o") == 0) {
			path = (argv[i + 1]);
		}

		if (std::strcmp(argv[i], "-aa") == 0) {
			aa = true;
			sample = std::atoi(argv[i + 1]) / 1000.0f;
			std::cout << sample << std::endl;
		}

	}



	Camera cam(argW, argH, 5, argFov, 0.1, 10000);
	Image img(cam.getWidth(), cam.getHeight(), 3);
	std::vector<unsigned char*> arr = img.getImage();
	std::vector<Material*>  mats;
	Scene scene = LoadScene(sceneName, cam, mats);

	int width = img.getWidth();
	int height = img.getHeight();

	// debut du timer
	auto start = std::chrono::system_clock::now();

	// fonction qui fait le rendu
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float x = (float)i / width;
			float y = (float)j / height;

			if (j == 0) {
				std::cout << (float)(x * 100.0f) << " %" << std::endl;
			}


			Color c;

			if (!aa) {
				Ray4 ray = cam.getRay(x, y);
				c = scene.getPixelColorPhong(ray, cam, shadow);
			}
			else {

				float r = 0.0f, g = 0.0f, b = 0.0f;
				for (int i = 0; i < 4; i++)
				{
					Ray4 ray = cam.getRaySampling(x, y, sample);
					c = scene.getPixelColorPhong(ray, cam, shadow);
					r += c.r;
					g += c.g;
					b += c.b;
				}

				c = Color(r / 4.0f, g / 4.0f, b / 4.0f);

			}


			arr[j * width + i][0] = c.r;
			arr[j * width + i][1] = c.g;
			arr[j * width + i][2] = c.b;
		}
	}

	// fin du timer
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << elapsed_seconds.count() << std::endl;

	img.WriteImage(path);

	return 0;
}


