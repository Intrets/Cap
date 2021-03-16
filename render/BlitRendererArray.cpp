#include "BlitRendererArray.h"

#include "GLEnabler.h"

#include "shaders/BlitArray.vert.inc"
#include "shaders/BlitArray.frag.inc"

namespace render
{
	void BlitRendererArrayTexture::render(BlitArrayRenderInfo const& blitInfos,
										  bwo::FrameBuffer& target,
										  glm::ivec4 viewport,
										  bwo::Texture2DArray const& texture,
										  std::optional<float> depth_,
										  glm::mat4 VP_) {
		this->VAO.bind();
		this->program.use();

		GLEnabler glEnabler;
		glEnabler.enable(GL_BLEND);

		if (depth_.has_value()) {
			glEnabler.enable(GL_DEPTH_TEST);
			this->depth.set(depth_.value());
		}
		else {
			glEnabler.disable(GL_DEPTH_TEST);
			this->depth.set(0.0f);
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		this->texture_t.set(texture);
		this->VP.set(VP_);

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

	BlitRendererArrayTexture::BlitRendererArrayTexture() :
		program(BlitArray_vert, BlitArray_frag, "BlitRendererArrayTexture"),
		texture_t("texture_t", this->program, 0),
		depth("depth", this->program),
		VP("VP", this->program) {

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
}