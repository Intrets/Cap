#include "infos/BlitArrayRenderInfo.h"

namespace render
{
	std::vector<SingleBlitArrayRenderInfo> const& BlitArrayRenderInfo::getData() const {
		return this->data;
	}

	void BlitArrayRenderInfo::addBlitInfo(glm::vec4 w, int32_t r, int32_t l) {
		this->data.push_back({ w, r, l });
	}
}
