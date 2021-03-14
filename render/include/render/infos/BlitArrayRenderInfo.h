#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace render
{
#pragma pack(push, 1)
	struct SingleBlitArrayRenderInfo
	{
		glm::vec4 world;
		int32_t rotation;
		int32_t layer;
	};
#pragma pack(pop)

	struct BlitArrayRenderInfo
	{
		std::vector<SingleBlitArrayRenderInfo> data;
	};
}
