#pragma once

#include <wglm/glm.hpp>

inline float norm2(glm::vec2 v) {
	return v.x * v.x + v.y * v.y;
}
