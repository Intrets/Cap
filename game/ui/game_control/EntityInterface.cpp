#include "ui/game_control/EntityInterface.h"

#include <render/infos/RenderInfo.h>

#include <ui/State.h>
#include <ui/Constructer.h>
#include <ui/TextDisplay.h>

#include <game/GameObject.h>
#include <game/player/PlayerInfo.h>
#include <game/GameState.h>

#include <mem/Global.h>

#include <misc/Timer.h>

namespace game
{
	int32_t EntityInterface::addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		renderInfo.highlightRenderInfo.addBox(
			glm::vec2(this->selected),
			glm::vec2(this->selected) + 1.0f,
			colors::alpha(colors::green, 80)
		);

		if (this->highlight) {
			renderInfo.highlightRenderInfo.addBox(
				this->highlight.value(),
				this->highlight.value() + 1.0f,
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
				auto maybeObject = playerInfo.gameState.world->get(glm::floor(mousePos));
				if (!maybeObject.has_value()) {
					static_cast<game::EntityInterface*>(self_)->highlight = std::nullopt;
				}
				else {
					static_cast<game::EntityInterface*>(self_)->highlight = maybeObject.value().get<GamePosition>().getInterpolatedPosition(playerInfo.gameState.tick);
				}

				return ui::BIND::RESULT::CONTINUE;
			}
		);

		this->addGameWorldBind(
			{ ui::CONTROL::KEY::ACTION0 },
			[this](PlayerInfo& playerInfo, ui::Base* self_) -> ui::CallBackBindResult {
				auto mousePos = playerInfo.uiState.getCursorPositionWorld();
				static_cast<game::EntityInterface*>(self_)->selected = glm::floor(mousePos);

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
							std::stringstream stream;
							serial::Serializer serializer;
							serializer.writeStream = &stream;

							if (this->selected.x >= 0 && this->selected.x < WORLD_SIZE && this->selected.y >= 0 && this->selected.y < WORLD_SIZE) {
								serializer.print(playerInfo.gameState.world->grid[this->selected.x][this->selected.y]);
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