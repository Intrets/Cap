#include "Invisible.h"

namespace ui
{
	Invisible::Invisible(Handle self) {
		this->selfHandle = self;
	}

	ScreenRectangle Invisible::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = newScreenRectangle;
		return this->screenRectangle;
	}
}