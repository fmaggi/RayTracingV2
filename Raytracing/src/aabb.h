#pragma once

#include "ray.h"

#include "glm/glm.hpp"

struct AABB {
	static AABB Union(AABB a, AABB b);

	AABB() {}
	AABB(glm::vec3 min, glm::vec3 max)
		: min(min), max(max) {}

	bool intersect(Ray ray, float tMin, float tMax) const;

	glm::vec3 min = {0, 0, 0};
	glm::vec3 max = {0, 0, 0};
};
