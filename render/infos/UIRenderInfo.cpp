#include "infos/UIRenderInfo.h"

#include <wglm/glm.hpp>
#include <misc/Rectangle.h>

namespace render
{
	void UIRenderInfo::addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color) {
		this->scales.push_back(pos2 - pos1);
		this->positions.push_back(glm::vec3(pos1, 0));
		this->colors.push_back(color);
	}

	void UIRenderInfo::addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color, int32_t depth) {
		this->scales.push_back(pos2 - pos1);
		this->positions.push_back(glm::vec3(pos1, 1 - 1 / static_cast<float>(1 + depth)));
		this->colors.push_back(color);
	}

	void UIRenderInfo::addRectangle(Rect& rec, glm::vec4 color) {
		this->scales.push_back(rec.getTopRight() - rec.getBottomLeft());
		this->positions.push_back(glm::vec3(rec.getBottomLeft(), 0));
		this->colors.push_back(color);
	}
}