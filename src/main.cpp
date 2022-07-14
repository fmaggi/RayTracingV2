#include <cstdio>
#include <string>

#include "material.h"
#include "renderer.h"
#include "utils/math.h"

static Scene randomScene() {
	Scene scene;
	Material* groundMaterial = scene.addMaterial<Lambertian>("ground", glm::vec3(0.5f));
	scene.add<Sphere>(glm::vec3(0, -1000, 0), 1000, groundMaterial);

	for (int a = -11; a < 11; ++a) {
		for (int b = -11; b < 11; ++b) {
			float chooseMat = Math::random<float>();
			glm::vec3 center(a + 0.9*Math::random<float>(), 0.2, b + 0.9*Math::random<float>());

			if (glm::length(center - glm::vec3(4, 0.2, 0)) > 0.9) {
				Material* material = nullptr;
				std::string name = "asdfagshasdfasdgafsasdfkadasdnadsfñjaweasdf";
				name[static_cast<int>(Math::random<float>(0.0f, 40.0f))] = 'a' + static_cast<int>(Math::random<float>(0, 100));

				if (chooseMat < 0.8) {
					glm::vec3 albedo = Math::random<glm::vec3>() * Math::random<glm::vec3>();
					material = scene.addMaterial<Lambertian>(name, albedo);
				} else if (chooseMat < 0.95) {
					glm::vec3 albedo = Math::random<glm::vec3>(0.5f, 1.0f);
					material = scene.addMaterial<Metal>(name, albedo, Math::random<float>(0.f, 0.5f));
				} else {
					material = scene.addMaterial<Dielectric>(name, glm::vec3(1.0f), 1.5f);
				}

				scene.add<Sphere>(center, 0.2, material);
			}
		}
	}

	Material* m1 = scene.addMaterial<Dielectric>("dielectric", glm::vec3(1.0f), 1.5f);
	scene.add<Sphere>(glm::vec3(0, 1, 0), 1.0f, m1);

	Material* m2 = scene.addMaterial<Lambertian>("lam", glm::vec3(0.4, 0.2, 0.1));
	scene.add<Sphere>(glm::vec3(-4, 1, 0), 1.0, m2);

	Material* m3 = scene.addMaterial<Metal>("metal", glm::vec3(0.8, 0.8, 0.8), 0.0f);
	scene.add<Sphere>(glm::vec3(4, 1, 0), 1.0, m3);

	scene.background = [](Ray r) {
				glm::vec3 u_dir = glm::normalize(r.direction);
				float y = 0.5*u_dir.y + 1;
				return (1-y) * glm::vec3(1) + y * glm::vec3(0.5, 0.7, 1.0);
		};

	return scene;
}

int main(int argc, char** argv) {
	std::printf("Hello raytraced world\n");

	if (argc != 2) {
		std::printf("Usage:\n raytrace [output]\n");
		return -1;
	}

	float a = 16.0f / 9.0f;

	Camera camera(glm::vec3(0, 0.5f, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 70, a);

//	Scene scene = randomScene();
	Scene scene;
	Material* left = scene.addMaterial<Dielectric>("l", glm::vec3(1.0f), 1.5f);
	Material* ground = scene.addMaterial<Lambertian>("g", glm::vec3(0.8, 0.8, 0.0));
	Material* center = scene.addMaterial<Lambertian>("c", glm::vec3(0.1, 0.2, 0.5));
	Material* right = scene.addMaterial<Metal>("r", glm::vec3(0.8, 0.6, 0.2));
	Material* em = scene.addMaterial<Emissive>("e", glm::vec3(1.3), 1);
	scene.add<Sphere>(glm::vec3(0, -100.5f, -1), 100, ground);
	// scene.add<Sphere>(glm::vec3(0, 0.f, -1), 0.5f, center);
	scene.add<Sphere>(glm::vec3(1, 0.f, -1), 0.5f, right);
	scene.add<Sphere>(glm::vec3(-1, 0.5f, -1), 0.5f, left);
	scene.add<Sphere>(glm::vec3(-1, 3.f, -1), 0.5f, em);
	// scene.add<Sphere>(glm::vec3(-1, 0, -1), -0.4f, left);
	scene.background = [](Ray r) {
			return glm::vec3(0.05f);
		};

//	BDPT renderer(400*a, 400);
	Renderer renderer(400*a, 400);
	renderer.samples = 50;

	Image im = renderer.render(camera, scene);

	std::string path = "output/" + std::string(argv[1]);
	FILE* image = std::fopen(path.c_str(), "wb");
	std::fprintf(image, "P6\n%i %i\n255\n", im.width, im.height);
	std::fwrite(im.pixels, im.pixelSize, im.width*im.height, image);
}
