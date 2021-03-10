#pragma once

#include "Base.h"

namespace render
{
	struct RenderInfo;
}

namespace ui
{
	class Grid : public BaseMulti
	{
	private:
		glm::ivec2 size;

	public:
		Grid() = default;
		Grid(Handle self, glm::ivec2 size_);
		virtual	~Grid() = default;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

		virtual int32_t addRenderInfo(GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) override;

		virtual TYPE getUIOType() override;
	};
}