#pragma once

#include "glm/geometric.hpp"
#include "glm/glm.hpp"

struct Ray {
	Ray(glm::vec3 origin, glm::vec3 direction)
		: origin(origin), direction(glm::normalize(direction)) {}


	glm::vec3 at(float t) {
		return origin + t * direction;
	}

	const glm::vec3 origin;
	const glm::vec3 direction;
};
