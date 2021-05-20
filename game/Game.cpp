#include "Game.h"

namespace game
{
	WeakObject EverythingS::make() {
		WeakObject res{ this->indirectionMap.size(), this };
		this->indirectionMap.push_back({ this });
		return res;
	}
	bool EverythingS::indirection::contains(SignatureType& sig) {
		return (this->signature & sig) == sig;
	}
}
