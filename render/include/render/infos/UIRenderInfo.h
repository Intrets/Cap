#pragma once

#include <wglm/glm.hpp>
#include <vector>

class Rect;

namespace render
{
	struct SingleUIRenderInfo
	{
		glm::vec2 scale;
		glm::vec3 positions;
		glm::vec4 colors;
	};

	struct UIRenderInfo
	{
	private:
		std::vector<SingleUIRenderInfo> data;

	public:
		std::vector<SingleUIRenderInfo> const& getData() const;

		void addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color);
		void addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color, int32_t depth_);
		void addRectangle(Rect& rec, glm::vec4 color);
	};
}
