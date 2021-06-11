#include "ui/game_control/EntityInterface.h"

#include <render/infos/RenderInfo.h>

#include <ui/State.h>
#include <ui/Constructer.h>
#include <ui/TextDisplay.h>

#include <game/GameObject.h>
#include <game/player/PlayerInfo.h>
#include <game/GameState.h>

#include <mem/Locator.h>

#include <misc/Timer.h>

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

	EntityInterface::EntityInterface() {
		this->addGameWorldBind(
			{ ui::CONTROL::KEY::C },
			[this](PlayerInfo& playerInfo, ui::Base* self_) -> ui::CallBackBindResult {
				auto mousePos = playerInfo.uiState.getCursorPositionWorld();
				auto const index = playerInfo.gameState.world->get(mousePos);
				if (auto maybeObject = playerInfo.gameState.everything.maybeGetFromIndex(index)) {
					static_cast<game::EntityInterface*>(self_)->controlled.set(maybeObject.value());
				}

				playerInfo.uiState.addNamedUI("entity info", [this]() {
					ui::Global::push();

					ui::window(
						"entity info",
						{ 0.85f, 0.7f, 1.0f, 1.0f },
						ui::WINDOW::TYPE::CLOSE |
						ui::WINDOW::TYPE::MOVE |
						ui::WINDOW::TYPE::RESIZE
					);

					auto text = ui::textDisplaySingle("");
					text.get()->addGlobalBind(
						{ ui::CONTROL::KEY::EVERY_TICK },
						[this](PlayerInfo& playerInfo, ui::Base* self_) {
							if (this->controlled.isQualified()) {
								std::stringstream stream;
								serial::Serializer serializer;
								serializer.writeStream = &stream;

								serializer.print(this->controlled);
								static_cast<ui::TextDisplay*>(self_)->setText(
									stream.str()
								);
							}
							return ui::BIND::RESULT::CONTINUE;
						}
					);

					return ui::Global::pop();
					});

				return ui::BIND::RESULT::CONTINUE;
			}
		);
	}
}