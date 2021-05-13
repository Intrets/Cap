#pragma once

#include <wglm/glm.hpp>

#include <utility>
#include <vector>
#include <sstream>

#define DEFAULTCOPY(T) T(const T&) = default; T& operator=(const T&) = default;
#define NOCOPY(T) T(const T&) = delete; T& operator=(const T&) = delete;
#define DEFAULTMOVE(T) T(T&&) = default; T& operator=(T&&) = default;
#define NOMOVE(T) T(T&&) = delete; T& operator=(T&&) = delete;
#define DEFAULTCOPYMOVE(T) DEFAULTCOPY(T) DEFAULTMOVE(T)
#define NOCOPYMOVE(T) NOCOPY(T) NOMOVE(T)

inline int32_t floordiv(int32_t x, int32_t y) {
	return (x + (x < 0)) / y - (x < 0);
}

inline glm::ivec2 floordiv(glm::ivec2 v, int32_t y) {
	return glm::ivec2(floordiv(v.x, y), floordiv(v.y, y));
};

inline std::pair<int32_t, int32_t> floordivmod(int32_t x, int32_t y) {
	int32_t div = floordiv(x, y);
	int32_t mod = x - div * y;
	return std::make_pair(div, mod);
}

inline std::pair<glm::ivec2, glm::ivec2> floordivmod(glm::ivec2 v, int32_t y) {
	auto pair1 = floordivmod(v.x, y);
	auto pair2 = floordivmod(v.y, y);
	return std::make_pair(glm::ivec2(pair1.first, pair2.first), glm::ivec2(pair1.second, pair2.second));
}

template<class T>
inline bool indexInVector(size_t const i, std::vector<T> const& V) {
	return i >= 0 && i < V.size();
}

inline bool periodic(int32_t val, int32_t on, int32_t off, int32_t offset = 0) {
	return ((val + offset) % (on + off)) <= on;
}

template<class T, size_t s>
inline bool indexInArray(size_t const i, std::array<T, s>& A) {
	return i >= 0 && i < s;
}

inline int32_t idot(glm::ivec2 v1, glm::ivec2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

struct hashVoidPtr
{
	std::size_t operator()(void* const& s) const noexcept {
		return std::hash<void*>{}(const_cast<void*>(s));
	}
};

inline float norm2(glm::vec2 v) {
	return v.x * v.x + v.y * v.y;
}

inline std::streamoff getSize(std::basic_stringstream<char>& b) {
	return b.tellp() - b.tellg();
}
