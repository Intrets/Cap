#pragma once

#include <glm/glm.hpp>
#include <vector>

class Rect;

namespace render
{
	struct UIRenderInfo
	{
		std::vector<glm::vec2> scales;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec4> colors;

		void addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color);
		void addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color, int32_t depth_);
		void addRectangle(Rect& rec, glm::vec4 color);
	};
}
