#include "aabb.h"

AABB AABB::Union(AABB a, AABB b) {
	glm::vec3 min;
	glm::vec3 max;

	min.x = a.min.x < b.min.x ? a.min.x : b.min.x;
	min.y = a.min.y < b.min.y ? a.min.y : b.min.y;
	min.z = a.min.z < b.min.z ? a.min.z : b.min.z;

	max.x = a.max.x > b.max.x ? a.max.x : b.max.x;
	max.y = a.max.y > b.max.y ? a.max.y : b.max.y;
	max.z = a.max.z > b.max.z ? a.max.z : b.max.z;

	return AABB(min, max);
}

bool AABB::intersect(Ray ray, float tMin, float tMax) const {
	{
		float invDirx = 1.0f / ray.direction.x;
		float t0x = (min.x - ray.origin.x) * invDirx;
		float t1x = (max.x - ray.origin.x) * invDirx;

		if (invDirx < 0.0f) {
			float t = t0x;
			t0x = t1x;
			t1x = t;
		}

		tMin = t0x > tMin ? t0x : tMin;
		tMax = t1x < tMax ? t1x: tMax;

		if (tMin > tMax) {
			return false;
		}
	}

	{
		float invDiry = 1.0f / ray.direction.y;
		float t0y = (min.y - ray.origin.y) * invDiry;
		float t1y = (max.y - ray.origin.y) * invDiry;

		if (invDiry < 0.0f) {
			float t = t0y;
			t0y = t1y;
			t1y = t;
		}

		tMin = t0y > tMin ? t0y : tMin;
		tMax = t1y < tMax ? t1y: tMax;

		if (tMin > tMax) {
			return false;
		}
	}

	{
		float invDirz = 1.0f / ray.direction.z;
		float t0z = (min.z - ray.origin.z) * invDirz;
		float t1z = (max.z - ray.origin.z) * invDirz;

		if (invDirz < 0.0f) {
			float t = t0z;
			t0z = t1z;
			t1z = t;
		}

		tMin = t0z > tMin ? t0z : tMin;
		tMax = t1z < tMax ? t1z: tMax;

		if (tMin > tMax) {
			return false;
		}
	}

	return true;
}
