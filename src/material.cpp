#include "material.h"

#include "glm/geometric.hpp"
#include "utils/math.h"

static glm::vec3 reflect(glm::vec3 dir, glm::vec3 normal) {
	return dir - 2 * glm::dot(dir, normal) * normal;
}

std::optional<Ray> Lambertian::scatter(Ray ray, glm::vec3 hitPoint, glm::vec3 hitNormal) const {
	glm::vec3 dir = hitNormal + Math::randomInUnitSphere();
	dir = Math::nearZero(dir) ? hitNormal : dir;
	return Ray(hitPoint, dir);
}

std::optional<Ray> Metal::scatter(Ray ray, glm::vec3 hitPoint, glm::vec3 hitNormal) const {
	glm::vec3 dir = reflect(glm::normalize(ray.direction), hitNormal) + fuzz * Math::randomInUnitSphere();
	return Ray(hitPoint, dir);
}
