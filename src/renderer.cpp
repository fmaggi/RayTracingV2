#include "renderer.h"

#include "accel/bvh.h"

#include "glm/geometric.hpp"
#include "utils/math.h"
#include "light.h"

// need to fix this
static bool visible(const Aggregate* agg, Ray lightRay, glm::vec3 point) {
	float tMax = lightRay.tAt(point);
	auto hit = agg->traverse(lightRay, 0.000001, tMax);
	if (!hit) {
		return true;
	}

//	if (hit->material->type == Material::MaterialType::Refractive) {
//		Ray bounced(hit->p, point - hit->p);
//		return visible(agg, bounced, point);
//	}

//	if (hit->material->type == Material::MaterialType::Reflective) {
//		return glm::dot(hit->n, point - hit->p) > 0;
//	}

	return false;
}

glm::vec3 Renderer::rayColor(Ray ray, const Aggregate* agg, const std::vector<Light*>& lights, uint32_t depth) const {
	float tMin = 0.000001;
	float tMax = Math::infinity;

	auto hit = agg->traverse(ray, tMin, tMax);
	if (!hit) {
		return background(ray);
	}

	glm::vec3 lightColor{};
	for (const auto& light : lights) {
		VisibilityTester tester = light->visibilityTester(hit->p);
		Ray lightRay(hit->p, -tester.lightDir);
		float v = visible(agg, lightRay, tester.p);
		lightColor += v * light->lightColor(hit->p, hit->n) * hit->material->attenuation(-ray.direction, -tester.lightDir);
	}

	if (depth > 0) {
		Ray scattered = hit->material->scatter(ray, *hit);
		lightColor += hit->material->albedo * rayColor(scattered, agg, lights, --depth);
	}

	return lightColor;
}

glm::vec3 Renderer::gammaCorrectColor(glm::vec3 color) const {
	float rc = sqrt(color.r * invSamples);
	float gc = sqrt(color.g * invSamples);
	float bc = sqrt(color.b * invSamples);

	float r = glm::clamp(rc, 0.f, 0.99f);
	float g = glm::clamp(gc, 0.f, 0.99f);
	float b = glm::clamp(bc, 0.f, 0.99f);

	return glm::vec3{r, g, b};
}

Pixel Renderer::shadePixel(glm::vec2 uv, const Camera& camera, const Aggregate* agg, const std::vector<Light*>& lights) const {
	glm::vec3 color{};
	for (uint32_t s = 0; s < samples; s++) {
		glm::vec2 suv;
		suv.x = (uv.x + Math::random<float>()) * invWidth;
		suv.y = (uv.y + Math::random<float>()) * invHeight;
		color += rayColor(camera.castRay(suv), agg, lights, reflections);
	}
	color = gammaCorrectColor(color);
	Pixel pixel;
	pixel.r = (uint8_t)(color.r * 255);
	pixel.g = (uint8_t)(color.g * 255);
	pixel.b = (uint8_t)(color.b * 255);
	return pixel;
}

#include <chrono>
#include <cstdio>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::seconds;

Image Renderer::render(const Camera &camera, const Scene &scene) {
	Image image(imageWidth, imageHeight);

	invWidth = 1.0f / imageWidth;
	invHeight = 1.0f / imageHeight;
	invSamples = 1.0f / samples;
	background = scene.background;

	BVHtree tree(scene.getHittables());

	// auto start = high_resolution_clock::now();

	for (uint32_t y = 0; y < imageHeight; y++) {
		std::fprintf(stderr, "\rLine: %u", y);
		for (uint32_t x = 0; x < imageWidth; x++) {
			glm::vec2 uv = { x, y };
			image.at(x, imageHeight-y-1) = shadePixel(uv, camera, &tree, scene.getLights());
		}
	}
	std::printf("\n");

	// auto stop = high_resolution_clock::now();

	// auto ms_int = duration_cast<seconds>(stop-start);

	// std::printf("%li\n", ms_int.count());

	return image;
}


