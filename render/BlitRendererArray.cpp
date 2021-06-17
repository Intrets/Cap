#include "BlitRendererArray.h"

#include <mem/Global.h>

#include "GLStateWrapper.h"

namespace render
{
	void BlitRendererArrayTexture::render(
		ogs::Configuration const& config,
		BlitArrayRenderInfo const& blitInfos,
		bwo::FrameBuffer& target,
		glm::ivec4 viewport,
		bwo::Texture2DArray const& texture,
		std::optional<float> depth_,
		glm::mat4 VP_) {

		this->VAO.bind();
		this->program.use();

		Global<ogs::State>()->setState(config);

		if (depth_.has_value()) {
			this->depth.set(depth_.value());
		}
		else {
			this->depth.set(0.0f);
		}

		this->texture_t.set(texture);
		this->VP.set(VP_);

		this->infos.set(blitInfos.getData());

		target.draw(
			viewport,
			[&]()
			{
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(blitInfos.getData().size()));
			});

		this->VAO.unbind();
	}

	BlitRendererArrayTexture::BlitRendererArrayTexture() {
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