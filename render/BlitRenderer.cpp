#include "BlitRenderer.h"

#include <mem/Locator.h>
#include "GLStateWrapper.h"

namespace render
{
	void BlitRenderer::render(
		ogs::Configuration const& config,
		BlitRenderInfo const& blitInfos,
		bwo::FrameBuffer& target,
		glm::ivec4 viewport,
		bwo::Texture2D const& texture,
		std::optional<float> depth_,
		bool flipUVvertical,
		glm::vec2 offset_,
		std::optional<glm::vec4> maybeColor) {

		if (blitInfos.getData().size() == 0) {
			return;
		}

		Locator<ogs::State>::ref().setState(config);

		this->VAO.bind();
		this->program.use();

		this->offset.set(offset_);

		if (maybeColor.has_value()) {
			this->color.set(maybeColor.value());
		}
		else {
			this->color.set(glm::vec4(1.0f));
		}

		if (depth_.has_value()) {
			this->depth.set(depth_.value());
		}
		else {
			this->depth.set(0.0f);
		}

		if (flipUVvertical) {
			this->UVflip.set({ -1.0f, 1.0f });
		}
		else {
			this->UVflip.set({ 1.0f, 0.0f });
		}

		this->texture_t.set(texture);

		this->infos.set(blitInfos.getData());

		target.draw(
			viewport,
			[&]()
			{
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(blitInfos.getData().size()));
			});

		this->VAO.unbind();
	}

	void BlitRenderer::render(
		ogs::Configuration const& config,
		SingleBlitRenderInfo const& info,
		bwo::FrameBuffer& target,
		glm::ivec4 viewport,
		bwo::Texture2D const& texture,
		std::optional<float> depth_,
		bool flipUVvertical,
		std::optional<glm::vec4> maybeColor) {

		BlitRenderInfo infos;
		infos.addBlitInfo(info);
		this->render(config, infos, target, viewport, texture, depth_, flipUVvertical, glm::vec2(0.0f), maybeColor);
	}

	BlitRenderer::BlitRenderer() {
		static const GLfloat g_quad_vertex_buffer_data[] = {
			0.0f,  0.0f,
			1.0f,  0.0f,
			0.0f,  1.0f,
			0.0f,  1.0f,
			1.0f,  0.0f,
			1.0f,  1.0f,
		};

		this->quad.setRaw(sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data);
	}

	BlitRenderer::~BlitRenderer() {
	}
}