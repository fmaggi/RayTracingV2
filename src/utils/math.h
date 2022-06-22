#pragma once

#include <random>
#include "glm/vec3.hpp"

namespace Math {

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

template<typename T>
inline T random() {
	static std::uniform_real_distribution<T> dist(T{0}, T{1});
	static std::mt19937 gen;
	return dist(gen);
}

template<>
inline glm::vec3 random<glm::vec3>() {
	return glm::vec3{ random<float>(), random<float>(),	random<float>()	};
}

template<typename T>
inline T random(T min, T max) {
	return min + (max-min)*random<T>();
}

}
