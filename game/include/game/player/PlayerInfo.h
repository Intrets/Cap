#pragma once

class Player;
class GameState;
class ControlState;
namespace ui
{
	class State;
}

struct PlayerInfo
{
	Player& player;
	GameState& gameState;
	ControlState& controlState;
	ui::State& uiState;

};
