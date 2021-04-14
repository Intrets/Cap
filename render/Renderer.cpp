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

#include <misc/PathManager.h>
#include <render/BlitRenderer.h>
#include <render/PixelRenderer.h>

namespace render
{
	void Renderer::render(GLFWwindow* window, RenderInfo const& renderInfo) {
		bwo::FrameBuffer target{ window };

		Locator<misc::Timer>::ref().newTiming("Render");

		target.clear({ 0.5f, 0.5f, 0.5f, 1.0f }, true);

		//auto config = ogs::WorldTileConfiguration();

		//Locator<BlitRendererArrayTexture>::ref().render(
		//	config,
		//	renderInfo.tileRenderInfo,
		//	target,
		//	{ 0, 0, renderInfo.cameraInfo.x, renderInfo.cameraInfo.y },
		//	Locator<BlockIDTextures>::ref().getTextureArray(),
		//	std::nullopt,
		//	renderInfo.cameraInfo.VP
		//);

		//target.clearDepth();

		//this->uiBackgroundRenderer.render(renderInfo.uiRenderInfo, 0, renderInfo.cameraInfo);

		//this->textRenderer.render(renderInfo.textRenderInfo,
		//						  Locator<Fonts>::ref(),
		//						  target);

		//if (misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal()) {
		//	//this->debugRenderer.render(0, renderInfo);
		//}
		//Locator<misc::Timer>::ref().endTiming("Render");


		auto abomination = Locator<misc::PathManager>::ref().LoadTexture2DP("abomination.dds");
		//glBindTexture(GL_TEXTURE_2D, abomination.ID);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		auto abominationNoFilter = Locator<misc::PathManager>::ref().LoadTexture2DP("abomination.dds");
		glBindTexture(GL_TEXTURE_2D, abominationNoFilter.ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		auto tex = bwo::Texture2DHelper::makeNoFiltering({ 2000,2000 });
		auto texTarget = bwo::FrameBuffer();
		texTarget.bindTextureColor(0, tex, 0);

		// [-1, 1]
		glm::vec2 pixelSize = glm::vec2(2.0f) / glm::vec2(tex.size);
		// total unscaled size of the quad
		auto size = pixelSize * glm::vec2(abomination.size);
		// total scaled size of the quad
		glm::vec2 renderScale = glm::vec2(2.321f);
		auto renderSize = size * renderScale;

		static int32_t o = 0;
		o++;

		auto config = ogs::UIBackground();
		render::PixelRenderInfo info;
		info.addBlitInfo(
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(-1.0f + (o * pixelSize.x / 5.0f), -1.0f + (o * pixelSize.x / 5.0f), renderSize.x, renderSize.y),
			glm::vec2(1.0f) / glm::vec2(abomination.size),
			1.0f / glm::vec2(abomination.size) / renderScale,
			0
		);

		render::BlitRenderInfo info3;
		info3.addBlitInfo(
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(-1.0f + renderSize.x + (o * pixelSize.x / 5.0f), -1.0f + (o * pixelSize.x / 5.0f), renderSize.x, renderSize.y),
			0
		);

		Locator<render::BlitRenderer>::ref().render(
			config,
			info3,
			texTarget,
			{ 0,0, texTarget.size.x, texTarget.size.y },
			abominationNoFilter,
			std::nullopt,
			true,
			glm::vec2(0, 0)
		);

		Locator<render::PixelRenderer>::ref().render(
			config,
			info,
			texTarget,
			{ 0,0, texTarget.size.x, texTarget.size.y },
			abomination,
			std::nullopt,
			true,
			glm::vec2(0, 0)
		);

		glm::vec2 pixelSize2 = glm::vec2(2.0f) / glm::vec2(target.size);
		auto size2 = pixelSize2 * glm::vec2(tex.size) * 1.0f;

		render::BlitRenderInfo info2;
		info2.addBlitInfo(
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(-1.0f, -1.0f, size2.x, size2.y),
			0
		);

		Locator<BlitRenderer>::ref().render(
			config,
			info2,
			target,
			{ 0,0,target.size.x,target.size.y },
			tex,
			std::nullopt,
			false,
			glm::vec2(0, 0)
		);

		Locator<misc::Timer>::ref().newTiming("Swap Buffers");
		glfwSwapBuffers(window);
		Locator<misc::Timer>::ref().endTiming("Swap Buffers");


		glfwWindowShouldClose(window);
	}
}
