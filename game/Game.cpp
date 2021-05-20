#include "Game.h"

namespace game
{
	WeakObject EverythingS::make() {
		WeakObject res{ this->indirectionMap.size(), this };
		this->indirectionMap.push_back({});
		return res;
	}
}
