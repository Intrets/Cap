#pragma once

#include <vector>

#include <wglm/glm.hpp>

namespace render
{
	struct TileRenderInfo
	{
		std::vector<glm::vec2> pos;
		std::vector<int32_t> rotation;
		std::vector<int32_t> texture;

		void push(glm::vec2 p, int32_t rot, int32_t tex);
	};
}