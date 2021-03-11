#include "Hideable.h"

#include <render/infos/RenderInfo.h>
#include <game/player/PlayerInfo.h>

namespace ui
{
	Hideable::Hideable(Handle self) {
		this->selfHandle = self;
	}

	void Hideable::hide() {
		this->hidden = true;
	}

	void Hideable::show() {
		this->hidden = false;
		if (this->focusOnShow) {
			this->shouldFocus = true;
		}
	}

	CallBackBindResult Hideable::runGlobalBinds(PlayerInfo& playerInfo) {
		int32_t res = 0;
		if (this->shouldFocus) {
			res |= BIND::RESULT::FOCUS;
			this->shouldFocus = false;
		}

		if (!this->hidden) {
			res |= this->BaseSingle::runGlobalBinds(playerInfo);
			if (res & BIND::RESULT::HIDE) {
				res &= ~BIND::RESULT::HIDE;
				this->hidden = true;
			}
			return res;
		}
		else {
			return res | BIND::RESULT::CONTINUE;
		}
	}

	CallBackBindResult Hideable::runFocussedBinds(PlayerInfo& playerInfo) {
		if (!this->hidden) {
			int32_t res = this->BaseSingle::runFocussedBinds(playerInfo);
			if (res & BIND::RESULT::HIDE) {
				res &= ~BIND::RESULT::HIDE;
				this->hidden = true;
			}
			return res;
		}
		else {
			return BIND::RESULT::CONTINUE;
		}
	}

	CallBackBindResult Hideable::runOnHoverBinds(PlayerInfo& playerInfo) {
		if (!this->hidden) {
			int32_t res = this->BaseSingle::runOnHoverBinds(playerInfo);
			if (res & BIND::RESULT::HIDE) {
				res &= ~BIND::RESULT::HIDE;
				this->hidden = true;
			}
			return res;
		}
		else {
			return BIND::RESULT::CONTINUE;
		}
	}

	CallBackBindResult Hideable::runActiveBinds(PlayerInfo& playerInfo) {
		if (!this->hidden) {
			int32_t res = this->BaseSingle::runActiveBinds(playerInfo);
			if (res & BIND::RESULT::HIDE) {
				res &= ~BIND::RESULT::HIDE;
				this->hidden = true;
			}
			return res;
		}
		else {
			return BIND::RESULT::CONTINUE;
		}
	}

	CallBackBindResult Hideable::runGameWorldBinds(PlayerInfo& playerInfo) {
		if (!this->hidden) {
			int32_t res = this->BaseSingle::runGameWorldBinds(playerInfo);
			if (res & BIND::RESULT::HIDE) {
				res &= ~BIND::RESULT::HIDE;
				this->hidden = true;
			}
			return res;
		}
		else {
			return BIND::RESULT::CONTINUE;
		}
	}

	int32_t Hideable::addRenderInfo(GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		if (!this->hidden) {
			return this->BaseSingle::addRenderInfo(gameState, renderInfo, depth);
		}
		else {
			return depth;
		}
	}
}
