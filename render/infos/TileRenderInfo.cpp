#include "infos/TileRenderInfo.h"

void render::TileRenderInfo::push(glm::vec2 p, int32_t rot, int32_t tex) {
	this->pos.push_back(p);
	this->rotation.push_back(rot);
	this->texture.push_back(tex);
}
