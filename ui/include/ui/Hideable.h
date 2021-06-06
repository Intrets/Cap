#pragma once

#include "Base.h"

struct PlayerInfo;

namespace render
{
	struct RenderInfo;
}

namespace ui
{
	class Hideable : public BaseSingle
	{
	private:
		bool hidden = false;

		// if the parent container should be brought to focus
		bool shouldFocus = false;

		// if the Hideable should bring the parent back to focus on being shown again
		bool focusOnShow = true;

	public:
		Hideable() = default;
		virtual ~Hideable() = default;

		void hide();
		void show();

		virtual CallBackBindResult runGlobalBinds(PlayerInfo& playerInfo) override;
		virtual CallBackBindResult runFocussedBinds(PlayerInfo& playerInfo) override;
		virtual CallBackBindResult runOnHoverBinds(PlayerInfo& playerInfo) override;
		virtual CallBackBindResult runActiveBinds(PlayerInfo& playerInfo) override;
		virtual CallBackBindResult runGameWorldBinds(PlayerInfo& playerInfo) override;

		virtual int32_t addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) override;
	};
}
