#include "renderer.h"

#include "accel/bvh.h"

#include "glm/geometric.hpp"
#include "utils/math.h"
#include <ctime>

Renderer::~Renderer() {
	wait();
}

glm::vec3 Renderer::rayColor(Ray ray, const Aggregate* agg, const std::vector<Surface*>& lights, uint32_t depth) const {
	float tMin = 0.000001;
	float tMax = Math::infinity;

	glm::vec3 radiance{};
	glm::vec3 throughput(1.0f);

	for (uint32_t i = 0; i < depth; ++i) {
		auto hit = agg->traverse(ray, tMin, tMax);
		if (!hit) {
			return radiance + background(ray) * throughput;
		}

		radiance += hit->material->emit() * throughput;

		auto s = hit->material->scatter(ray, *hit);
		if (!s) {
			return radiance * throughput;
		}

		radiance += sampleLight(*hit, agg, lights) * throughput;
		throughput *= hit->material->albedo;

		ray = *s;
	}

	return radiance * throughput;
}

glm::vec3 Renderer::sampleLight(HitInfo hit, const Aggregate* agg, const std::vector<Surface*>& lights) const {
	if (lights.size() == 0) {
		return glm::vec3(0.0f);
	}

	int index = lights.size() > 1 ? Random::Int(0, lights.size()-1) : 0;
	Surface* light = lights[index];

	glm::vec3 d = light->position - hit.p;
	glm::vec3 normD = glm::normalize(d);
	float r2 = glm::dot(d, d);

	Ray shadowRay(hit.p, d);
	float tMax = shadowRay.tAt(light->limit(normD));

	float v = !agg->traverse(shadowRay, Math::almostZero, tMax - 0.001);

	float p = glm::dot(normD, hit.n);
	p = fmax(0.0f, p);

	const Emissive* e = static_cast<const Emissive*>(light->material);
	glm::vec3 color = p * e->albedo * e->intensity * Math::invPi * 0.25f / r2;

	return v * color * hit.material->attenuation(hit.hitDir, glm::vec3(0.0f)) / (float)lights.size();
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

Pixel Renderer::shadePixel(glm::vec2 uv, const Camera& camera, const Aggregate* agg, const std::vector<Surface*>& lights) const {
	glm::vec3 color{};
	for (uint32_t s = 0; s < samples; s++) {
		glm::vec2 suv;
		suv.x = (uv.x + Random::Float()) * invWidth;
		suv.y = (uv.y + Random::Float()) * invHeight;
		color += rayColor(camera.castRay(suv), agg, lights, reflections);
	}
	color = gammaCorrectColor(color);
	Pixel pixel;
	pixel.r = (uint8_t)(color.r * 255);
	pixel.g = (uint8_t)(color.g * 255);
	pixel.b = (uint8_t)(color.b * 255);
	return pixel;
}

#include <cstdio>
#include <thread>

const Image& Renderer::render(const Camera &camera, const Scene &scene) {
	if (rowsToGo() > 0) {
		std::printf("Already at work!\n");
		return image;
	}

	if (imageWidth == 0 || imageHeight == 0) {
		std::printf("Invalid image size (%u, %u)\n", imageWidth, imageHeight);
		return image;
	}

	image.resize(imageWidth, imageHeight);

	invWidth = 1.0f / imageWidth;
	invHeight = 1.0f / imageHeight;
	invSamples = 1.0f / samples;
	background = scene.background;

	delete agg;
	agg = new BVHtree(scene.getHittables());

	if (nThreads == 0) {
		nThreads = std::jthread::hardware_concurrency()-1;
	}
	std::printf("Rendering in %u threads\n", nThreads);

	progress.clear();
	for (uint32_t i = 0; i < nThreads; ++i) {
		progress.push_back(0);
	}

	uint32_t rowsPerThread = imageHeight / nThreads;
	total = image.height;

	for (uint32_t i = 0; i < nThreads; ++i) {
		auto start = i * rowsPerThread;
		auto end = i < nThreads-1 ? (i+1)*rowsPerThread : image.height;
		std::thread t([this, &c=camera, &s=scene](uint32_t start, uint32_t end, uint32_t* progress) {
			for (uint32_t y = start; y < end; ++y) {
				*progress += 1;
				for (uint32_t x = 0; x < this->image.width; ++x) {
					glm::vec2 uv = { x, y };
					this->image.at(x, this->image.height-y-1) = this->shadePixel(uv, c, this->agg, s.getLights());
				}
			}
		}, start, end, progress.data() + i);
		t.detach();
	}

	return image;
}

uint32_t Renderer::rowsToGo() const {
	uint32_t p = 0;
	for (uint32_t pi : progress) {
		p += pi;
	}
	return total - p;
}

void Renderer::wait() {
	while (rowsToGo() > 1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	delete agg;
	agg = nullptr;
}
