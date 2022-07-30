#pragma once

#include "glm/geometric.hpp"
#include "glm/glm.hpp"

struct Ray {
	Ray(glm::vec3 origin, glm::vec3 direction)
		: origin(origin), direction(direction) {}


	glm::vec3 at(float t) {
		return origin + t * direction;
	}

	float tAt(glm::vec3 p) {
		return (p.x - origin.x) / direction.x;
	}

	glm::vec3 origin;
	glm::vec3 direction;
};
