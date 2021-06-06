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
		InitialSize() = default;
		InitialSize(Rect rect);
		virtual ~InitialSize() = default;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	};
}