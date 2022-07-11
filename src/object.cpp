#include <object.h>

#include "glm/glm.hpp"
#include "ray.h"

std::optional<HitInfo> Sphere::intersect(Ray ray, float tMin, float tMax) const {
	glm::vec3 oc = ray.origin - m_position;
	float a = glm::dot(ray.direction, ray.direction);
	float half_b = glm::dot(oc, ray.direction);
	float c = glm::dot(oc, oc) - m_radius*m_radius;
	float discriminant = half_b*half_b - a*c;

	if (discriminant < 0) {
		return {};
	}

	float sd = sqrt(discriminant) / a;
	float half_b_over_a = half_b / a;

	float t = -half_b_over_a - sd;
	if (t < tMin || t > tMax) {
		t = -half_b_over_a + sd;
		if (t < tMin || t > tMax) {
			return {};
		}
		 //this gives a weird artiffact that I cannot yet fix
	}

	glm::vec3 p = ray.at(t);
	glm::vec3 n = (p - m_position) / m_radius;
	HitInfo info(p, t, n, ray.direction, m_material);
	return info;
}

AABB Sphere::boundingBox() const {
	glm::vec3 min = m_position - glm::vec3(fabs(m_radius));
	glm::vec3 max = m_position + glm::vec3(fabs(m_radius));
	return AABB(min, max);
}
