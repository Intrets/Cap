#include "BlitRendererArray.h"

#include "GLEnabler.h"

#include "shaders/BlitArray.vert.inc"
#include "shaders/BlitArray.frag.inc"

namespace render
{
	void BlitRendererArrayTexture::render(std::vector<glm::vec2> const& positions,
										  std::vector<int32_t> const& rotations,
										  std::vector<int32_t> const& layers,
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

		this->position.set(positions);
		this->rotation.set(rotations);
		this->layer.set(layers);

		target.draw(
			{ viewport[2], viewport[3] },
			viewport,
			[&]()
		{
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(positions.size()));
		});

		this->VAO.unbind();
	}

	BlitRendererArrayTexture::BlitRendererArrayTexture() :
		program(BlitArray_vert, BlitArray_frag, "BlitRendererArrayTexture"),
		texture_t("texture_t", this->program, 0),
		depth("depth", this->program),
		VP("VP", this->program) {

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
			0,                   // attribute
			2,                   // size
			GL_FLOAT,            // type
			GL_FALSE,            // normalized?
			0,                   // stride
			(void*) 0            // array buffer offset
		);
		glVertexAttribDivisor(0, 0);

		this->rotation.bind(GL_ARRAY_BUFFER);
		glVertexAttribIPointer(
			1,                                // attribute
			1,                                // size
			GL_INT,                           // type
			0,								  // stride
			(void*) 0                         // array buffer offset
		);
		glVertexAttribDivisor(1, 1);

		this->layer.bind(GL_ARRAY_BUFFER);
		glVertexAttribIPointer(
			2,                                // attribute
			1,                                // size
			GL_INT,                           // type
			0,								  // stride
			(void*) 0                         // array buffer offset
		);
		glVertexAttribDivisor(2, 1);

		this->position.bind(GL_ARRAY_BUFFER);
		glVertexAttribPointer(
			3,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,								  // stride
			(void*) 0                         // array buffer offset
		);
		glVertexAttribDivisor(3, 1);

		this->VAO.unbind();
	}
}