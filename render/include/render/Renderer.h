#pragma once

#include "TextRenderer.h"
#include "UIBackgroundRenderer.h"
#include "DebugRenderer.h"

namespace render
{
	struct RenderInfo;

	class Renderer
	{
	public:
		TextRenderer textRenderer;
		UIBackgroundRenderer uiBackgroundRenderer;
		DebugRenderer debugRenderer;

		void render(GLFWwindow* window, RenderInfo const& renderInfo);
	};
}
