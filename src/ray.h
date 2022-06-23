#pragma once

#include "glm/glm.hpp"

struct Ray {
	Ray(glm::vec3 origin, glm::vec3 direction)
		: origin(origin), direction(direction) {}


	glm::vec3 at(float t) {
		return origin + t * direction;
	}

	const glm::vec3 origin;
	const glm::vec3 direction;
};

inline glm::vec3 normalDirection(Ray ray, glm::vec3 outwardNormal) {
	bool frontFace = glm::dot(ray.direction, outwardNormal) < 0;
	return frontFace ? outwardNormal : -outwardNormal;
}
