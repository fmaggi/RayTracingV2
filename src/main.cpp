#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>

#include "renderer.h"

int main(int argc, char** argv) {
	std::printf("Hello raytraced world\n");

	if (argc != 2) {
		std::printf("Usage:\n raytrace [output]\n");
		return -1;
	}

	float a = 16.0 / 9.0;

	Camera camera(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 70, a);

	Scene scene;
	scene.background = [](Ray r) {
				glm::vec3 u_dir = glm::normalize(r.direction);
				float y = 0.5*u_dir.y + 1;
				return (1-y) * glm::vec3(1) + y * glm::vec3(0.5, 0.7, 1.0);
		};

	Material* redMetal = scene.addMaterial<Lambertian>("red", glm::vec3(0.8, 0.8, 0.0));
	Material* diffuse = scene.addMaterial<Lambertian>("d", glm::vec3(0.7, 0.3, 0.3));
	Material* metal = scene.addMaterial<Metal>("m", glm::vec3(0.8, 0.8, 0.8), 0.3);
	Material* metal2 = scene.addMaterial<Metal>("m2", glm::vec3(0.8, 0.6, 0.2), 1.0f);

	scene.add<Sphere>(glm::vec3(0, 0, -1), 0.5f, diffuse);
  scene.add<Sphere>(glm::vec3(0,-100.5,-1), 100, redMetal);
	scene.add<Sphere>(glm::vec3(1, 0, -1), 0.5f, metal);
	scene.add<Sphere>(glm::vec3(-1, 0, -1), 0.5f, metal2);

	Renderer renderer(400*a, 400, 50);

	Image im = renderer.render(camera, scene);

	std::string path = "output/" + std::string(argv[1]);
	FILE* image = std::fopen(path.c_str(), "wb");
	std::fprintf(image, "P6\n%i %i\n255\n", im.width, im.height);
	std::fwrite(im.pixels, im.pixelSize, im.width*im.height, image);
}
