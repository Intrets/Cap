#pragma once

#include "TextRenderer.h"
#include "UIBackgroundRenderer.h"

namespace render
{
	struct RenderInfo;

	class Renderer
	{
	public:
		TextRenderer textRenderer;
		UIBackgroundRenderer uiBackgroundRenderer;

		void render(GLFWwindow* window, RenderInfo const& renderInfo);
	};
}
