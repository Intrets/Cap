#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <misc/Option.h>
#include <mem/Locator.h>
#include "infos/RenderInfo.h"

namespace render
{
	void Renderer::render(GLFWwindow* window, RenderInfo const& renderInfo) {
		bwo::FrameBuffer target{ window };

		//Locator<Timer>::ref().newTiming("Render");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT);

		this->uiBackgroundRenderer.render(renderInfo.uiRenderInfo, 0, renderInfo.cameraInfo);

		this->textRenderer.render(renderInfo.textRenderInfo, Locator<Fonts>::ref(), 0, renderInfo.cameraInfo);

		if (misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal()) {
			//this->debugRenderer.render(0, renderInfo);
		}
		//Locator<Timer>::ref().endTiming("Render");

		//Locator<Timer>::ref().newTiming("Swap Buffers");
		glfwSwapBuffers(window);
		//Locator<Timer>::ref().endTiming("Swap Buffers");
	}
}
