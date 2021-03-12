#include "Colors.h"

#include <sstream>

glm::vec4 COLORS::rgb(int32_t r, int32_t g, int32_t b) {
	return glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}

glm::vec4 COLORS::rgba(int32_t r, int32_t g, int32_t b, int32_t a) {
	return glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

glm::vec4 COLORS::rgba(int32_t r, int32_t g, int32_t b, float a) {
	return rgba(r, g, b, static_cast<int32_t>(glm::round(255.0f * a)));
}

glm::vec4 COLORS::rgba(int32_t r, int32_t g, int32_t b, double a) {
	return rgba(r, g, b, static_cast<int32_t>(glm::round(255.0 * a)));
}

glm::vec4 COLORS::hex(std::string h) {
	if (h[0] == '#') {
		h.erase(0, 1);
	}
	std::stringstream ss;
	ss << std::hex << h;
	uint32_t hexval;
	ss >> hexval;
	return hex(hexval);
}

glm::vec4 COLORS::hex(uint32_t h) {
	int32_t r, g, b, a;
	a = h & 0xff;
	h >>= 8;
	b = h & 0xff;
	h >>= 8;
	g = h & 0xff;
	h >>= 8;
	r = h & 0xff;

	return rgba(r, g, b, a);
}

glm::vec4 COLORS::DARKEN(glm::vec4 color) {
	return color * glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
}

glm::vec4 COLORS::DARKEN2(glm::vec4 color) {
	return color * glm::vec4(0.64f, 0.64f, 0.64f, 1.0f);
}

glm::vec4 COLORS::LIGHTEN(glm::vec4 color) {
	return color / glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
}
