#pragma once

#include "ray.h"

#include <optional>

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
	virtual ~Material() {}
	virtual std::optional<Ray> scatter(Ray ray, HitInfo hit) const = 0;
	virtual glm::vec3 attenuation() const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(glm::vec3 color)
		: albedo(color) {}

	std::optional<Ray> scatter(Ray ray, HitInfo hit) const override;
	glm::vec3 attenuation() const override { return albedo; }
private:
	glm::vec3 albedo;
};

class Metal : public Material {
public:
	Metal(glm::vec3 color, float fuzz=0)
		: albedo(color), fuzz(fuzz > 1 ? 1 : fuzz) {}

	std::optional<Ray> scatter(Ray ray, HitInfo hit) const override;
	glm::vec3 attenuation() const override { return albedo; }
private:
	glm::vec3 albedo;
	float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float refraction)
		: ir(refraction) {}

	std::optional<Ray> scatter(Ray ray, HitInfo hit) const override;
	glm::vec3 attenuation() const override { return glm::vec3(1); }
private:
	float ir = 0;
};
