#include "infos/TextRenderInfo.h"

#include <mem/Locator.h>

#include "Enums.h"
#include "Fonts.h"

namespace render
{
	WindowTextRenderInfo::WindowTextRenderInfo(ScreenRectangle rect, bool lineWrap_, bool clickSupport_) :
		screenRectangle(rect),
		lineWrap(lineWrap_),
		clickSelect(clickSupport_) {
	}

	void WindowTextRenderInfo::addString(FONT font, std::string text) {
		FontInfo& fontInfo = Locator<Fonts>::ref().getFont(font);

		glm::ivec2 sizei = glm::floor(glm::vec2(this->screenRectangle.getPixelSize()) * this->screenRectangle.getAbsSize() / 2.0f);

		for (char c : text) {
			glm::vec2 size = glm::vec2(fontInfo.charSize[static_cast<int32_t>(c)]) / glm::vec2(sizei) * 2.0f;

			glm::vec2 addPos;

			if (this->lineWrap && this->nextPos.x + size.x > 1.0f && c != '\n') {
				this->newLine();
			}

			addPos = this->nextPos;
			addPos.y -= size.y;

			this->laneHeight = glm::max(this->laneHeight, size.y);

			if (this->clickSelect) {
				glm::vec2 vertRange = glm::vec2(addPos.y, addPos.y + size.y);
				glm::vec2 horRange = glm::vec2(addPos.x, addPos.x + size.x);

				auto it = this->lines.find(vertRange);
				if (it == this->lines.end()) {
					it = this->lines.insert({ vertRange, {} }).first;
				}
				it->second[horRange] = static_cast<int32_t>(this->textRenderInfo.data.size());
			}

			this->textRenderInfo.data.push_back({
				fontInfo.charUV[static_cast<int32_t>(c)],
				glm::vec4(addPos, size),
				0 });

			this->nextPos.x += size.x;

			if (c == '\n') {
				this->newLine();
			}

			glm::vec2 addBot = addPos;
			addBot.x += size.x;
			glm::vec2 curSize = glm::abs(addBot - glm::vec2(-1.0f, 1.0f));
			this->renderedSize = glm::max(this->renderedSize, curSize);
		}
	}

	void WindowTextRenderInfo::newLine() {
		this->nextPos.x = -1.0f;
		this->nextPos.y -= this->laneHeight;
	}

	void WindowTextRenderInfo::setDepth(int32_t layer) {
		this->depth = 1 - 1 / static_cast<float>(1 + layer);
	}

	void WindowTextRenderInfo::setDepth(float depth_) {
		this->depth = depth_;
	}

	glm::vec2 WindowTextRenderInfo::getRenderedScreenSize() {
		return this->screenRectangle.getAbsSize() * this->renderedSize / 2.0f;
	}

	std::optional<int32_t> WindowTextRenderInfo::getIndex(glm::vec2 p) {
		auto line = this->lines.find(glm::vec2(p.y));
		if (line == this->lines.end()) {
			return std::nullopt;
		}
		auto index = line->second.find(glm::vec2(p.x));
		if (index == line->second.end()) {
			if (line->second.size() == 0) {
				return std::nullopt;
			}
			else {
				auto t = line->second.end();
				return (--t)->second;
			}
		}
		return index->second;
	}

	std::optional<glm::vec4> WindowTextRenderInfo::getCursorPos(int32_t index) const {
		if (indexInVector(index, this->textRenderInfo.data)) {
			return this->textRenderInfo.data[index].world;
		}
		return std::nullopt;
	}
}