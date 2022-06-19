#pragma once

#include "ray.h"

#include <optional>

struct HitInfo {
	glm::vec3 p;
	glm::vec3 n;
	float t;
};

class Hittable {
public:
	virtual ~Hittable() {}
	virtual std::optional<HitInfo> intersect(Ray ray, float tMin, float tMax) const = 0;
};

class Sphere : public Hittable {
public:
	Sphere(glm::vec3 position, float radius)
		: m_position(position), m_radius(radius) {}

	std::optional<HitInfo> intersect(Ray ray, float tMin, float tMax) const override;

private:
	glm::vec3 m_position;
	float m_radius;
};
