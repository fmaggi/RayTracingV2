#include "material.h"

#include "utils/math.h"

std::optional<Ray> Lambertian::scatter(glm::vec3 hitPoint, glm::vec3 hitNormal) const {
	glm::vec3 dir = hitNormal + Math::randomInUnitSphere();
	return Math::nearZero(dir) ? std::optional<Ray>{} : Ray(hitPoint, dir);
}

std::optional<Ray> Metal::scatter(glm::vec3 hitPoint, glm::vec3 hitNormal) const {
	return {};
}
