#pragma once

#include "hit.h"
#include "ray.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

class Material {
public:

	Material(glm::vec3 albedo)
		: albedo(albedo) {}
	virtual ~Material() {}
	virtual Ray scatter(Ray ray, HitInfo hit) const = 0;
	virtual glm::vec3 attenuation(glm::vec3 wo, glm::vec3 wi) const { return glm::vec3(0.0f); }
	virtual glm::vec3 absortion(Ray ray, HitInfo hit) const { return glm::vec3(0.0f); }
	const glm::vec3 albedo;
};

class Lambertian : public Material {
public:
	Lambertian(glm::vec3 color)
		: Material(color) {}

	Ray scatter(Ray ray, HitInfo hit) const override;
	glm::vec3 attenuation(glm::vec3 wo, glm::vec3 wi) const override { return albedo; }
};

class Metal : public Material {
public:
	Metal(glm::vec3 color, float fuzz=0)
		: Material(color), fuzz(fuzz > 1 ? 1 : fuzz) {}

	Ray scatter(Ray ray, HitInfo hit) const override;
	const float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(glm::vec3 albedo, float refraction)
		: Material(albedo), ir(refraction) {}

	Ray scatter(Ray ray, HitInfo hit) const override;
	glm::vec3 absortion(Ray ray, HitInfo info) const override;
	const float ir = 0;
};

#pragma GCC diagnostic pop
