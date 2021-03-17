#pragma once

#include <wglm/glm.hpp>

class Player;

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
	// Player& player;
	game::GameState& gameState;
	ui::ControlState& controlState;
	ui::State& uiState;
};
