#pragma once

#include "Base.h"

namespace ui
{
	class Destructible : public BaseSingle
	{
	public:
		bool destruct = false;

		Destructible();
		virtual ~Destructible() = default;
	};
}