#pragma once

#include "hit.h"
#include "image.h"
#include "camera.h"
#include "scene.h"

class Renderer {
public:
	~Renderer();

	const Image& render(const Camera& camera, const Scene& scene);
	const Image& getImage() { return image; }

	uint32_t rowsToGo() const;
	void wait();
	bool working() const { return rowsToGo() > 0; }

	uint32_t samples = 8;
	uint32_t imageWidth = 0, imageHeight = 0;
	uint32_t reflections = 50;

	uint32_t nThreads = 0;
protected:
	virtual glm::vec3 rayColor(Ray ray, const Aggregate* agg, const std::vector<Surface*>& lights, uint32_t depth) const;
	Pixel shadePixel(glm::vec2 uv, const Camera& camera, const Aggregate* agg, const std::vector<Surface*>& lights) const;
	glm::vec3 sampleLight(HitInfo hit, const Aggregate* agg, const std::vector<Surface*>& lights) const;

	glm::vec3 gammaCorrectColor(glm::vec3 color) const;

	float invWidth = 0;
	float invHeight = 0;
	float invSamples = 0;
	std::function<glm::vec3 (Ray)> background;

	Image image;

	std::vector<uint32_t> progress;
	uint32_t total = 0;
	Aggregate* agg = nullptr;
};
