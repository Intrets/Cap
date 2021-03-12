#pragma once

class Player;
class GameState;

namespace ui
{
	class State;
	class ControlState;
}

struct PlayerInfo
{
	Player& player;
	GameState& gameState;
	ui::ControlState& controlState;
	ui::State& uiState;
};
