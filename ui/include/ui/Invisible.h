#pragma once

#include "Base.h"

namespace ui
{
	class Invisible : public BaseMulti
	{
	public:
		Invisible(Handle self);
		virtual ~Invisible() = default;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	};
}