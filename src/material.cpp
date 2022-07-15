#include "material.h"

#include "glm/geometric.hpp"
#include "hit.h"
#include "utils/math.h"

static glm::vec3 reflect(glm::vec3 dir, glm::vec3 normal) {
	return dir - 2 * glm::dot(dir, normal) * normal;
}

static glm::vec3 refract(glm::vec3 dir, glm::vec3 normal, float n) {
	// sinT = n1/n2 * sinI
	// sinI = sqrt(1 - cos2I)
	// r = (n1/n2) * dir + (n1/n2 * cosI - sqrt(1 - sinT2)) * normal
	const float cosI = fmin(glm::dot(-dir, normal), 1.0f);
	const float sinT2 = n * n * (1 - cosI * cosI);
	const float cosT = sqrt(1 - sinT2);
	const glm::vec3 r = (n * dir) + ((n * cosI - cosT) * normal);
	return r;
}

static float shlickApprox(float cosI, float ir) {
	float r0 = (1-ir) / (1+ir);
	r0 *= r0;
	return r0 + (1-r0)*pow((1-cosI), 5);
}

std::optional<Ray> Lambertian::scatter(Ray ray, HitInfo hit) const {
	glm::vec3 dir = hit.n + Random::InUnitSphere();
	dir = Math::nearZero(dir) ? hit.n : dir;
	return Ray(hit.p, dir);
}

std::optional<Ray> Metal::scatter(Ray ray, HitInfo hit) const {
	glm::vec3 dir = reflect(glm::normalize(ray.direction), hit.n) + fuzz * Random::InUnitSphere();
	return Ray(hit.p, dir);
}

std::optional<Ray> Dielectric::scatter(Ray ray, HitInfo hit) const {
	const float refraction = hit.frontFace ? (1.0f/ir) : ir;

	const glm::vec3 normDir = glm::normalize(ray.direction);
	const float cosI = fmin(glm::dot(-normDir, hit.n), 1.0f);
	const float sinI = sqrt(1 - cosI*cosI);
	glm::vec3 dir;
	if (refraction * sinI > 1.0f || shlickApprox(cosI, refraction) > Random::Float()) {
		dir = reflect(normDir, hit.n);
	} else {
		dir = refract(normDir, hit.n, refraction);
	}
	return Ray(hit.p, dir);
}

glm::vec3 Emissive::emit() const {
	return albedo * intensity;
}
