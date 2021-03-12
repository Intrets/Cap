#pragma once

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
	// Player& player;
	game::GameState& gameState;
	ui::ControlState& controlState;
	ui::State& uiState;
};
