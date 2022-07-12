#pragma once

#include "glm/vec3.hpp"
#include "ray.h"

struct VisibilityTester {
	glm::vec3 p;
	glm::vec3 lightDir;
};

class Light {
public:
	Light(glm::vec3 color, float intensity)
		: color(color), intensity(intensity) {}
	virtual ~Light() {}
	virtual glm::vec3 lightColor(glm::vec3 hitPoint, glm::vec3 hitNormal) const = 0;
	virtual VisibilityTester visibilityTester(glm::vec3 hitPoint) const = 0;
	virtual glm::vec3 position() const = 0;
	glm::vec3 color;
protected:
	float intensity;
};

class PointLight : public Light {
public:
	PointLight(glm::vec3 color, float intensity, glm::vec3 position)
		: Light(color, intensity), pos(position) {}

	glm::vec3 lightColor(glm::vec3 hitPoint, glm::vec3 hitNormal) const override;
	VisibilityTester visibilityTester(glm::vec3 hitPoint) const override;
	glm::vec3 position() const override { return pos; }
private:
	glm::vec3 pos;
};
