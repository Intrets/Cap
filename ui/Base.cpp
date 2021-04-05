#include "Base.h"

#include <misc/Rectangle.h>
#include <game/player/PlayerInfo.h>

#include <ui/State.h>
#include <ui/ControlState.h>

namespace ui
{
	ScreenRectangle const& Base::getScreenRectangle() const {
		return this->screenRectangle;
	}

	TYPE Base::getUIOType() {
		return TYPE::UNSPECIFIED;
	}

	Handle Base::getSelfHandle() {
		return this->selfHandle;
	}

	void BaseMulti::clear() {
		this->elements.clear();
	}

	void BaseMulti::addElement(UniqueReference<Base, Base> element) {
		this->elements.push_back(std::move(element));
	}

	void Base::activate() {
		this->active = true;
	}

	void Base::deactivate() {
		this->active = false;
	}

	void BaseMulti::translate(glm::vec2 p) {
		this->screenRectangle.translate(p);
		for (auto& element : this->elements) {
			element.get()->translate(p);
		}
	}

	void BaseMulti::setScreenPixels(glm::ivec2 px) {
		this->screenRectangle.setPixelSize(px);
		for (auto& element : this->elements) {
			element.get()->setScreenPixels(px);
		}
	}

	ScreenRectangle BaseMulti::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = newScreenRectangle;

		for (auto& element : this->elements) {
			element.get()->updateSize(this->screenRectangle);
		}

		return this->screenRectangle;
	}

	bool Base::contains(glm::vec2 p) const {
		return this->screenRectangle.contains(p);
	}

	void Base::addGlobalBind(BindControl bindControl, CallBack callBack) {
		this->globalBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void Base::addFocussedBind(BindControl bindControl, CallBack callBack) {
		this->focussedBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void Base::addOnHoverBind(BindControl bindControl, CallBack callBack) {
		this->onHoverBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void Base::addActiveBind(BindControl bindControl, CallBack callBack) {
		this->activeBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void Base::addGameWorldBind(BindControl bindControl, CallBack callBack) {
		this->gameWorldBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void Base::addGlobalBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addGlobalBind(control, callBack);
		}
	}

	void Base::addFocussedBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addFocussedBind(control, callBack);
		}
	}

	void Base::addOnHoverBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addOnHoverBind(control, callBack);
		}
	}

	void Base::addActiveBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addActiveBind(control, callBack);
		}
	}

	void Base::addGameWorldBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addGameWorldBind(control, callBack);
		}
	}

	CallBackBindResult Base::runGlobalBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;

		for (auto& [control, bind] : this->globalBinds) {
			if (playerInfo.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(playerInfo, this);
				sumResult |= bindResult;
				if (bindResult & BIND::RESULT::CONSUME) {
					playerInfo.controlState.consumeBufferControl(control.control);
				}
				if (sumResult & BIND::RESULT::STOP) {
					return sumResult;
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult Base::runFocussedBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;

		for (auto& [control, bind] : this->focussedBinds) {
			if (playerInfo.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(playerInfo, this);
				if (bindResult & BIND::RESULT::CONSUME) {
					playerInfo.controlState.consumeBufferControl(control.control);
				}
				sumResult |= bindResult;
				if (sumResult & BIND::RESULT::STOP) {
					return sumResult;
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult Base::runOnHoverBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;

		for (auto& [control, bind] : onHoverBinds) {
			if (playerInfo.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(playerInfo, this);
				sumResult |= bindResult;
				if (bindResult & BIND::RESULT::CONSUME) {
					playerInfo.controlState.consumeBufferControl(control.control);
				}
				if (sumResult & BIND::RESULT::STOP) {
					return sumResult;
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult Base::runActiveBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;

		if (this->active) {
			for (auto& [control, bind] : activeBinds) {
				if (playerInfo.controlState.activated(control)) {
					CallBackBindResult bindResult = bind(playerInfo, this);
					sumResult |= bindResult;
					if (bindResult & BIND::RESULT::CONSUME) {
						playerInfo.controlState.consumeBufferControl(control.control);
					}
					if (sumResult & BIND::RESULT::STOP) {
						return sumResult;
					}
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult Base::runGameWorldBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;

		for (auto& [control, bind] : this->gameWorldBinds) {
			if (playerInfo.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(playerInfo, this);
				sumResult |= bindResult;
				if (bindResult & BIND::RESULT::CONSUME) {
					playerInfo.controlState.consumeBufferControl(control.control);
				}
				if (sumResult & BIND::RESULT::STOP) {
					return sumResult;
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult BaseMulti::runGlobalBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runGlobalBinds(playerInfo);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->Base::runGlobalBinds(playerInfo);
	}

	CallBackBindResult BaseMulti::runFocussedBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runFocussedBinds(playerInfo);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->Base::runFocussedBinds(playerInfo);
	}

	CallBackBindResult BaseMulti::runOnHoverBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;
		if (!this->screenRectangle.contains(playerInfo.uiState.getCursor())) {
			return sumResult;
		}

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runOnHoverBinds(playerInfo);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->Base::runOnHoverBinds(playerInfo);
	}

	CallBackBindResult BaseMulti::runActiveBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runActiveBinds(playerInfo);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->Base::runActiveBinds(playerInfo);
	}

	CallBackBindResult BaseMulti::runGameWorldBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runGameWorldBinds(playerInfo);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->Base::runGameWorldBinds(playerInfo);
	}

	int32_t BaseMulti::addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		int32_t maxDepth = 0;
		for (auto& element : this->elements) {
			maxDepth = glm::max(maxDepth, element.get()->addRenderInfo(gameState, renderInfo, depth));
		}
		return 1 + maxDepth;
	}

	void BaseSingle::addElement(UniqueReference<Base, Base> element) {
		assert(main.isNull());
		this->main = std::move(element);
	}

	void BaseSingle::translate(glm::vec2 p) {
		assert(this->main.isNotNull());
		this->screenRectangle.translate(p);
		this->main.get()->translate(p);
	}

	void BaseSingle::setScreenPixels(glm::ivec2 px) {
		assert(this->main.isNotNull());
		this->screenRectangle.setPixelSize(px);
		this->main.get()->setScreenPixels(px);
	}

	ScreenRectangle BaseSingle::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = this->main.get()->updateSize(newScreenRectangle);
		return this->screenRectangle;
	}

	CallBackBindResult BaseSingle::runGlobalBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = this->main.get()->runGlobalBinds(playerInfo);
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->Base::runGlobalBinds(playerInfo);
	}

	CallBackBindResult BaseSingle::runFocussedBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = this->main.get()->runFocussedBinds(playerInfo);
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->Base::runFocussedBinds(playerInfo);
	}

	CallBackBindResult BaseSingle::runOnHoverBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = 0;
		if (!this->screenRectangle.contains(playerInfo.uiState.getCursor())) {
			return sumResult;
		}

		sumResult = this->main.get()->runOnHoverBinds(playerInfo);

		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->Base::runOnHoverBinds(playerInfo);
	}

	CallBackBindResult BaseSingle::runActiveBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = this->main.get()->runActiveBinds(playerInfo);
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->Base::runActiveBinds(playerInfo);
	}

	CallBackBindResult BaseSingle::runGameWorldBinds(PlayerInfo& playerInfo) {
		CallBackBindResult sumResult = this->main.get()->runGameWorldBinds(playerInfo);
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->Base::runGameWorldBinds(playerInfo);
	}

	int32_t BaseSingle::addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		assert(main.isNotNull());
		return this->main.get()->addRenderInfo(gameState, renderInfo, depth);
	}

	void BaseEnd::addElement(UniqueReference<Base, Base> element) {
		assert(0);
	}

	void BaseEnd::translate(glm::vec2 p) {
		this->screenRectangle.translate(p);
	}

	void BaseEnd::setScreenPixels(glm::ivec2 px) {
		this->screenRectangle.setPixelSize(px);
	}
}
