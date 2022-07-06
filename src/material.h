#pragma once

#include "ray.h"

class Material;

struct HitInfo {
	glm::vec3 p;
	glm::vec3 n;
	Material* material;
	float t;
	bool frontFace;

	void setNormal(glm::vec3 hitDir, glm::vec3 normal) {
		frontFace = glm::dot(hitDir, normal) < 0;
		n = frontFace ? normal : -normal;
	}
};

class Material {
public:

	Material(glm::vec3 albedo)
		: albedo(albedo) {}
	virtual ~Material() {}
	virtual Ray scatter(Ray ray, HitInfo hit) const = 0;
	virtual glm::vec3 attenuation(glm::vec3 wo, glm::vec3 wi) const = 0;
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
	glm::vec3 attenuation(glm::vec3 wo, glm::vec3 wi) const override { return glm::vec3(0.0f); }
	const float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(glm::vec3 albedo, float refraction)
		: Material(albedo), ir(refraction) {}

	Ray scatter(Ray ray, HitInfo hit) const override;
	glm::vec3 attenuation(glm::vec3 wo, glm::vec3 wi) const override { return glm::vec3(0.0f); }
	const float ir = 0;
};
