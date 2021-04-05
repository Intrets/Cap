#pragma once

#include <misc/Rectangle.h>

#include "Enums.h"

namespace ui
{
	struct SizeType
	{
		SIZETYPE type;

		union
		{
			int32_t px;
			float relative;
			float absolute_height;
			float absolute_width;
			float relative_height;
			float relative_width;
		};

		SizeType(SIZETYPE t, int32_t val);
		SizeType(SIZETYPE t, float val);

		int32_t getWidth(ScreenRectangle screenRectangle) const;
		int32_t getHeight(ScreenRectangle screenRectangle) const;
	};
 }