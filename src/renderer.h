#pragma once

#include "image.h"
#include "Camera.h"
#include "scene.h"

class Renderer {
public:
	Renderer() {}
	Renderer(uint32_t width, uint32_t height)
		: imageWidth(width), imageHeight(height) {}

	Image render(const Camera& camera, const Scene& scene);

	uint32_t samples = 8;
	uint32_t imageWidth = 0, imageHeight = 0;
	uint32_t reflections = 50;
protected:
	virtual glm::vec3 rayColor(Ray ray, const Aggregate* agg, const std::vector<Emissive*>& lights, uint32_t depth) const;
	Pixel shadePixel(glm::vec2 uv, const Camera& camera, const Aggregate* agg, const std::vector<Emissive*>& lights) const;

	glm::vec3 gammaCorrectColor(glm::vec3 color) const;

	float invWidth = 0;
	float invHeight = 0;
	float invSamples = 0;
	std::function<glm::vec3 (Ray)> background;
};
