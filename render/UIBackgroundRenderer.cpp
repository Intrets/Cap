#include "UIBackgroundRenderer.h"

#include "infos/RenderInfo.h"
#include "infos/CameraInfo.h"

#include <mem/Global.h>

namespace render
{
	void UIBackgroundRenderer::render(
		UIRenderInfo const& renderInfo,
		GLuint target,
		CameraInfo const& cameraInfo) {

		if (renderInfo.getData().empty()) {
			return;
		}

		Global<ogs::State>::ref().setState(ogs::UIBackground());

		this->VAO.bind();
		this->program.use();

		glBindFramebuffer(GL_FRAMEBUFFER, target);
		glViewport(0, 0, cameraInfo.x, cameraInfo.y);

		this->infos.set(renderInfo.getData());

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<int32_t>(renderInfo.getData().size()));
	}

	UIBackgroundRenderer::UIBackgroundRenderer() {
		static const GLfloat g_quad_vertex_buffer_data[] = {
			0.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			1.0f,  1.0f,  0.0f,
		};

		this->quad.setRaw(sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data);
	}

	UIBackgroundRenderer::~UIBackgroundRenderer() {
	}
}