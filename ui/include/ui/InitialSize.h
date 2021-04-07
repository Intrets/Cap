#pragma once

#include "Base.h"

namespace ui
{
	class InitialSize : public BaseSingle
	{
	private:
		Rect rectangle;
		bool initialized = false;

	public:
		InitialSize(Handle self);
		InitialSize(Handle self, Rect rect);
		virtual ~InitialSize() = default;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	};
}