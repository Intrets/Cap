#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <misc/Timer.h>
#include <misc/Option.h>

#include <mem/Locator.h>

#include "infos/RenderInfo.h"
#include "BlitRendererArray.h"
#include "textures/BlockIDTextures.h"
#include "GLStateWrapper.h"

namespace render
{
	void Renderer::render(GLFWwindow* window, RenderInfo const& renderInfo) {
		bwo::FrameBuffer target{ window };

		Locator<misc::Timer>::ref().newTiming("Render");

		target.clear({ 0.5f, 0.5f, 0.5f, 1.0f }, true);

		auto config = ogs::WorldTileConfiguration();

		Locator<BlitRendererArrayTexture>::ref().render(
			config,
			renderInfo.tileRenderInfo,
			target,
			{ 0, 0, renderInfo.cameraInfo.x, renderInfo.cameraInfo.y },
			Locator<BlockIDTextures>::ref().getTextureArray(),
			std::nullopt,
			renderInfo.cameraInfo.VP
		);

		target.clearDepth();

		this->uiBackgroundRenderer.render(renderInfo.uiRenderInfo, 0, renderInfo.cameraInfo);

		this->textRenderer.render(renderInfo.textRenderInfo,
								  Locator<Fonts>::ref(),
								  target);

		if (misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal()) {
			//this->debugRenderer.render(0, renderInfo);
		}
		Locator<misc::Timer>::ref().endTiming("Render");

		Locator<misc::Timer>::ref().newTiming("Swap Buffers");
		glfwSwapBuffers(window);
		Locator<misc::Timer>::ref().endTiming("Swap Buffers");
	}
}
