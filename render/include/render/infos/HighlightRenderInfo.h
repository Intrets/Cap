#pragma once

#include <vector>

#include <wglm/glm.hpp>
#include <render/Colors.h>

namespace render
{

	namespace highlight
	{
		constexpr colors::Color defaultColor = colors::alpha(colors::green, 0.5f);
	}
	struct SingleHighlightRenderInfo
	{
		glm::vec2 offset;
		glm::vec2 scale;
		colors::Color color;
	};

	struct HighlightRenderInfo
	{
		std::vector<SingleHighlightRenderInfo> infos;

		void addBox(glm::vec2 p1, glm::vec2 p2, colors::Color c = highlight::defaultColor);
	};
}