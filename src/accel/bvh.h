#pragma once

#include "hittable.h"

#include <vector>

struct BVHnode {
	AABB aabb;
	Hittable* hittable;
	int left;
	int right;
};

class BVHtree {
public:
	BVHtree() {}
	BVHtree(const std::vector<Hittable*>& hittables);

	std::optional<HitInfo> traverse(Ray ray, float tMin, float tMax) const;
private:
	struct HittableInfo {
		HittableInfo() {}
		HittableInfo(AABB aabb, uint32_t index)
			: aabb(aabb), centroid((aabb.min + aabb.max)*0.5f), index(index) {}
		AABB aabb;
		glm::vec3 centroid;
		uint32_t index;
	};

	int recursiveBuild(std::vector<HittableInfo>& info, uint32_t start, uint32_t end, const std::vector<Hittable*>& hittables);

	int rootIndex;
	std::vector<BVHnode> nodes;
	void dump();
};
