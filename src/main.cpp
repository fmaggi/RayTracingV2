#include <cstdio>
#include <cstdint>
#include <cstdlib>

int main(int argc, char** argv) {
	std::printf("Hello raytraced world\n");

	if (argc != 2) {
		std::printf("Usage:\n\t raytrace [output]\n");
		return -1;
	}

	FILE* image = std::fopen(argv[1], "w");
	std::fprintf(image, "P3\n%i %i\n255\n", 255, 255);

	for (int i = 0; i < 255; i++){
		for (int j = 0; j < 255; j++) {
			std::fprintf(image, "%i %i %i\n", i, j, 0);
		}
	}
}
