#include "bdpt.h"
#include "utils/math.h"

glm::vec3 BDPT::rayColor(Ray ray, const Aggregate* agg, const std::vector<Light*>& lights, uint32_t depth) const {
	auto cameraPath = generateCameraSubpath(agg, ray, depth);
	if (cameraPath.size() == 1) {
		return background(ray);
	}

	// light = pickRandomLight(lights);
	std::vector<Vertex> lightPath;
	if (lights.size() > 0) {
		lightPath = generateLightSubpath(agg, lights[0], depth);
	}
	//
	glm::vec3 color{};
	for (int t = 1; t < (int)cameraPath.size(); ++t) {
		for (int s = 0; s < (int)lightPath.size(); ++s) {
			color += connectPaths(agg, cameraPath, t, lightPath, s);
		}
	}
	return color;
}

static bool isBlack(glm::vec3 c) {
	return c.r < 0.01 && c.g < 0.01 && c.b < 0.01;
}

float BDPT::G(Vertex cv, Vertex cl) const {
	return 0.0f;
}

glm::vec3 BDPT::connectPaths(const Aggregate* agg, const std::vector<Vertex>& cameraPath, int t, const std::vector<Vertex>& lightPath, int s) const {
	if (s == 0) {
		return cameraPath[t].albedo;
	}
	Vertex cv = cameraPath[t];
	Vertex cl = lightPath[s];
	glm::vec3 dir = cv.hit.p - cl.hit.p;
	glm::vec3 color = cv.albedo * cv.hit.material->attenuation(cv.hit.hitDir, dir) * cl.hit.material->attenuation(cl.hit.hitDir, -dir) * cl.albedo;
	if (isBlack(color)) {
		return color;
	}

	glm::vec3 d = cv.hit.p - cl.hit.p;
	Ray r(cl.hit.p, d);
	float tMin = 0.00001;
	float tMax = r.tAt(cv.hit.p) + 1;
	for (uint32_t i = 0; i < reflections; ++i) {
		auto hit = agg->traverse(r, tMin, tMax);
		if (!hit) {
			return color;
		}

		d = cv.hit.p - hit->p;
		r = Ray(hit->p, d);

		color *= hit->material->absortion(r, *hit);
	}


	return color;
}

std::vector<BDPT::Vertex> BDPT::generateCameraSubpath(const Aggregate* agg, Ray startingRay, uint32_t depth) const {
	std::vector<Vertex> vertices;
	{
		Vertex v0;
		v0.albedo = background(startingRay);
		v0.type = VertexType::Camera;
		vertices.push_back(v0);
	}

	float tMin = 0.000001;
	float tMax = Math::infinity;

	Ray r = startingRay;
	glm::vec3 color = background(startingRay);
	for (uint32_t i = 0; i < depth; ++i) {
		auto hit = agg->traverse(r, tMin, tMax);
		if (!hit) {
			break;
		}

		r = hit->material->scatter(r, *hit);
		color *= hit->material->albedo;

		Vertex v;
		v.albedo = color;
		v.type = VertexType::Surface;
		v.hit = *hit;
		vertices.push_back(v);
	}

	return vertices;
}

std::vector<BDPT::Vertex> BDPT::generateLightSubpath(const Aggregate* agg, const Light* light, uint32_t depth) const {
	std::vector<Vertex> vertices;
	{
		Vertex v0;
		v0.type = VertexType::Light;
		vertices.push_back(v0);
	}

	float tMin = 0.000001;
	float tMax = Math::infinity;

	Ray r(light->position(), Math::randomInUnitSphere()); // Need to fix random direction
	glm::vec3 color = light->color;
	for (uint32_t i = 0; i < depth; ++i) {
		auto hit = agg->traverse(r, tMin, tMax);
		if (!hit) {
			break;
		}

		r = hit->material->scatter(r, *hit);
		color *= hit->material->albedo;

		Vertex v;
		v.albedo = color;
		v.type = VertexType::Surface;
		v.hit = *hit;
		vertices.push_back(v);
	}

	return vertices;
}
