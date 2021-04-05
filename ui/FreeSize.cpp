#include "FreeSize.h"

namespace ui
{
	FreeSize::FreeSize(Handle self) {
		this->selfHandle = self;
	}

	ScreenRectangle FreeSize::updateSize(ScreenRectangle newScreenRectangle) {
		ScreenRectangle r = this->main.get()->getScreenRectangle();

		// TODO: is this needed?
		//if (r.getPixelSize().x != 0 && r.getPixelSize().y != 0) {
		//	glm::vec2 ratio = glm::vec2(r.getPixelSize()) / glm::vec2(newScreenRectangle.getPixelSize());
		//	glm::vec2 size = r.getAbsSize() * ratio;
		//	r.setSize(size);
		//}

		r.setPixelSize(newScreenRectangle.getPixelSize());
		this->main.get()->updateSize(r);

		this->screenRectangle = this->main.get()->getScreenRectangle();

		return this->screenRectangle;
	}
}