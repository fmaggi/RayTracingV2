#include <object.h>

#include "glm/glm.hpp"
#include "ray.h"
#include <cstdio>
std::optional<HitInfo> Sphere::intersect(Ray ray, float tMin, float tMax) const {
	glm::vec3 oc = ray.origin - m_position;
	float a = glm::dot(ray.direction, ray.direction);
	float half_b = glm::dot(oc, ray.direction);
	float c = glm::dot(oc, oc) - m_radius*m_radius;

	float discriminant = half_b*half_b - a*c;
	if (discriminant < 0) {
		return {};
	}

	float t = (-half_b - sqrt(discriminant)) / a;
	if (t < tMin || t > tMax) {
		return {};
	}

	HitInfo info;
	info.t = t;
	info.p = ray.at(t);
	info.setNormal(ray.direction, (info.p - m_position) / m_radius);
	info.material = m_material;
	return info;
}
