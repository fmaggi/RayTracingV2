#pragma once

#include "hittable.h"

class Sphere : public Surface {
public:
	Sphere(glm::vec3 position, float radius, Material* material)
		: Surface(position, material), radius(radius) {}

	std::optional<HitInfo> intersect(Ray ray, float tMin, float tMax) const override;
	AABB boundingBox() const override;

	glm::vec3 limit(glm::vec3 direction) const override;

private:
	float radius;
};
