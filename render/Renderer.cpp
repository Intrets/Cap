#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <misc/Timer.h>
#include <misc/Option.h>

#include <mem/Locator.h>

#include "infos/RenderInfo.h"
#include "BlitRendererArray.h"
#include "textures/BlockIDTextures.h"

namespace render
{
	void Renderer::render(GLFWwindow* window, RenderInfo const& renderInfo) {
		bwo::FrameBuffer target{ window };

		Locator<misc::Timer>::ref().newTiming("Render");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Locator<BlitRendererArrayTexture>::ref().render(
			renderInfo.tileRenderInfo.pos,
			renderInfo.tileRenderInfo.rotation,
			renderInfo.tileRenderInfo.texture,
			target,
			{ 0, 0, renderInfo.cameraInfo.x, renderInfo.cameraInfo.y },
			Locator<BlockIDTextures>::ref().getTextureArray(),
			std::nullopt,
			renderInfo.cameraInfo.VP
		);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT);

		this->uiBackgroundRenderer.render(renderInfo.uiRenderInfo, 0, renderInfo.cameraInfo);

		this->textRenderer.render(renderInfo.textRenderInfo,
								  Locator<Fonts>::ref(),
								  target,
								  renderInfo.cameraInfo);

		if (misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal()) {
			//this->debugRenderer.render(0, renderInfo);
		}
		Locator<misc::Timer>::ref().endTiming("Render");

		Locator<misc::Timer>::ref().newTiming("Swap Buffers");
		glfwSwapBuffers(window);
		Locator<misc::Timer>::ref().endTiming("Swap Buffers");
	}
}
