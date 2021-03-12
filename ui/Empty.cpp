#include "Empty.h"

namespace ui
{
	Empty::Empty(Handle self_) {
		this->selfHandle = self_;
	}

	ScreenRectangle Empty::updateSize(ScreenRectangle newScreenRectangle) {
		return newScreenRectangle;
	}

	void Empty::addElement(UniqueReference<Base, Base> element) {
	}

	void Empty::translate(glm::vec2 p) {
	}

	void Empty::setScreenPixels(glm::ivec2 px) {
	}

	int32_t Empty::addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		return depth;
	}
}