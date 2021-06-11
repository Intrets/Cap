#pragma once

#include <ui/Base.h>

#include <game/Game.h>

namespace game
{
	class EntityInterface : public ui::BaseInvisibleEnd
	{
	public:
		game::QualifiedObject controlled;

		virtual int32_t addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) override;

		EntityInterface();
		virtual ~EntityInterface() = default;
	};
}