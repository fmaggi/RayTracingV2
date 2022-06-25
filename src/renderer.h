#pragma once

#include "image.h"
#include "Camera.h"
#include "scene.h"

namespace Renderer {

	Image render(const Camera& camera, const Scene& scene);

}
