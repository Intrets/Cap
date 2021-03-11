#include "Proxy.h"

namespace ui
{
	Proxy::Proxy(Handle self) {
		this->selfHandle = self;
	}

	UniqueReference<Base, Base> Proxy::getMain() {
		return std::move(this->main);
	}
}
