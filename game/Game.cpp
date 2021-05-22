#include "Game.h"

namespace game
{
	UniqueObject::~UniqueObject() {
		this->proxy->remove(this->index);
	}

	UniqueObject::UniqueObject(WeakObject&& other) {
		this->index = other.index;
		this->proxy = other.proxy;
	}

	UniqueObject::UniqueObject(UniqueObject&& other) {
		this->index = other.index;
		this->proxy = other.proxy;
		other.index = 0;
		other.proxy = nullptr;
	}

	UniqueObject& UniqueObject::operator=(UniqueObject&& other) {
		if (this->proxy != nullptr) {
			this->proxy->remove(this->index);
		}

		this->index = other.index;
		this->proxy = other.proxy;

		return *this;
	}
}
