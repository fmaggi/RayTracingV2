#pragma once

#include "renderer.h"

class BDPT : public Renderer {
public:
	BDPT(uint32_t width, uint32_t height)
		: Renderer(width, height) {}
protected:
	glm::vec3 rayColor(Ray ray, const Aggregate* agg, const std::vector<Light*>& lights, uint32_t depth) const;
private:
	enum struct VertexType { None, Camera, Surface, Light };

	struct Vertex {
		HitInfo hit;
		glm::vec3 albedo{1, 1, 1};
		VertexType type{VertexType::None};
	};

	std::vector<Vertex> generateCameraSubpath(const Aggregate* agg, Ray startingRay, uint32_t depth) const;
	std::vector<Vertex> generateLightSubpath(const Aggregate* agg, const Light* light, uint32_t depth) const;
	glm::vec3 connectPaths(const Aggregate* agg, const std::vector<Vertex>& cameraPath, int t, const std::vector<Vertex>& lightPath, int s) const;
	float G(Vertex cv, Vertex cl) const;

	// TODO:
	// generatePath()
	// connect(cameraPath, lightPath)
	// anything else
};
