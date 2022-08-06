#pragma once

#include <cstdint>

struct Pixel {
	Pixel()
		: r(0), g(0), b(0), a(255){}
	union {
		struct {
			uint8_t r, g, b, a;
		};
		uint8_t value[4];
	};
};

struct Image {

	static constexpr uint64_t pixelSize = sizeof(Pixel);

	Image() {}
	Image(uint32_t w, uint32_t h)
		: width(w), height(h), pixels(new Pixel[w * h]) {}

	~Image() {
		delete[] pixels;
	}

	Pixel& at(uint32_t x, uint32_t y) {
		return pixels[x + y*width];
	}

	void resize(uint32_t w, uint32_t h) {
		delete[] pixels;
		pixels = new Pixel[w * h];
		width = w;
		height = h;
	}

	uint32_t width = 0, height = 0;
	Pixel* pixels = nullptr;
};
