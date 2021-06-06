#include "Invisible.h"

namespace ui
{
	ScreenRectangle Invisible::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = newScreenRectangle;
		return this->screenRectangle;
	}
}