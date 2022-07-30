#pragma once

#include "ray.h"
#include "material.h"
#include "aabb.h"

#include <optional>

class Hittable {
public:
	Hittable(glm::vec3 p)
		: position(p) {}
	virtual ~Hittable() {}
	virtual std::optional<HitInfo> intersect(Ray ray, float tMin, float tMax) const = 0;
	virtual AABB boundingBox() const = 0;
	const glm::vec3 position;
};

class Surface : public Hittable {
public:
	Surface(glm::vec3 position, Material* m)
		: Hittable(position), material(m) {}
	virtual ~Surface() {}
	virtual glm::vec3 limit(glm::vec3 direction) const = 0;

	const Material* material = nullptr;
};

class Aggregate {
public:
	virtual ~Aggregate() {}
	virtual std::optional<HitInfo> traverse(Ray ray, float tMin, float tMax) const = 0;
};
