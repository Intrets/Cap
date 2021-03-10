#include "BlitRenderer.h"

#include "GLEnabler.h"

namespace render
{
	void BlitRenderer::render(std::vector<glm::vec4> const& uv, std::vector<glm::vec4> const& world, GLuint target, glm::ivec4 viewport, GLuint texture, std::optional<float> depth_, bool flipUVvertical, glm::vec2 offset_, std::optional<glm::vec4> maybeColor) {
		if (uv.size() == 0) {
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

		glBindFramebuffer(GL_FRAMEBUFFER, target);

		if (flipUVvertical) {
			this->UVflip.set(-1.0f);
		}
		else {
			this->UVflip.set(1.0f);
		}

		this->texture_t.set(texture);

		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

		this->UVSource.set(uv);
		this->worldTarget.set(world);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(uv.size()));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		this->VAO.unbind();
	}

	void BlitRenderer::render(glm::vec4 uv, glm::vec4 world, GLuint target, glm::ivec4 viewport, GLuint texture, std::optional<float> depth_, bool flipUVvertical, std::optional<glm::vec4> maybeColor) {
		std::vector uvv{ uv };
		std::vector worldv{ world };
		this->render(uvv, worldv, target, viewport, texture, depth_, flipUVvertical, glm::vec2(0.0f), maybeColor);
	}

	BlitRenderer::BlitRenderer() :
		program("Blit", "BlitRenderer"),
		UVflip("UVflip", this->program),
		texture_t("texture_t", this->program, 0),
		depth("depth", this->program),
		offset("offset", this->program),
		color("c", this->program) {
		this->VAO.gen(3);

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

		this->UVSource.bind(GL_ARRAY_BUFFER);
		glVertexAttribPointer(
			1,                                // attribute
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,								  // stride
			(void*) 0                         // array buffer offset
		);
		glVertexAttribDivisor(1, 1);

		this->worldTarget.bind(GL_ARRAY_BUFFER);
		glVertexAttribPointer(
			2,                                // attribute
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,								  // stride
			(void*) 0                         // array buffer offset
		);
		glVertexAttribDivisor(2, 1);

		this->VAO.unbind();
	}

	BlitRenderer::~BlitRenderer() {
	}
}