#include "infos/UIRenderInfo.h"

#include <wglm/glm.hpp>
#include <misc/Rectangle.h>

namespace render
{
	std::vector<SingleUIRenderInfo> const& UIRenderInfo::getData() const {
		return this->data;
	}

	void UIRenderInfo::addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color) {
		this->data.push_back({
			pos2 - pos1,
			glm::vec3(pos1, 0),
			color }
		);
	}

	void UIRenderInfo::addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color, int32_t depth) {
		this->data.push_back({
			pos2 - pos1,
			glm::vec3(pos1, 1 - 1 / static_cast<float>(1 + depth)),
			color }
		);
	}

	void UIRenderInfo::addRectangle(Rect& rec, glm::vec4 color) {
		this->data.push_back({
			rec.getTopRight() - rec.getBottomLeft(),
			glm::vec3(rec.getBottomLeft(), 0),
			color }
		);
	}
}