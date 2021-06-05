#pragma once

#include <wglm/glm.hpp>
#include <string>
#include <concepts>

namespace colors
{
	struct Color
	{
		uint32_t c;
	};

	static constexpr Color red = { 0xFF0000FF };
	static constexpr Color green = { 0xFF00FF00 };
	static constexpr Color blue = { 0xFFFF0000 };
	static constexpr Color magenta = { 0xFFFF00FF };
	static constexpr Color yellow = { 0xF00FFFFF };
	static constexpr Color cyan = { 0xFFFFFF00 };
	static constexpr Color white = { 0xFFFFFFFF };

	template<class T>
	Color uniqueColor(T val) {
		return { static_cast<uint32_t>(std::hash<T>{}(val)) | 0xFF000000 };
	}

	constexpr Color alpha(Color c, std::integral auto val) {
		assert(0 <= val && val < 256);

		return { c.c & (0x00FFFFFF | (static_cast<uint32_t>(val) << 24)) };
	}

	constexpr Color alpha(Color c, std::floating_point auto val) {
		assert(0.0 <= val && val < 1.0);

		return alpha(c, static_cast<int32_t>(256 * val));
	}
}

constexpr uint32_t char_to_hex(const char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	else {
		assert(0);
		return 0;
	}
};

constexpr uint32_t operator"" _hex(const char* c, std::size_t size) {
	uint32_t hex = 0;
	uint32_t offset = 0;
	if (size == 7 || size == 6) {
		hex = 0xff;
	}
	else {
		hex = (char_to_hex(c[size - 2 - offset]) << 4) + char_to_hex(c[size - 1 - offset]);
		offset += 2;
	}

	for (uint32_t i = 0; i < 6; i++) {
		hex += char_to_hex(c[size - i - 1 - offset]) << (4 * (i + 2));
	}
	return hex;
};

namespace COLORS
{
	glm::vec4 DARKEN(glm::vec4 color);
	glm::vec4 DARKEN2(glm::vec4 color);
	glm::vec4 LIGHTEN(glm::vec4 color);

	glm::vec4 rgb(int32_t r, int32_t g, int32_t b);
	glm::vec4 rgba(int32_t r, int32_t g, int32_t b, int32_t a);
	glm::vec4 rgba(int32_t r, int32_t g, int32_t b, float a);
	glm::vec4 rgba(int32_t r, int32_t g, int32_t b, double a);
	glm::vec4 hex(std::string h);
	glm::vec4 hex(uint32_t h);

	static const glm::vec4 NICERED = hex("#ff4747"_hex);

	namespace UI
	{
		static const glm::vec4 CURSOR = rgb(191, 0, 0);
		static const glm::vec4 RED = rgb(255, 153, 153);
		static const glm::vec4 GREEN = rgb(153, 255, 153);
		static const glm::vec4 WINDOWBACKGROUND = rgb(193, 193, 157);


		static const glm::vec4 TEXT = hex("#104070"_hex);
		static const glm::vec4 BACKGROUND = hex("#d7d7af"_hex);
		static const glm::vec4 FOREGROUND = hex("#CDCDA5"_hex);
		static const glm::vec4 FOCUSSED = hex("#e0dfbc"_hex);
	}

	namespace GR
	{
		static const glm::vec4 SELECTION = rgba(50, 50, 255, 0.5);
		static const glm::vec4 HIGHLIGHT = rgba(50, 255, 50, 0.5);
		static const glm::vec4 HOVER = rgba(255, 255, 255, 0.5);
		static const glm::vec4 BLOCKED = rgba(255, 0, 0, 0.5);
	}
}
