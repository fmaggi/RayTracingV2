#pragma once

#include "ray.h"

class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 Vup, float vfov, float aspectRatio);

	Ray castRay(float u, float v) const;

	float viewportWidth() const;
	float viewportHeight() const;

private:
	glm::vec3 m_position;
	struct {
		glm::vec3 lowerLeftCorner;
		glm::vec3 horizontalDirection;
		glm::vec3 verticalDirection;
	} m_projectionPlane;
};
