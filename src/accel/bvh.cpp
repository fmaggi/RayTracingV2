#include "bvh.h"

#include <algorithm>

auto maxExtent(AABB a) {
	glm::vec3 v = a.max - a.min;
	if (v.x > v.y && v.x > v.z) {
		return &glm::vec3::x;
	}
	if (v.y > v.z) {
		return &glm::vec3::y;
	}
	return &glm::vec3::z;
}

int BVHtree::recursiveBuild(std::vector<HittableInfo>& info, uint32_t start, uint32_t end, const std::vector<Hittable*>& hittables) {
	BVHnode node;
	if (end - start == 1) {
		int hittableIndex = info[start].index;
		node.hittable = hittables[hittableIndex];
		node.right = 0;
		node.left = 0;
		node.aabb = node.hittable->boundingBox();

		nodes.push_back(node);
		return nodes.size() - 1;
	}

	AABB boundingBox;
	for(uint32_t i = start; i < end; ++i) {
		boundingBox = AABB::Union(boundingBox, info[i].aabb);
	}

	auto dir = maxExtent(boundingBox);

	auto startIt = info.begin() + start;
	auto endIt = info.begin() + end;
	std::sort(startIt, endIt, [dir](HittableInfo a, HittableInfo b){
				return a.centroid.*dir < b.centroid.*dir;
			});

	uint32_t mid = (end + start) / 2;
	node.left = recursiveBuild(info, start, mid, hittables);
	node.right = recursiveBuild(info, mid, end, hittables);
	node.hittable = nullptr;
	node.aabb = boundingBox;

	nodes.push_back(node);
	return nodes.size() - 1;
}

BVHtree::BVHtree(const std::vector<Hittable*>& hittables) {
	std::vector<HittableInfo> hittableInfo;
	for (uint32_t i = 0; i < hittables.size(); ++i) {
		HittableInfo info( hittables[i]->boundingBox(), i );
		hittableInfo.push_back(info);
	}

	rootIndex = recursiveBuild(hittableInfo, 0, hittableInfo.size(), hittables);
}

std::optional<HitInfo> BVHtree::traverse(Ray ray, float tMin, float tMax) const {
	int currentIndex = rootIndex;
	int visitOffset = 0;
	int nodesToVisit[64];

	std::optional<HitInfo> hitInfo{};

	while (true) {
		const BVHnode* node = nodes.data() + currentIndex;
		if (node->aabb.intersect(ray, tMin, tMax)) {
			if (node->hittable) {
				auto hit = node->hittable->intersect(ray, tMin, tMax);
				if (hit) {
					hitInfo = hit;
					tMax = hit->t;
				}
			} else {
				nodesToVisit[visitOffset++] = node->left;
				currentIndex = node->right;
				continue;
			}
		}

		if (visitOffset == 0) {
			break;
		}
		currentIndex = nodesToVisit[--visitOffset];
	}

	return hitInfo;
}

static void rdump(BVHnode node, const std::vector<BVHnode>& nodes, int offset) {
	if (node.hittable) {
		std::printf("%*s %p\n", offset, "", node.hittable);
		return;
	}
	std::printf("%*s left:\n", offset, "");
	rdump(nodes[node.left], nodes, offset+1);
	std::printf("%*s right\n", offset, "");
	rdump(nodes[node.right], nodes, offset+1);
}

void BVHtree::dump() {
	std::printf("-------Tree dump-------\n");
	rdump(nodes[rootIndex], nodes, 0);
}
