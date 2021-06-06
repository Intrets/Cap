#include "Proxy.h"

namespace ui
{
	UniqueReference<Base, Base> Proxy::getMain() {
		return std::move(this->main);
	}
}
