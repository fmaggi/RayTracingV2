#include "renderer.h"

#include "utils/math.h"

namespace Renderer {

	static glm::vec3 ray_color(Ray ray, const Scene& scene, int depth) {
		if (depth <= 0) {
			return glm::vec3(0);
		}

 	  auto info = scene.intersect(ray);
 	  if (info){
			glm::vec3 dir = info->n + Math::randomInUnitSphere();
			Ray reflection(info->p, dir);
			return 0.5f * ray_color(reflection, scene, depth--);
 	  }
		return scene.backgroundColor(ray);
	}

	static glm::vec3 normalizeColor(glm::vec3 color, int samples) {
		float scale = 1.0f / samples;

		float rc = sqrt(color.r * scale);
		float gc = sqrt(color.g * scale);
		float bc = sqrt(color.b * scale);

		float r = glm::clamp(rc, 0.f, 0.99f);
		float g = glm::clamp(gc, 0.f, 0.99f);
		float b = glm::clamp(bc, 0.f, 0.99f);

		return glm::vec3{r, g, b};
	}

	static glm::vec3 shadePixel(glm::vec2 uv, const Camera& camera, const Scene& scene) {
			return ray_color(camera.castRay(uv.x, uv.y), scene, 50);
	}

	Image render(const Camera &camera, const Scene &scene) {
		static constexpr uint32_t numberofRays = 150; // TEMPORARY
		static constexpr uint32_t samples = 8;
		uint32_t w = camera.viewportWidth() * numberofRays;
		uint32_t h = camera.viewportHeight() * numberofRays;

		Image image(w, h);

		for (uint32_t y = 0; y < h; y++) {
			for (uint32_t x = 0; x < w; x++) {
				glm::vec3 color{};
				for (uint32_t k = 0; k < samples; k++) {
					glm::vec2 uv = {
						((float)x + Math::random<float>()) / w,
						((float)y + Math::random<float>()) / h
					};
					color += shadePixel(uv, camera, scene);
				}
				color = normalizeColor(color, samples);
				Pixel& pixel = image.at(x, h-y-1);
				pixel.r = (uint8_t)(color.r * 255);
				pixel.g = (uint8_t)(color.g * 255);
				pixel.b = (uint8_t)(color.b * 255);
			}
		}

		return image;
	}

}
