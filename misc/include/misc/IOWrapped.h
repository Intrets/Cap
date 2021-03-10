#pragma once

#include <glm/glm.hpp>

#include <istream>
#include <ostream>

// vec2

inline std::ostream& operator<<(std::ostream& out, const glm::vec2& v) {
	out << v.x << " " << v.y;
	return out;
}

inline std::istream& operator>>(std::istream& in, glm::vec2& v) {
	in >> v.x >> v.y;
	return in;
}

// vec3

inline std::ostream& operator<<(std::ostream& out, const glm::vec3& v) {
	out << v.x << " " << v.y << " " << v.z;
	return out;
}

inline std::istream& operator>>(std::istream& in, glm::vec3& v) {
	in >> v.x >> v.y >> v.z;
	return in;
}

// vec 4

inline std::ostream& operator<<(std::ostream& out, const glm::vec4& v) {
	out << v.x << " " << v.y << " " << v.z << " " << v.w;
	return out;
}

inline std::istream& operator>>(std::istream& in, glm::vec4& v) {
	in >> v.x >> v.y >> v.z >> v.w;
	return in;
}

// ivec2

inline std::ostream& operator<<(std::ostream& out, const glm::ivec2& v) {
	out << v.x << " " << v.y;
	return out;
}

inline std::istream& operator>>(std::istream& in, glm::ivec2& v) {
	in >> v.x >> v.y;
	return in;
}

// ivec3

inline std::ostream& operator<<(std::ostream& out, const glm::ivec3& v) {
	out << v.x << " " << v.y << " " << v.z;
	return out;
}

inline std::istream& operator>>(std::istream& in, glm::ivec3& v) {
	in >> v.x >> v.y >> v.z;
	return in;
}

// ivec 4

inline std::ostream& operator<<(std::ostream& out, const glm::ivec4& v) {
	out << v.x << " " << v.y << " " << v.z << " " << v.w;
	return out;
}

inline std::istream& operator>>(std::istream& in, glm::ivec4& v) {
	in >> v.x >> v.y >> v.z >> v.w;
	return in;
}