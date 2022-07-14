#pragma once

#include "glm/vec3.hpp"

struct Material;

struct HitInfo {
	HitInfo() {}
	HitInfo(glm::vec3 point, float t, glm::vec3 normal, glm::vec3 hitDir, Material* material);

	glm::vec3 p;
	glm::vec3 n;
	glm::vec3 hitDir;
	Material* material;
	float t;
	bool frontFace;
};

