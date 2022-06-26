#include "renderer.h"

#include "utils/math.h"

glm::vec3 Renderer::rayColor(Ray ray, const Scene& scene, uint32_t depth) const {
	if (depth <= 0) {
		return glm::vec3(0);
	}

  auto info = scene.intersect(ray);
  if (info){
		glm::vec3 dir = info->n + Math::randomInUnitSphere();
		Ray reflection(info->p, dir);
		return 0.5f * rayColor(reflection, scene, depth--);
  }
	return scene.backgroundColor(ray);
}

glm::vec3 Renderer::gammaCorrectColor(glm::vec3 color) const {
	float rc = sqrt(color.r * invSamples);
	float gc = sqrt(color.g * invSamples);
	float bc = sqrt(color.b * invSamples);

	float r = glm::clamp(rc, 0.f, 0.99f);
	float g = glm::clamp(gc, 0.f, 0.99f);
	float b = glm::clamp(bc, 0.f, 0.99f);

	return glm::vec3{r, g, b};
}

Pixel Renderer::shadePixel(glm::vec2 uv, const Camera& camera, const Scene& scene) const {
	glm::vec3 color{};
	for (uint32_t s = 0; s < samples; s++) {
		uv.x += Math::random<float>() * invWidth;
		uv.y += Math::random<float>() * invHeight;
		color += rayColor(camera.castRay(uv), scene, reflections);
	}
	color = gammaCorrectColor(color);
	Pixel pixel;
	pixel.r = (uint8_t)(color.r * 255);
	pixel.g = (uint8_t)(color.g * 255);
	pixel.b = (uint8_t)(color.b * 255);
	return pixel;
}

Image Renderer::render(const Camera &camera, const Scene &scene) {
	Image image(imageWidth, imageHeight);

	invWidth = 1.0f / imageWidth;
	invHeight = 1.0f / imageHeight;
	invSamples = 1.0f / samples;

	for (uint32_t y = 0; y < imageHeight; y++) {
		for (uint32_t x = 0; x < imageWidth; x++) {
			glm::vec2 uv = { (float)x * invWidth, (float)y * invHeight };
			image.at(x, imageHeight-y-1) = shadePixel(uv, camera, scene);
		}
	}

	return image;
}


