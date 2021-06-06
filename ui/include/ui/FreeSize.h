#pragma once

#include "Base.h"

namespace ui
{
	class FreeSize : public BaseSingle
	{
	public:
		FreeSize() = default;
		virtual ~FreeSize() = default;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	};
}