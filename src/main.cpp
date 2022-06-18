#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>

#include "Camera.h"
#include "glm/glm.hpp"

bool hit_sphere(Ray ray, glm::vec3 center, float r) {
	glm::vec3 oc = ray.origin - center;
	float a = glm::dot(ray.direction, ray.direction);
	float b = 2 * glm::dot(oc, ray.direction);
	float c = glm::dot(oc, oc) - r*r;

	float d = b*b - 4*a*c;
	return d > 0;
}

glm::vec3 ray_color(Ray ray) {
	if (hit_sphere(ray, {0, 0, -1}, 0.5)) {
		return glm::vec3(1, 0, 0);
	}
	glm::vec3 u_dir = glm::normalize(ray.direction);
	float y = 0.5*u_dir.y + 1;

	return (1-y) * glm::vec3(1) + y * glm::vec3(0.5, 0.7, 1.0);
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
	for (int i = h-1; i > -1; i--){
		for (int j = 0; j < w; j++) {
			float u = (float) j / (w-1);
			float v = (float) i / (h-1);
			glm::vec3 color = ray_color(camera.castRay(u, v));
			std::fprintf(image, "%i %i %i\n", (int)(color.r*255), (int)(color.g*255), (int)(color.b*255));
		}
	}
}
