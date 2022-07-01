#pragma once

#include "ray.h"
#include "material.h"
#include "aabb.h"

#include <optional>

class Hittable {
public:
	virtual ~Hittable() {}
	virtual std::optional<HitInfo> intersect(Ray ray, float tMin, float tMax) const = 0;
	virtual AABB boundingBox() const = 0;
};
