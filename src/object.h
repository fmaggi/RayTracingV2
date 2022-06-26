#pragma once

#include "ray.h"
#include "material.h"

#include <optional>

struct HitInfo {
	glm::vec3 p;
	glm::vec3 n;
	float t;
	Material* material;
};

class Hittable {
public:
	virtual ~Hittable() {}
	virtual std::optional<HitInfo> intersect(Ray ray, float tMin, float tMax) const = 0;
};

class Sphere : public Hittable {
public:
	Sphere(glm::vec3 position, float radius, Material* material)
		: m_material(material), m_position(position), m_radius(radius) {}

	std::optional<HitInfo> intersect(Ray ray, float tMin, float tMax) const override;

private:
	Material* m_material;
	glm::vec3 m_position;
	float m_radius;
};
