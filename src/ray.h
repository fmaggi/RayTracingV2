#pragma once

#include "glm/vec3.hpp"

struct Ray {

	Ray() {}
	Ray(glm::vec3 origin, glm::vec3 direction)
		: origin(origin), direction(direction) {}


	glm::vec3 at(float t) {
		return origin + t * direction;
	}

	glm::vec3 origin;
	glm::vec3 direction;
};

