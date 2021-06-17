#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <misc/Timer.h>
#include <misc/Option.h>

#include <mem/Global.h>

#include "infos/RenderInfo.h"
#include "infos/DebugRenderInfo.h"
#include "BlitRendererArray.h"
#include "textures/BlockIDTextures.h"
#include "GLStateWrapper.h"

namespace render
{
	void Renderer::render(GLFWwindow* window, RenderInfo const& renderInfo) {
		bwo::FrameBuffer target{ window };

		Global<misc::Timer>::ref().newTiming("Render");

		target.clear({ 0.5f, 0.5f, 0.5f, 1.0f }, true);

		auto config = ogs::WorldTileConfiguration();
		glm::ivec4 viewport{ 0,0,renderInfo.cameraInfo.x, renderInfo.cameraInfo.y };

		Global<BlitRendererArrayTexture>::ref().render(
			config,
			renderInfo.tileRenderInfo,
			target,
			viewport,
			Global<BlockIDTextures>::ref().getTextureArray(),
			std::nullopt,
			renderInfo.cameraInfo.VP
		);

		this->highlightRenderer.render(
			renderInfo.highlightRenderInfo,
			target,
			viewport,
			renderInfo.cameraInfo.VP
		);

		target.clearDepth();

		this->uiBackgroundRenderer.render(
			renderInfo.uiRenderInfo,
			0,
			renderInfo.cameraInfo
		);

		this->textRenderer.render(
			renderInfo.textRenderInfo,
			Global<Fonts>::ref(),
			target
		);

		if (misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal()) {
			this->debugRenderer.render(
				Global<DebugRenderInfo>::ref(),
				target,
				viewport,
				renderInfo.cameraInfo
			);

			Global<DebugRenderInfo>::ref().clear();
		}
		Global<misc::Timer>::ref().endTiming("Render");

		Global<misc::Timer>::ref().newTiming("Swap Buffers");
		glfwSwapBuffers(window);
		Global<misc::Timer>::ref().endTiming("Swap Buffers");
	}
}
