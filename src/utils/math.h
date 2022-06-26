#pragma once

#include <cstdint>
#include <random>
#include "glm/geometric.hpp"
#include "glm/vec3.hpp"

namespace Math {

static constexpr float infinity = std::numeric_limits<float>::infinity();
static constexpr float pi = 3.1415926535897932385;
static constexpr float almostZero = 1e-8f;

namespace internal {
	inline float random() {
		static std::mt19937 gen;
		static std::uniform_real_distribution<float> dist{0.0f, 1.0f};
		return dist(gen);
	}
}

template<typename T>
inline T random() {
	return static_cast<T>(internal::random());
}

template<>
inline glm::vec3 random<glm::vec3>() {
	return glm::vec3{ random<float>(), random<float>(),	random<float>()	};
}

template<typename T, typename V>
inline T random(V min, V max) {
	return min + (max-min)*random<T>();
}

template<>
inline glm::vec3 random<glm::vec3, float>(float min, float max) {
	return glm::vec3{ random<float>(min, max), random<float>(min, max), random<float>(min, max) };
}

inline glm::vec3 randomUnitVec3() {
	return glm::normalize(random<glm::vec3>());
}

inline glm::vec3 randomInUnitSphere() {
	while (true) {
		auto v = random<glm::vec3>(-1.0f, 1.0f);
		if (glm::dot(v, v) > 1) {
			continue;
		}
		return v;
	}
}

inline bool nearZero(glm::vec3 v) {
	return v.x < almostZero && v.y < almostZero && v.z < almostZero;
}

}
