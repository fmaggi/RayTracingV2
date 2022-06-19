#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>

#include "Camera.h"
#include "glm/glm.hpp"
#include "object.h"
#include "scene.h"

glm::vec3 ray_color(Ray ray, Scene& scene) {
  auto info = scene.intersect(ray);
  if (info){
		return 0.5f * (info->n + glm::vec3(1,1,1));
  }
	return scene.backgroundColor(ray);
}

int main(int argc, char** argv) {
	std::printf("Hello raytraced world\n");

	if (argc != 2) {
		std::printf("Usage:\n raytrace [output]\n");
		return -1;
	}

	float a = 16.0 / 9.0;
	// int w = 400;
	// int h = 400 / a;

	std::string path = "output/" + std::string(argv[1]);
	FILE* image = std::fopen(path.c_str(), "w");

	int numberOfRays = 150;
	Camera camera(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 70, a);
	int w = camera.viewportWidth() * numberOfRays;
	int h = camera.viewportHeight() * numberOfRays;

	std::fprintf(image, "P3\n%i %i\n255\n", w, h);

	Scene scene;
//	scene.setBackgroundColor(
//			[](Ray r) {
//				glm::vec3 u_dir = glm::normalize(r.direction);
//				float y = 0.5*u_dir.y + 1;
//				return (1-y) * glm::vec3(1) + y * glm::vec3(0.5, 0.7, 1.0);
//			});

	scene.add<Sphere>(glm::vec3(0, 0, -1), 0.5f);

	for (int i = h-1; i > -1; i--){
		for (int j = 0; j < w; j++) {
			float u = (float) j / (w-1);
			float v = (float) i / (h-1);
			glm::vec3 color = ray_color(camera.castRay(u, v), scene);
			std::fprintf(image, "%i %i %i\n", (int)(color.r*255), (int)(color.g*255), (int)(color.b*255));
		}
	}
}
