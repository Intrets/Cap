#pragma once

#include "Base.h"

namespace ui
{
	class Empty : public Base
	{
	public:
		Empty() = default;
		virtual ~Empty() = default;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

		virtual void addElement(UniqueReference<Base, Base> element) override;
		virtual void translate(glm::vec2 p) override;
		virtual void setScreenPixels(glm::ivec2 px) override;
		virtual int32_t addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) override;
	};
}