#include "HighlightRenderer.h"

#include <mem/Locator.h>

#include "GLStateWrapper.h"

void render::HighlightRenderer::render(
	HighlightRenderInfo const& highlightInfos,
	bwo::FrameBuffer& target,
	glm::ivec4 viewport,
	glm::mat4 VP_) {

	if (highlightInfos.infos.empty()) {
		return;
	}

	Locator<ogs::State>::ref().setState(ogs::HighlightConfiguration());

	this->VAO.bind();
	this->program.use();

	this->infos.set(highlightInfos.infos);
	this->VP.set(VP_);

	target.draw(
		viewport,
		[&]() {
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(highlightInfos.infos.size()));
		}
	);
}

render::HighlightRenderer::HighlightRenderer() {
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
