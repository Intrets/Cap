#include "AnchoredProxy.h"

#include <game/player/PlayerInfo.h>

#include "State.h"
#include "Constructer.h"
#include "Destructible.h"

namespace ui
{
	void AnchoredProxy::closeProxy() {
		if (this->proxyBase.isValid()) {
			this->destructible.get()->destruct = true;
		}
	}

	bool AnchoredProxy::hasProxy() {
		return this->proxyBase.isValid();
	}

	void AnchoredProxy::setProxy(UniqueReference<Base, Base> ref, State& uiState) {
		this->closeProxy();

		Global::push();

		free();
		this->destructible = ui::destructible();
		this->proxyBase.set(this->destructible);

		auto ptr = makeEnd(std::move(ref));
		ptr.get()->addGlobalBind({ CONTROL::KEY::ACTION0 }, [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		{
			if (!self_->getScreenRectangle().contains(playerInfo.uiState.getCursorPositionScreen())) {
				return BIND::RESULT::CLOSE;
			}
			else {
				return BIND::RESULT::CONTINUE;
			}
		});

		ptr.get()->addOnHoverBind({ CONTROL::KEY::ACTION0 }, [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		{
			return BIND::RESULT::CONSUME;
		});

		uiState.addUI(Global::pop());
	}

	ScreenRectangle AnchoredProxy::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = newScreenRectangle;
		if (auto proxyBaseRef = this->proxyBase.getRef()) {
			const float scale = 10.0f;
			switch (this->alignment) {
				case ALIGNMENT::TOP:
					newScreenRectangle.setBot(-scale);
					break;
				case ALIGNMENT::BOTTOM:
					newScreenRectangle.setTop(scale);
					break;
				case ALIGNMENT::LEFT:
					newScreenRectangle.setRight(scale);
					break;
				case ALIGNMENT::RIGHT:
					newScreenRectangle.setLeft(-scale);
					break;
				case ALIGNMENT::TOPRIGHT:
					newScreenRectangle.setBottomLeft({ -scale, -scale });
					break;
				case ALIGNMENT::TOPLEFT:
					newScreenRectangle.setBottomRight({ scale, -scale });
					break;
				case ALIGNMENT::BOTTOMLEFT:
					newScreenRectangle.setTopRight({ scale, scale });
					break;
				case ALIGNMENT::BOTTOMRIGHT:
					newScreenRectangle.setTopLeft({ -scale,scale });
					break;
				default:
					break;
			}
			proxyBaseRef.get()->updateSize(newScreenRectangle);
		}
		return this->screenRectangle;
	}

	int32_t AnchoredProxy::addRenderInfo(game::GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		return depth;
	}

	AnchoredProxy::AnchoredProxy(Handle self) {
		this->selfHandle = self;
	}

	AnchoredProxy::~AnchoredProxy() {
		if (this->proxyBase.isValid()) {
			this->destructible.get()->destruct = true;
		}
	}
}