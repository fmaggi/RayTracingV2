#pragma once

#include "hittable.h"
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

class Scene : public Aggregate {
public:
	Scene();
	~Scene();

	template<HittableBaseClass T, typename... Args>
	Hittable* add(Args&&... args) {
		Hittable* h = new T(std::forward<Args>(args)...);
		hittables.push_back(h);
		return h;
	}

	template<MaterialBaseClass T, typename... Args>
	Material* addMaterial(std::string name, Args&&... args) {
		Material* m = new T(std::forward<Args>(args)...);
		materials[name] = m;
		return m;
	}

	Material* getMaterial(std::string name) {
		auto it = materials.find(name);
		if (it == materials.end()) {
			return nullptr;
		}
		return it->second;
	}

	std::optional<HitInfo> traverse(Ray ray, float tMin, float tMax) const override;

	const std::vector<Hittable*>& getHittables() const { return hittables; }

	std::function<glm::vec3 (Ray)> background;
private:
	std::vector<Hittable*> hittables;
	std::unordered_map<std::string, Material*> materials;
};
