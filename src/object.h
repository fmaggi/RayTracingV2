#pragma once

#include "hittable.h"

class Sphere : public Hittable {
public:
	Sphere(glm::vec3 position, float radius, Material* material)
		: m_material(material), m_position(position), m_radius(radius) {}

	std::optional<HitInfo> intersect(Ray ray, float tMin, float tMax) const override;
	AABB boundingBox() const override;

private:
	Material* m_material;
	glm::vec3 m_position;
	float m_radius;
};
