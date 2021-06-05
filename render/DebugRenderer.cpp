#include "DebugRenderer.h"

#include "GLStateWrapper.h"
#include "infos/CameraInfo.h"
#include "infos/DebugRenderInfo.h"

#include <mem/Locator.h>
#include <misc/Option.h>

void render::DebugRenderer::render(
	DebugRenderInfo const& info,
	bwo::FrameBuffer& target,
	glm::ivec4 viewport,
	CameraInfo const& cameraInfo) {

	this->VAO.bind();
	this->program.use();

	float scale = misc::Option<misc::OPTION::CL_VIEWPORTSCALE, float>::getVal();

	if (!info.world.lines.lines.empty()) {
		auto config = ogs::DebugLineConfiguration();
		Locator<ogs::State>::ref().setState(config);

		this->VP.set(cameraInfo.VP);
		this->pointSize.set(1.0f);

		this->points.set(info.world.lines.lines);

		target.draw(viewport, [&]() {
			glDrawArrays(
				GL_LINES,
				0,
				static_cast<GLsizei>(info.world.lines.lines.size())
			); }
		);
	}
	if (!info.world.points.points.empty()) {
		auto config = ogs::DebugPointConfiguration();

		Locator<ogs::State>::ref().setState(config);

		this->VP.set(cameraInfo.VP);
		this->pointSize.set(200.0f / scale);

		this->points.set(info.world.points.points);

		target.draw(viewport, [&]() {
			glDrawArrays(
				GL_POINTS,
				0,
				static_cast<GLsizei>(info.world.points.points.size())
			); }
		);
	}
	if (!info.screen.lines.lines.empty()) {
		auto config = ogs::DebugLineConfiguration();
		Locator<ogs::State>::ref().setState(config);

		this->VP.set(glm::mat4(1.0f));
		this->pointSize.set(1.0f);

		this->points.set(info.screen.lines.lines);

		target.draw(viewport, [&]() {
			glDrawArrays(
				GL_LINES,
				0,
				static_cast<GLsizei>(info.screen.lines.lines.size())
			); }
		);
	}
	if (!info.screen.points.points.empty()) {
		auto config = ogs::DebugPointConfiguration();

		Locator<ogs::State>::ref().setState(config);

		this->VP.set(glm::mat4(10.0f));
		this->pointSize.set(1.0f);

		this->points.set(info.screen.points.points);

		target.draw(viewport, [&]() {
			glDrawArrays(
				GL_POINTS,
				0,
				static_cast<GLsizei>(info.screen.points.points.size())
			); }
		);
	}

	this->VAO.unbind();
}
