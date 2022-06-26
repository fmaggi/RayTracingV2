#pragma once

#include "ray.h"

#include <optional>

class Material {
public:
	virtual ~Material() {}
	virtual std::optional<Ray> scatter(glm::vec3 hitPoint, glm::vec3 hitNormal) const = 0;
	virtual glm::vec3 lightValue() const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(glm::vec3 color)
		: albedo(color) {}

	std::optional<Ray> scatter(glm::vec3 hitPoint, glm::vec3 hitNormal) const override;
	glm::vec3 lightValue() const override { return albedo; }
private:
	glm::vec3 albedo;
};

class Metal : public Material {
public:
	Metal(glm::vec3 color)
		: albedo(color) {}

	std::optional<Ray> scatter(glm::vec3 hitPoint, glm::vec3 hitNormal) const override;
	glm::vec3 lightValue() const override { return albedo; }
private:
	glm::vec3 albedo;
};
