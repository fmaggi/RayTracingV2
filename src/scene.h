#pragma once

#include "object.h"
#include <vector>
#include <unordered_map>
#include <concepts>
#include <functional>
#include <string>

template<typename T>
concept HittableBaseClass = std::derived_from<T, Hittable>;

template<typename T>
concept MaterialBaseClass = std::derived_from<T, Material>;

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

	template<MaterialBaseClass T, typename... Args>
	Material* addMaterial(std::string name, Args&&... args) {
		Material* m = new T(std::forward<Args>(args)...);
		m_materials[name] = m;
		return m;
	}

	Material* getMaterial(std::string name) {
		auto it = m_materials.find(name);
		if (it == m_materials.end()) {
			return nullptr;
		}
		return it->second;
	}

	glm::vec3 backgroundColor(Ray ray) const {
		return m_background(ray);
	}

	std::optional<HitInfo> intersect(Ray ray, float tMin, float tMax) const;
private:
	std::vector<Hittable*> m_hittables;
	std::unordered_map<std::string, Material*> m_materials;
	std::function<glm::vec3 (Ray)> m_background;
};
