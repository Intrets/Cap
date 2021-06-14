#pragma once

#include <ui/Base.h>

#include <game/Game.h>

namespace game
{
	class EntityInterface : public ui::BaseInvisibleEnd
	{
	public:
		game::QualifiedObject controlled;

		std::optional<glm::ivec2> highlight;

		virtual int32_t addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) override;

		EntityInterface();
		virtual ~EntityInterface() = default;
	};
}