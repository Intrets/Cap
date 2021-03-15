#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace render
{
	struct SingleBlitArrayRenderInfo
	{
		glm::vec4 world;
		int32_t rotation;
		int32_t layer;
	};

	struct BlitArrayRenderInfo
	{
		std::vector<SingleBlitArrayRenderInfo> data;
	};
}
