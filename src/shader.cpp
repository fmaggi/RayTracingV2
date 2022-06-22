#include "shader.h"

#include "glm/common.hpp"

glm::vec3 normalizeColor(glm::vec3 color, int samples) {
	color *= 1.0f / samples;

	float r = glm::clamp(color.r, 0.f, 0.99f);
	float g = glm::clamp(color.g, 0.f, 0.99f);
	float b = glm::clamp(color.b, 0.f, 0.99f);

	return glm::vec3{r, g, b};
}
