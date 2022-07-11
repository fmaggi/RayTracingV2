#include "bdpt.h"
#include "utils/math.h"

glm::vec3 BDPT::rayColor(Ray ray, const Aggregate* agg, const std::vector<Light*>& lights, uint32_t depth) const {
	auto cameraPath = generateCameraSubpath(agg, ray, depth);

	// light = pickRandomLight(lights);
	auto lightPath = generateLightSubpath(agg, lights[0], depth);
	//
	return connectPaths(cameraPath, lightPath);
}

glm::vec3 BDPT::connectPaths(const std::vector<Vertex>& cameraPath, const std::vector<Vertex>& lightPath) const {
	glm::vec3 color{};
	for (const auto v : cameraPath) {
		color += v.albedo;
	}
	return color / (float)cameraPath.size();
}

std::vector<BDPT::Vertex> BDPT::generateCameraSubpath(const Aggregate* agg, Ray startingRay, uint32_t depth) const {
	std::vector<Vertex> vertices;
	{
		Vertex v0;
		v0.albedo = background(startingRay);
		v0.type = VertexType::Camera;
		vertices.push_back(v0);
	}

	// float tMin = 0.000001;
	// float tMax = Math::infinity;

	// Ray r = startingRay;
	// glm::vec3 color(1.0f);
	// for (uint32_t i = 0; i < depth; ++i) {
	// 	auto hit = agg->traverse(r, tMin, tMax);
	// 	if (!hit) {
	// 		break;
	// 	}

	// 	r = hit->material->scatter(r, *hit);
	// 	color *= hit->material->albedo;

	// 	Vertex v;
	// 	v.albedo = color;
	// 	v.type = VertexType::Surface;
	// 	vertices.push_back(v);
	// }

	return vertices;
}

std::vector<BDPT::Vertex> BDPT::generateLightSubpath(const Aggregate* agg, const Light* light, uint32_t depth) const {
	std::vector<Vertex> vertices;
	{
		Vertex v0;
		v0.type = VertexType::Light;
		vertices.push_back(v0);
	}

	//float tMin = 0.000001;
	//float tMax = Math::infinity;

	//Ray r(light->position(), Math::random<glm::vec3>()); // Need to fix random direction
	//glm::vec3 color = light->color;
	//for (uint32_t i = 0; i < depth; ++i) {
	//	auto hit = agg->traverse(r, tMin, tMax);
	//	if (!hit) {
	//		break;
	//	}

	//	Vertex v;
	//	v.albedo = color;
	//	v.type = VertexType::Surface;
	//	vertices.push_back(v);

	//	r = hit->material->scatter(r, *hit);
	//	color *= hit->material->albedo;
	//}

	return vertices;
}
