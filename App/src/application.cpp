#include "application.h"
#include <iostream>

#include "Raytracing.h"

#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::seconds;

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

static void Init() {

}

Application* Application::Create(ApplicationSpecification spec) {
	if (app == nullptr) {
		Init();
		app = new Application(spec);
	}
	return app;
}

Application::Application(ApplicationSpecification spec) {
    this->spec = spec;
}

void Application::run() {
    float a = 16.0f / 9.0f;

	Camera camera(glm::vec3(0, 0.5f, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 70, a);

//	Scene scene = randomScene();
	Scene scene;
	Material* left = scene.addMaterial<Dielectric>("l", glm::vec3(1.0f), 1.5f);
	Material* ground = scene.addMaterial<Lambertian>("g", glm::vec3(0.8, 0.8, 0.0));
	Material* center = scene.addMaterial<Lambertian>("c", glm::vec3(0.1, 0.2, 0.5));
	Material* right = scene.addMaterial<Metal>("r", glm::vec3(0.8, 0.6, 0.2));
	Material* em = scene.addMaterial<Emissive>("e", glm::vec3(1.f), 30);
	scene.add<Sphere>(glm::vec3(0, -100.5f, -1), 100, ground);
	scene.add<Sphere>(glm::vec3(0, 0.f, -1), 0.5f, center);
	scene.add<Sphere>(glm::vec3(1, 0.f, -1), 0.5f, right);
	scene.add<Sphere>(glm::vec3(-1, 0.5f, -1), 0.5f, left);
	scene.add<Sphere>(glm::vec3(-1, 0.5f, -1), -0.4f, left);
	scene.add<Sphere>(glm::vec3(1, 0., 2), 0.5f, em);
	scene.add<Sphere>(glm::vec3(-1, 8, -1), 0.5f, em);
	scene.background = [](Ray r) {
			return glm::vec3(0.03f);
		};

	Renderer renderer;
	renderer.imageWidth = 400*a;
	renderer.imageHeight = 400;
	renderer.samples = 5;

	auto start = high_resolution_clock::now();

	const Image& im = renderer.render(camera, scene);

	while (renderer.renderedRows() < im.height) {
		std::fprintf(stderr, "\rLine %u/%u", renderer.renderedRows(), im.height);
	}
	renderer.wait();
	std::printf("\n");

	auto stop = high_resolution_clock::now();

	auto ms_int = duration_cast<seconds>(stop-start);

	std::printf("Done in %lis\n", ms_int.count());

	std::string path = "output/" + spec.outputFile;
	FILE* image = std::fopen(path.c_str(), "wb");
	std::fprintf(image, "P6\n%i %i\n255\n", im.width, im.height);
	std::fwrite(im.pixels, im.pixelSize, im.width*im.height, image);
}
