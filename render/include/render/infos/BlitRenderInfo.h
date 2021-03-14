#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace render
{
#pragma pack(push, 1)
	struct SingleBlitRenderInfo
	{
		glm::vec4 quad;
		glm::vec4 world;
		int32_t rotation;
	};
#pragma pack(pop)

	struct BlitRenderInfo
	{
		std::vector<SingleBlitRenderInfo> data;
	};
}