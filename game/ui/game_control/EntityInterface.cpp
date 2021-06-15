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

		if (this->highlight) {
			renderInfo.highlightRenderInfo.addBox(
				this->highlight.value(),
				this->highlight.value() + 1,
				colors::alpha(colors::yellow, 80)
			);
		}

		return depth;
	}

	EntityInterface::EntityInterface() {
		this->addGameWorldBind(
			{ ui::CONTROL::KEY::MOUSE_POS_CHANGED },
			[](PlayerInfo& playerInfo, ui::Base* self_) {
				auto mousePos = playerInfo.uiState.getCursorPositionWorld();
				auto const maybeObject = playerInfo.gameState.world->get(mousePos);
				if (!maybeObject.has_value()) {
					static_cast<game::EntityInterface*>(self_)->highlight = std::nullopt;
				}
				else {
					static_cast<game::EntityInterface*>(self_)->highlight = mousePos;
				}

				return ui::BIND::RESULT::CONTINUE;
			}
		);

		this->addGameWorldBind(
			{ ui::CONTROL::KEY::ACTION0 },
			[this](PlayerInfo& playerInfo, ui::Base* self_) -> ui::CallBackBindResult {
				auto mousePos = playerInfo.uiState.getCursorPositionWorld();
				auto const maybeObject = playerInfo.gameState.world->get(mousePos);
				if (!maybeObject.has_value()) {
					return ui::BIND::RESULT::CONTINUE;
				}
				static_cast<game::EntityInterface*>(self_)->controlled.set(maybeObject.value());

				playerInfo.uiState.addNamedUI("entity info", [this]() {
					ui::Global::push();

					ui::window(
						"entity info",
						{ 0.85f, 0.7f, 1.0f, 1.0f },
						ui::WINDOW::TYPE::CLOSE |
						ui::WINDOW::TYPE::MOVE |
						ui::WINDOW::TYPE::RESIZE
					);

					auto text = ui::textDisplayMulti("");
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
							else {
								static_cast<ui::TextDisplay*>(self_)->setText(
									"Nothing selected"
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

		this->addGameWorldBind(
			{ ui::CONTROL::KEY::X, ui::CONTROL::STATE::DOWN },
			[](PlayerInfo& playerInfo, ui::Base* self_) {
				auto mousePos = playerInfo.uiState.getCursorPositionWorld();
				auto const maybeObject = playerInfo.gameState.world->get(mousePos);
				if (!maybeObject.has_value()) {
					return ui::BIND::RESULT::CONTINUE;
				}
				auto _ = playerInfo.gameState.removeFromWorld(maybeObject.value());

				return ui::BIND::RESULT::CONTINUE;
			}
		);
	}
}