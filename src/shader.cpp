#include "shader.h"

#include "glm/common.hpp"

glm::vec3 normalizeColor(glm::vec3 color, int samples) {
	float scale = 1.0f / samples;

	float rc = sqrt(color.r * scale);
	float gc = sqrt(color.g * scale);
	float bc = sqrt(color.b * scale);

	float r = glm::clamp(rc, 0.f, 0.99f);
	float g = glm::clamp(gc, 0.f, 0.99f);
	float b = glm::clamp(bc, 0.f, 0.99f);

	return glm::vec3{r, g, b};
}
