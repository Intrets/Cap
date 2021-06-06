#pragma once

#include "Base.h"
#include "SizeType.h"

namespace ui
{
	struct PadType
	{
		PAD::TYPE type;

		SizeType size;

		SizeType const& getSizeType() const;
		PadType& operator=(PAD::TYPE const& t);
		PadType& operator=(SizeType const& s);

		PadType();
		PadType(PAD::TYPE t);
		PadType(SizeType s);
	};

	class Pad : public BaseSingle
	{
	public:
		friend class Hotbar;

		PadType top;
		PadType bottom;
		PadType left;
		PadType right;

	public:
		Pad() = default;
		virtual ~Pad() = default;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

		virtual TYPE getUIOType() override;
	};
}
