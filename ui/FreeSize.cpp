#include "FreeSize.h"

namespace ui
{
	ScreenRectangle FreeSize::updateSize(ScreenRectangle newScreenRectangle) {
		ScreenRectangle r = this->main.get()->getScreenRectangle();

		r.setPixelSize(newScreenRectangle.getPixelSize());
		this->main.get()->updateSize(r);

		this->screenRectangle = this->main.get()->getScreenRectangle();

		return this->screenRectangle;
	}
}