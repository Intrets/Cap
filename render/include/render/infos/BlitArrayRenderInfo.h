#pragma once

#include <wglm/glm.hpp>
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
	private:
		std::vector<SingleBlitArrayRenderInfo> data;

	public:
		std::vector<SingleBlitArrayRenderInfo> const& getData() const;

		void addBlitInfo(glm::vec4 w, int32_t r, int32_t l);
	};
}
