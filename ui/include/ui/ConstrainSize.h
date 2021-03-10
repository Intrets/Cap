#pragma once

#include <optional>

#include "Base.h"
#include "SizeType.h"

namespace ui
{
	class ConstrainSize : public BaseSingle
	{
	private:
		std::optional<SizeType> maybeHeight;
		std::optional<SizeType> maybeWidth;

		ALIGNMENT alignment = ALIGNMENT::CENTER;

	public:
		void setHeight(SizeType height);
		void setWidth(SizeType width);
		void setAlignment(ALIGNMENT alignment);

		ConstrainSize(Handle self);
		~ConstrainSize() = default;

		virtual TYPE getUIOType() override;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	};
}
