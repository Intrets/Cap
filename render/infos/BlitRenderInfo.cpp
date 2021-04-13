#include "infos/BlitRenderInfo.h"

namespace render
{
	std::vector<SingleBlitRenderInfo> const& BlitRenderInfo::getData() const {
		return this->data;
	}

	std::vector<SingleBlitRenderInfo>& BlitRenderInfo::getDataMut() {
		return this->data;
	}

	void BlitRenderInfo::addBlitInfo(glm::vec4 q, glm::vec4 w, int32_t r) {
		this->data.push_back({ q, w, r });
	}

	void BlitRenderInfo::addBlitInfo(SingleBlitRenderInfo const& info) {
		this->data.push_back(info);
	}

	std::vector<SinglePixelRenderInfo> const& PixelRenderInfo::getData() const {
		return this->data;
	}

	std::vector<SinglePixelRenderInfo>& PixelRenderInfo::getDataMut() {
		return this->data;
	}
	void PixelRenderInfo::addBlitInfo(SinglePixelRenderInfo const& info) {
		this->data.push_back(info);
	}
	void PixelRenderInfo::addBlitInfo(glm::vec4 q, glm::vec4 w, glm::vec2 a, glm::vec2 b, int32_t r) {
		this->data.push_back({ q, w, a, b, r });
	}
}
