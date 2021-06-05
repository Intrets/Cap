#include "ui/game_control/EntityInterface.h"

#include <render/infos/RenderInfo.h>

#include <ui/State.h>

#include <game/GameObject.h>
#include <game/player/PlayerInfo.h>
#include <game/GameState.h>

namespace game
{
	int32_t EntityInterface::addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		if (this->controlled.isQualified()) {
			if (auto posMaybe = this->controlled->getMaybe<GamePosition>()) {
				auto& pos = *posMaybe.value();
				renderInfo.highlightRenderInfo.addBox(pos.pos, pos.pos + 1);
			}
		}
		else {
			renderInfo.highlightRenderInfo.addBox({ 0,0 }, { 1,1 });
		}
		return depth;
	}

	EntityInterface::EntityInterface(Handle self) {
		this->selfHandle = self;

		this->addGlobalBind(
			{ ui::CONTROL::KEY::C },
			[](PlayerInfo& playerInfo, ui::Base* self_) -> ui::CallBackBindResult {
				auto mousePos = playerInfo.uiState.getCursorPositionWorld();
				auto const index = playerInfo.gameState.world->get(mousePos);
				if (auto maybeObject = playerInfo.gameState.everything.maybeGetFromIndex(index)) {
					static_cast<game::EntityInterface*>(self_)->controlled.set(maybeObject.value());
				}

				return ui::BIND::RESULT::CONTINUE;
			}
		);
	}
}