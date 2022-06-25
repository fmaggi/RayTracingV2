#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>

#include "Camera.h"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "object.h"
#include "scene.h"
#include "utils/math.h"
#include "shader.h"
#include "image.h"

glm::vec3 ray_color(Ray ray, Scene& scene, int depth) {
	if (depth <= 0) {
		return glm::vec3(0);
	}

  auto info = scene.intersect(ray);
  if (info){
		glm::vec3 dir = info->n + Math::randomInUnitSphere();
		Ray reflection(info->p, dir);
		return 0.5f * ray_color(reflection, scene, depth--);
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


	int numberOfRays = 300;
	Camera camera(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 70, a);
	int w = camera.viewportWidth() * numberOfRays;
	int h = camera.viewportHeight() * numberOfRays;

	Image im(w, h);

	Scene scene;
	scene.setBackgroundColor(
			[](Ray r) {
				glm::vec3 u_dir = glm::normalize(r.direction);
				float y = 0.5*u_dir.y + 1;
				return (1-y) * glm::vec3(1) + y * glm::vec3(0.5, 0.7, 1.0);
			});

	scene.add<Sphere>(glm::vec3(0, 0, -1), 0.5f);
	scene.add<Sphere>(glm::vec3(0,-100.5,-1), 100);

	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++) {
			glm::vec3 color{};
			for (int k = 0; k < 8; k++) {
				float u = (float) (j + Math::random<float>()) / (w-1);
				float v = (float) (i + Math::random<float>()) / (h-1);
				color += ray_color(camera.castRay(u, v), scene, 50);
			}
			color = normalizeColor(color, 8);
			auto& pixel = im.at(h-i-1, j);
			pixel.r = (uint8_t)(color.r * 255);
			pixel.g = (uint8_t)(color.g * 255);
			pixel.b = (uint8_t)(color.b * 255);
		}
	}

	std::string path = "output/" + std::string(argv[1]);
	FILE* image = std::fopen(path.c_str(), "wb");
	std::fprintf(image, "P6\n%i %i\n255\n", w, h);
	std::fwrite(im.pixels, im.pixelSize, w*h, image);
}
