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
		this->VAO.gen(4);

		static const GLfloat g_quad_vertex_buffer_data[] = {
			0.0f,  0.0f,
			1.0f,  0.0f,
			0.0f,  1.0f,
			0.0f,  1.0f,
			1.0f,  0.0f,
			1.0f,  1.0f,
		};

		this->quad.setRaw(sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data);
		glVertexAttribPointer(
			0,                  // attribute
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*) 0           // array buffer offset
		);
		glVertexAttribDivisor(0, 0);

		this->infos.bind(GL_ARRAY_BUFFER);

		constexpr int32_t stride = sizeof(SingleBlitRenderInfo);
		size_t offset = 0;

		glVertexAttribPointer(
			1,                                // attribute
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			stride,							  // stride
			(void*) offset                    // array buffer offset
		);
		glVertexAttribDivisor(1, 1);

		offset += sizeof(decltype(SingleBlitRenderInfo::quad));

		glVertexAttribPointer(
			2,                                // attribute
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			stride,							  // stride
			(void*) offset                    // array buffer offset
		);
		glVertexAttribDivisor(2, 1);

		offset += sizeof(decltype(SingleBlitRenderInfo::world));

		glVertexAttribIPointer(
			3,                                // attribute
			1,                                // size
			GL_INT,                           // type
			stride,							  // stride
			(void*) offset                    // array buffer offset
		);
		glVertexAttribDivisor(3, 1);

		offset += sizeof(decltype(SingleBlitRenderInfo::rotation));

		assert(offset == stride);

		this->VAO.unbind();

		bwo::ArrayBuffer<glm::vec2> quadBuffer{ bwo::BufferHint::STATIC_DRAW };
		bwo::ArrayBuffer<SingleBlitRenderInfo> blitBuffer{ bwo::BufferHint::STREAM_DRAW };

		bwo::VertexArrayObject3<
			bwo::Group<
				glm::vec2,
				bwo::TTuple2<float, 2, 0>
			>,
			bwo::Group<
				SingleBlitRenderInfo,
				bwo::TTuple2<float, 4, 1>,
				bwo::TTuple2<float, 4, 1>,
				bwo::TTuple2<int32_t, 1, 1>
			>> test3 ( quadBuffer, blitBuffer);

		bwo::VertexArrayObject3<
			bwo::Group<
				glm::vec2,
				bwo::TTuple2<float, 2, 0>
			>,
			bwo::Group<
				SingleBlitRenderInfo,
				bwo::TTuple2<float, 4, 1>,
				bwo::TTuple2<float, 4, 1>,
				bwo::TTuple2<int32_t, 1, 1>
			>> test2 ( quadBuffer, blitBuffer);

	}

	BlitRenderer::~BlitRenderer() {
	}
}