#include "Base.h"

// #include "PlayerState.h"

#include <misc/Rectangle.h>

namespace ui
{
	ScreenRectangle const& UIOBase::getScreenRectangle() const {
		return this->screenRectangle;
	}

	TYPE UIOBase::getUIOType() {
		return TYPE::UNSPECIFIED;
	}

	Handle UIOBase::getSelfHandle() {
		return this->selfHandle;
	}

	void UIOBaseMulti::clear() {
		this->elements.clear();
	}

	void UIOBaseMulti::addElement(UniqueReference<UIOBase, UIOBase> element) {
		this->elements.push_back(std::move(element));
	}

	void UIOBase::activate() {
		this->active = true;
	}

	void UIOBase::deactivate() {
		this->active = false;
	}

	void UIOBaseMulti::translate(glm::vec2 p) {
		this->screenRectangle.translate(p);
		for (auto& element : this->elements) {
			element.get()->translate(p);
		}
	}

	void UIOBaseMulti::setScreenPixels(glm::ivec2 px) {
		this->screenRectangle.setPixelSize(px);
		for (auto& element : this->elements) {
			element.get()->setScreenPixels(px);
		}
	}

	ScreenRectangle UIOBaseMulti::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = newScreenRectangle;

		for (auto& element : this->elements) {
			element.get()->updateSize(this->screenRectangle);
		}

		return this->screenRectangle;
	}

	void UIOBase::moveTopLeftTo(glm::vec2 p) {
		translate(p - this->screenRectangle.getTopLeft());
	}

	bool UIOBase::contains(glm::vec2 p) const {
		return this->screenRectangle.contains(p);
	}

	void UIOBase::addGlobalBind(BindControl bindControl, CallBack callBack) {
		this->globalBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void UIOBase::addFocussedBind(BindControl bindControl, CallBack callBack) {
		this->focussedBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void UIOBase::addOnHoverBind(BindControl bindControl, CallBack callBack) {
		this->onHoverBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void UIOBase::addActiveBind(BindControl bindControl, CallBack callBack) {
		this->activeBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void UIOBase::addGameWorldBind(BindControl bindControl, CallBack callBack) {
		this->gameWorldBinds.push_back(std::make_pair(bindControl, callBack));
	}

	void UIOBase::addGlobalBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addGlobalBind(control, callBack);
		}
	}

	void UIOBase::addFocussedBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addFocussedBind(control, callBack);
		}
	}

	void UIOBase::addOnHoverBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addOnHoverBind(control, callBack);
		}
	}

	void UIOBase::addActiveBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addActiveBind(control, callBack);
		}
	}

	void UIOBase::addGameWorldBinds(std::vector<BindControl> const& bindControls, CallBack callBack) {
		for (auto const& control : bindControls) {
			this->addGameWorldBind(control, callBack);
		}
	}

	CallBackBindResult UIOBase::runGlobalBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;

		for (auto& [control, bind] : this->globalBinds) {
			if (playerState.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(playerState, this);
				sumResult |= bindResult;
				if (bindResult & BIND::RESULT::CONSUME) {
					playerState.controlState.consumeBufferControl(control.control);
				}
				if (sumResult & BIND::RESULT::STOP) {
					return sumResult;
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult UIOBase::runFocussedBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;

		for (auto& [control, bind] : this->focussedBinds) {
			if (playerState.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(playerState, this);
				if (bindResult & BIND::RESULT::CONSUME) {
					playerState.controlState.consumeBufferControl(control.control);
				}
				sumResult |= bindResult;
				if (sumResult & BIND::RESULT::STOP) {
					return sumResult;
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult UIOBase::runOnHoverBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;

		for (auto& [control, bind] : onHoverBinds) {
			if (playerState.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(playerState, this);
				sumResult |= bindResult;
				if (bindResult & BIND::RESULT::CONSUME) {
					playerState.controlState.consumeBufferControl(control.control);
				}
				if (sumResult & BIND::RESULT::STOP) {
					return sumResult;
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult UIOBase::runActiveBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;

		if (this->active) {
			for (auto& [control, bind] : activeBinds) {
				if (playerState.controlState.activated(control)) {
					CallBackBindResult bindResult = bind(playerState, this);
					sumResult |= bindResult;
					if (bindResult & BIND::RESULT::CONSUME) {
						playerState.controlState.consumeBufferControl(control.control);
					}
					if (sumResult & BIND::RESULT::STOP) {
						return sumResult;
					}
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult UIOBase::runGameWorldBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;

		for (auto& [control, bind] : this->gameWorldBinds) {
			if (playerState.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(playerState, this);
				sumResult |= bindResult;
				if (bindResult & BIND::RESULT::CONSUME) {
					playerState.controlState.consumeBufferControl(control.control);
				}
				if (sumResult & BIND::RESULT::STOP) {
					return sumResult;
				}
			}
		}

		return sumResult;
	}

	CallBackBindResult UIOBaseMulti::runGlobalBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runGlobalBinds(playerState);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->UIOBase::runGlobalBinds(playerState);
	}

	CallBackBindResult UIOBaseMulti::runFocussedBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runFocussedBinds(playerState);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->UIOBase::runFocussedBinds(playerState);
	}

	CallBackBindResult UIOBaseMulti::runOnHoverBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;
		if (!this->screenRectangle.contains(playerState.uiState.getCursorPositionScreen())) {
			return sumResult;
		}

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runOnHoverBinds(playerState);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->UIOBase::runOnHoverBinds(playerState);
	}

	CallBackBindResult UIOBaseMulti::runActiveBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runActiveBinds(playerState);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->UIOBase::runActiveBinds(playerState);
	}

	CallBackBindResult UIOBaseMulti::runGameWorldBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;

		for (auto& element : this->elements) {
			CallBackBindResult elementResult = element.get()->runGameWorldBinds(playerState);
			sumResult |= elementResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}

		return sumResult | this->UIOBase::runGameWorldBinds(playerState);
	}

	int32_t UIOBaseMulti::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
		int32_t maxDepth = 0;
		for (auto& element : this->elements) {
			maxDepth = glm::max(maxDepth, element.get()->addRenderInfo(gameState, renderInfo, depth));
		}
		return 1 + maxDepth;
	}

	void UIOBaseSingle::addElement(UniqueReference<UIOBase, UIOBase> element) {
		assert(main.isNull());
		this->main = std::move(element);
	}

	void UIOBaseSingle::translate(glm::vec2 p) {
		assert(this->main.isNotNull());
		this->screenRectangle.translate(p);
		this->main.get()->translate(p);
	}

	void UIOBaseSingle::setScreenPixels(glm::ivec2 px) {
		assert(this->main.isNotNull());
		this->screenRectangle.setPixelSize(px);
		this->main.get()->setScreenPixels(px);
	}

	ScreenRectangle UIOBaseSingle::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = this->main.get()->updateSize(newScreenRectangle);
		return this->screenRectangle;
	}

	CallBackBindResult UIOBaseSingle::runGlobalBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = this->main.get()->runGlobalBinds(playerState);
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->UIOBase::runGlobalBinds(playerState);
	}

	CallBackBindResult UIOBaseSingle::runFocussedBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = this->main.get()->runFocussedBinds(playerState);
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->UIOBase::runFocussedBinds(playerState);
	}

	CallBackBindResult UIOBaseSingle::runOnHoverBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = 0;
		if (!this->screenRectangle.contains(playerState.uiState.getCursorPositionScreen())) {
			return sumResult;
		}

		sumResult = this->main.get()->runOnHoverBinds(playerState);

		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->UIOBase::runOnHoverBinds(playerState);
	}

	CallBackBindResult UIOBaseSingle::runActiveBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = this->main.get()->runActiveBinds(playerState);
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->UIOBase::runActiveBinds(playerState);
	}

	CallBackBindResult UIOBaseSingle::runGameWorldBinds(PlayerState& playerState) {
		CallBackBindResult sumResult = this->main.get()->runGameWorldBinds(playerState);
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}

		return sumResult | this->UIOBase::runGameWorldBinds(playerState);
	}

	int32_t UIOBaseSingle::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
		assert(main.isNotNull());
		return this->main.get()->addRenderInfo(gameState, renderInfo, depth);
	}

	void UIOBaseEnd::addElement(UniqueReference<UIOBase, UIOBase> element) {
		assert(0);
	}

	void UIOBaseEnd::translate(glm::vec2 p) {
		this->screenRectangle.translate(p);
	}

	void UIOBaseEnd::setScreenPixels(glm::ivec2 px) {
		this->screenRectangle.setPixelSize(px);
	}
}
