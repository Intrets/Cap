#pragma once

#include "TextRenderer.h"
#include "UIBackgroundRenderer.h"
#include "DebugRenderer.h"
#include "HighlightRenderer.h"

namespace render
{
	struct RenderInfo;

	class Renderer
	{
	public:
		TextRenderer textRenderer;
		UIBackgroundRenderer uiBackgroundRenderer;
		DebugRenderer debugRenderer;
		HighlightRenderer highlightRenderer;

		void render(GLFWwindow* window, RenderInfo const& renderInfo);
	};
}
