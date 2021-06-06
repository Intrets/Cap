#include "Button.h"

#include <game/player/PlayerInfo.h>
#include <render/infos/RenderInfo.h>
#include <mem/Locator.h>

#include "State.h"

#include <SDL_mixer.h>


namespace ui
{
	void Button::setColor(glm::vec4 c) {
		this->color = c;
	}

	void Button::setOnPress(CallBack f) {
		this->onPress = f;
	}

	void Button::setOnRelease(CallBack f) {
		this->onRelease = f;
	}

	void Button::setShrinkToFit(bool b) {
		this->shrinkToFit = b;
	}

	bool Button::isDown() {
		return this->down;
	}

	void Button::dePress() {
		this->down = false;
	}

	glm::ivec2 const& Button::getMousePressOffset() const {
		return this->mousePressOffset;
	}

	Button::Button(Handle self) {
		this->onPress = [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		{
			return BIND::RESULT::CONTINUE;
		};

		this->onRelease = [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		{
			return BIND::RESULT::CONTINUE;
		};

		this->selfHandle = self;

		this->addOnHoverBind({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL, CONTROL::STATE::PRESSED },
			[](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
			{
				if (!self_->isActive()) {
					auto channel = Mix_PlayChannel(-1, Locator<std::vector<Mix_Chunk*>>::ref()[1], 0);
					Mix_Volume(channel, 40);
				}
				self_->activate();
				return BIND::RESULT::CONSUME;
			});

		this->addGlobalBind({ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED }, [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
			{
				if (!self_->getScreenRectangle().contains(playerInfo.uiState.getCursor()) ||
					!playerInfo.controlState.activated({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL })) {
					self_->deactivate();
				}
				return BIND::RESULT::CONTINUE;
			});

		this->addOnHoverBind({ CONTROL::KEY::ACTION0, CONTROL::STATE::PRESSED }, [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
			{
				auto self = static_cast<Button*>(self_);
				self->down = true;
				self->mousePressOffset = playerInfo.uiState.getCursor() - self->getScreenRectangle().getTopLeft();
				auto channel = Mix_PlayChannel(-1, Locator<std::vector<Mix_Chunk*>>::ref()[0], 0);
				Mix_Volume(channel, 128);
				return self->onPress(playerInfo, self_) | BIND::RESULT::FOCUS | BIND::RESULT::CONSUME;
			});

		this->addGlobalBind({ CONTROL::KEY::ACTION0, CONTROL::STATE::RELEASED }, [this](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
			{
				if (this->down) {
					this->down = false;

					if (self_->getScreenRectangle().contains(playerInfo.uiState.getCursor())) {
						return this->onRelease(playerInfo, self_) | BIND::RESULT::CONTINUE;
					}
					else {
						return BIND::RESULT::CONTINUE;
					}
				}
				else {
					return BIND::RESULT::CONTINUE;
				}
			});
	}

	ScreenRectangle Button::updateSize(ScreenRectangle newScreenRectangle) {
		auto temp = this->main.get()->updateSize(newScreenRectangle);
		if (this->shrinkToFit) {
			newScreenRectangle = temp;
		}
		this->screenRectangle = newScreenRectangle;
		return this->screenRectangle;
	}

	int32_t Button::addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		glm::vec4 c = this->color;
		if (down) {
			c = COLORS::DARKEN(c);
		}
		else if (this->active) {
			c = COLORS::LIGHTEN(c);
		}

		depth = this->main.get()->addRenderInfo(gameState, renderInfo, depth + 1);
		renderInfo.uiRenderInfo.addPixelRectangle(
			this->screenRectangle.getPixelSize(),
			this->screenRectangle.getBottomLeft(),
			this->screenRectangle.getTopRight(),
			c,
			depth++
		);
		return depth;
	}
}