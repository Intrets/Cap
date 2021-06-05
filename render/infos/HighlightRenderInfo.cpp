#include "infos/HighlightRenderInfo.h"

void render::HighlightRenderInfo::addBox(glm::vec2 p1, glm::vec2 p2, colors::Color c) {
	this->infos.push_back({ p1, p2 - p1, c });
}
