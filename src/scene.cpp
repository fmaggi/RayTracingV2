#include "scene.h"

#include "utils/math.h"

Scene::Scene() {
	background = [](Ray ray) { return glm::vec3(0.3); };
}

Scene::~Scene() {
	for (const auto& hittable : hittables) {
		delete hittable;
	}
	for (const auto& it : materials) {
		delete it.second;
	}
}

std::optional<HitInfo> Scene::intersect(Ray ray, float tMin, float tMax) const {
	std::optional<HitInfo> ret{};
	bool hit = false;
	for (const auto& hittable : hittables) {
		auto info = hittable->intersect(ray, tMin, tMax);
		if (info) {
			tMax = info->t;
			ret = info;
			hit = true;
		}
	}
	if (!hit){
		return {};
	}

	return ret;
}
