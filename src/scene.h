#pragma once

#include "object.h"
#include <vector>
#include <concepts>
#include <functional>

template<typename T>
concept HittableBaseClass = std::derived_from<T, Hittable>;

class Scene {
public:
	Scene();
	~Scene();

	void setBackgroundColor(std::function<glm::vec3(Ray)> func) {
		m_background = func;
	}

	template<HittableBaseClass T, typename... Args>
	void add(Args&&... args) {
		m_hittables.push_back(new T(std::forward<Args>(args)...));
	}

	glm::vec3 backgroundColor(Ray ray) const {
		return m_background(ray);
	}

	std::optional<HitInfo> intersect(Ray ray) const;
private:
	std::vector<Hittable*> m_hittables;
	std::function<glm::vec3 (Ray)> m_background;
};
