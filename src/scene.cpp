#include "scene.h"

Scene::Scene() {
	m_background = [](Ray ray) { return glm::vec3(0.3); };
}
Scene::~Scene() {
	for (const auto& hittable : m_hittables) {
		delete hittable;
	}
}

std::optional<HitInfo> Scene::intersect(Ray ray) {
	float tMin = 0;
	float tMax = 10000;
	std::optional<HitInfo> ret{};
	bool hit = false;
	for (const auto& hittable : m_hittables) {
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
