#include "light.h"

#include "glm/glm.hpp"
#include "utils/math.h"

glm::vec3 PointLight::lightColor(glm::vec3 hitPoint, glm::vec3 hitNormal) const {
	glm::vec3 lightDir = hitPoint - pos;
	float r2 = glm::dot(lightDir, lightDir);

	float invDist = 1.0f / sqrtf(r2);
	lightDir *= invDist;

	float p = glm::dot(glm::normalize(-lightDir), hitNormal);
	p = fmax(0, p);

	return p * intensity * color / (4 * Math::pi * r2);
}

VisibilityTester PointLight::visibilityTester(glm::vec3 hitPoint) const {
	return {pos, hitPoint - pos};
}
