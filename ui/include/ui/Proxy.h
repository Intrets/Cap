#pragma once

#include "Base.h"

namespace ui
{
	class Proxy : public BaseSingle
	{
	public:
		Proxy(Handle self);
		virtual ~Proxy() = default;

		[[nodiscard]]
		UniqueReference<Base, Base> getMain();
	};
}