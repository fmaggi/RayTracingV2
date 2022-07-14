#pragma once

#include "hit.h"
#include "ray.h"

#include <optional>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

struct Material {
	Material(glm::vec3 albedo, bool emissive=false)
		: albedo(albedo), emissive(emissive) {}
	virtual ~Material() {}
	virtual std::optional<Ray> scatter(Ray ray, HitInfo hit) const { return{}; }
	virtual glm::vec3 attenuation(glm::vec3 wo, glm::vec3 wi) const { return glm::vec3(0.0f); }
	virtual glm::vec3 emit() const { return glm::vec3(0.0f); }
	const glm::vec3 albedo;
	const bool emissive = false;
};

struct Lambertian : public Material {
	Lambertian(glm::vec3 color)
		: Material(color) {}

	std::optional<Ray> scatter(Ray ray, HitInfo hit) const override;
	glm::vec3 attenuation(glm::vec3 wo, glm::vec3 wi) const override { return albedo; }
};

struct Metal : public Material {
	Metal(glm::vec3 color, float fuzz=0)
		: Material(color), fuzz(fuzz > 1 ? 1 : fuzz) {}

	std::optional<Ray> scatter(Ray ray, HitInfo hit) const override;
	const float fuzz;
};

struct Dielectric : public Material {
	Dielectric(glm::vec3 albedo, float refraction)
		: Material(albedo), ir(refraction) {}

	std::optional<Ray> scatter(Ray ray, HitInfo hit) const override;
	const float ir = 0;
};

struct Emissive : public Material {
	Emissive(glm::vec3 albedo, float intensity)
		: Material(albedo, true), intensity(intensity) {}
	glm::vec3 emit() const override;
	const float intensity;
};

#pragma GCC diagnostic pop
