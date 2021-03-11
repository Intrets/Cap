#include "ColoredBackground.h"

#include <render/infos/RenderInfo.h>

namespace ui
{
	ColoredBackground::ColoredBackground(Handle self) {
		this->selfHandle = self;
	}

	int32_t ColoredBackground::addRenderInfo(GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		depth = this->BaseSingle::addRenderInfo(gameState, renderInfo, depth);
		renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft(), this->screenRectangle.getTopRight(), this->color, depth++);
		return depth;
	}
}