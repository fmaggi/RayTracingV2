#include "Camera.h"

#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/trigonometric.hpp"


Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 Vup, float vfov, float aspectRatio) {
	m_position = position;

	float theta = glm::radians(vfov);
	float h = glm::tan(theta / 2);

	float viewportHeight = h * 2;
	float viewportWidth = viewportHeight * aspectRatio;

	glm::vec3 direction = glm::normalize(m_position - lookAt);

	glm::vec3 w = direction;
	glm::vec3 u = glm::normalize(glm::cross(Vup, w));
	glm::vec3 v = glm::cross(w, u);

	glm::vec3 hDir = u * viewportWidth;
	glm::vec3 vDir = v * viewportHeight;

	m_projectionPlane.horizontalDirection = hDir;
	m_projectionPlane.verticalDirection = vDir;

	m_projectionPlane.lowerLeftCorner = position - hDir/2.0f - vDir/2.0f - w;
};

Ray Camera::castRay(float u, float v) {
	return Ray(m_position, m_projectionPlane.lowerLeftCorner + u*m_projectionPlane.horizontalDirection + v*m_projectionPlane.verticalDirection - m_position);
}

float Camera::viewportWidth() {
	return glm::length(m_projectionPlane.horizontalDirection);
}

float Camera::viewportHeight() {
	return glm::length(m_projectionPlane.verticalDirection);
}



