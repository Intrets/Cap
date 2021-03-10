#include "Grid.h"

#include <render/infos/RenderInfo.h>

namespace ui
{
	Grid::Grid(Handle self, glm::ivec2 size_) :
		size(size_) {
		this->selfHandle = self;
	}

	ScreenRectangle Grid::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = newScreenRectangle;

		//glm::vec2 pos = newScreenRectangle.getTopLeft();
		glm::vec2 pos = glm::vec2(0.0f, 0.0f);
		glm::vec2 gridSize = newScreenRectangle.getAbsSize() / glm::vec2(size);

		ScreenRectangle rec = newScreenRectangle;
		rec.setWidth(gridSize.x);
		rec.setHeight(gridSize.y);

		int32_t i = 0;
		for (int32_t y = 0; y < size.y; y++) {
			for (int32_t x = 0; x < size.x; x++) {
				if (i >= this->elements.size()) {
					return newScreenRectangle;
				}
				ScreenRectangle newRect = rec;
				newRect.translate(pos);

				elements[i].get()->updateSize(newRect);

				pos.x += gridSize.x;
				i++;
			}
			pos.x = 0.0f;
			pos.y -= gridSize.y;
		}
		return newScreenRectangle;
	}

	int32_t Grid::addRenderInfo(GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		for (auto& element : this->elements) {
			auto rec = element.get()->getScreenRectangle();
		}

		return this->BaseMulti::addRenderInfo(gameState, renderInfo, depth);
	}

	TYPE Grid::getUIOType() {
		return TYPE::GRID;
	}
}