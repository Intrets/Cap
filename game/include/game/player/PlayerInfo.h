#pragma once

#include <wglm/glm.hpp>

#include "game/Game.h"

namespace game
{
	class GameState;
}

namespace ui
{
	class State;
	class ControlState;
}

struct PlayerInfo
{
	glm::vec2 pos;
	game::QualifiedObject controlled;

	game::GameState& gameState;
	ui::ControlState& controlState;
	ui::State& uiState;
};
