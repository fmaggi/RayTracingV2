#include "hit.h"

#include "glm/glm.hpp"

HitInfo::HitInfo(glm::vec3 point, float t, glm::vec3 normal, glm::vec3 hitDir, Material* material) {
	this->t = t;
	this->p = point;
	this->material = material;
	this->frontFace = glm::dot(hitDir, normal) < 0;
	this->n = this->frontFace ? normal : -normal;
	this->hitDir = hitDir;
}
