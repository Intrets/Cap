#pragma once

#include "Base.h"

namespace ui
{
	class List : public BaseMulti
	{
	private:
		DIR direction;

	public:
		List(Handle self, DIR dir);
		virtual ~List() = default;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

		virtual TYPE getUIOType() override;
	};
}
