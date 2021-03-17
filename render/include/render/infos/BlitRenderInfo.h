#pragma once

#include <wglm/glm.hpp>
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
	private:
		std::vector<SingleBlitRenderInfo> data;

	public:
		std::vector<SingleBlitRenderInfo> const& getData() const;

		std::vector<SingleBlitRenderInfo>& getDataMut();

		void addBlitInfo(glm::vec4 q, glm::vec4 w, int32_t r);
		void addBlitInfo(SingleBlitRenderInfo const& info);
	};
}