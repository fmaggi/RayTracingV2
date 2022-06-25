#pragma once

#include <cstdint>

enum class ImageFormat {
	None,
	RGB,
	RGBA
};

template<ImageFormat>
struct Pixel;

template<>
struct Pixel<ImageFormat::RGB> {
	union {
		struct {
			uint8_t r, g, b;
		};
		uint8_t value[3];
	};
};

template<>
struct Pixel<ImageFormat::RGBA> {
	union {
		struct {
			uint8_t r, g, b, a;
		};
		uint8_t value[4];
	};
};

template<ImageFormat format = ImageFormat::RGB>
struct Image {

	static constexpr uint64_t pixelSize = sizeof(Pixel<format>);

	Image(uint32_t w, uint32_t h)
		: width(w), height(h), pixels(new Pixel<format>[w * h]) {}

	Pixel<format>& at(uint32_t i, uint32_t j) {
		return pixels[i*width + j];
	}

	uint32_t width, height;
	Pixel<format>* pixels = nullptr;
};
