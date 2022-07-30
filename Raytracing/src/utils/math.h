#pragma once

#include <cstdint>
#include <limits>
#include <random>
#include "glm/geometric.hpp"
#include "glm/vec3.hpp"

namespace Math {

static constexpr float infinity = std::numeric_limits<float>::infinity();
static constexpr float pi = 3.1415926535897932385;
static constexpr float invPi = 1.0f/pi;
static constexpr float almostZero = 1e-8f;

inline bool nearZero(glm::vec3 v) {
	return v.x < Math::almostZero && v.y < Math::almostZero && v.z < Math::almostZero;
}

}

namespace Random {

inline int Int() {
	static std::mt19937 gen;
	static std::uniform_int_distribution<int> dist;
	return dist(gen);
}

inline int Int(int min, int max) {
	return min + Int() % (max - min + 1);
}

inline float Float() {
	return (float)Int() / (float)std::numeric_limits<int>::max();
}

inline float Float(float min, float max) {
	return min + (max - min) * Float();
}

inline glm::vec3 Vec3() {
	return glm::vec3(Float(), Float(), Float());
}

inline glm::vec3 Vec3(float min, float max) {
	return glm::vec3(Float(min, max), Float(min, max), Float(min, max));
}

inline glm::vec3 UnitVec3() {
	return glm::normalize(Vec3());
}

inline glm::vec3 InUnitSphere() {
	while (true) {
		auto v = Vec3(-1.0f, 1.0f);
		if (glm::dot(v, v) > 1) {
			continue;
		}
		return v;
	}
}

}
