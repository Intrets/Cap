#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace render
{
	struct SingleBlitRenderInfo
	{
		glm::vec4 quad;
		glm::vec4 world;
		int32_t rotation;
	};

	struct BlitRenderInfo
	{
		std::vector<SingleBlitRenderInfo> data;
	};
}