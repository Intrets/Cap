#include "BlitRenderer.h"

#include "GLEnabler.h"

#include "../shaders/Blit.vert.inc"
#include "../shaders/Blit.frag.inc"

namespace render
{
	void BlitRenderer::render(BlitRenderInfo const& blitInfos,
							  bwo::FrameBuffer& target,
							  glm::ivec4 viewport,
							  bwo::Texture2D const& texture,
							  std::optional<float> depth_,
							  bool flipUVvertical,
							  glm::vec2 offset_,
							  std::optional<glm::vec4> maybeColor) {
		if (blitInfos.data.size() == 0) {
			return;
		}

		this->VAO.bind();
		this->program.use();

		GLEnabler glEnabler;
		glEnabler.enable(GL_BLEND);

		this->offset.set(offset_);

		if (maybeColor.has_value()) {
			this->color.set(maybeColor.value());
		}
		else {
			this->color.set(glm::vec4(1.0f));
		}

		if (depth_.has_value()) {
			glEnabler.enable(GL_DEPTH_TEST);
			this->depth.set(depth_.value());
		}
		else {
			glEnabler.disable(GL_DEPTH_TEST);
			this->depth.set(0.0f);
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (flipUVvertical) {
			this->UVflip.set(-1.0f);
		}
		else {
			this->UVflip.set(1.0f);
		}

		this->texture_t.set(texture);

		this->infos.set(blitInfos.data);

		target.draw(
			{ viewport[2], viewport[3] },
			viewport,
			[&]()
		{
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(blitInfos.data.size()));
		});

		this->VAO.unbind();
	}

	void BlitRenderer::render(SingleBlitRenderInfo const& info,
							  bwo::FrameBuffer& target,
							  glm::ivec4 viewport,
							  bwo::Texture2D const& texture,
							  std::optional<float> depth_,
							  bool flipUVvertical,
							  std::optional<glm::vec4> maybeColor) {
		BlitRenderInfo infos;
		infos.data.push_back(info);
		this->render(infos, target, viewport, texture, depth_, flipUVvertical, glm::vec2(0.0f), maybeColor);
	}

	BlitRenderer::BlitRenderer() :
		program(Blit_vert, Blit_frag, "BlitRenderer"),
		UVflip("UVflip", this->program),
		texture_t("texture_t", this->program, 0),
		depth("depth", this->program),
		offset("offset", this->program),
		color("c", this->program) {

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