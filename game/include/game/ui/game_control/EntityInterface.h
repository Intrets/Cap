#pragma once

#include <ui/Base.h>

#include <game/Game.h>


class EntityInterface : public ui::BaseInvisibleEnd
{
public:
	game::QualifiedObject selected;



	virtual int32_t addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) override;

	EntityInterface() = default;
	virtual ~EntityInterface() = default;
};
