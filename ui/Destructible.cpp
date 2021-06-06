#include "Destructible.h"

#include <game/player/PlayerInfo.h>

namespace ui
{
	Destructible::Destructible() {
		this->addGlobalBind({ CONTROL::KEY::EVERY_TICK }, [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		{
			if (static_cast<Destructible*>(self_)->destruct) {
				return BIND::RESULT::CLOSE;
			}
			else {
				return BIND::RESULT::CONTINUE;
			}
		});
	}
}