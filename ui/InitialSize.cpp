#include "initialsize.h"

ui::InitialSize::InitialSize(Handle self) {
	this->selfHandle = self;
}

ui::InitialSize::InitialSize(Handle self, Rect rect) {
	this->selfHandle = self;
	this->rectangle = rect;
}

ScreenRectangle ui::InitialSize::updateSize(ScreenRectangle newScreenRectangle) {
	if (!this->initialized) {
		this->initialized = true;

		auto bot = newScreenRectangle.getBottomLeft();
		auto top = bot;
		auto size = newScreenRectangle.getSize();

		bot += glm::ivec2(glm::round(glm::vec2(size) * this->rectangle.getBottomLeft()));
		top += glm::ivec2(glm::round(glm::vec2(size) * this->rectangle.getTopRight()));

		this->main.get()->screenRectangle.setBottomLeft(bot);
		this->main.get()->screenRectangle.setTopRight(top);
	}

	ScreenRectangle r = this->main.get()->getScreenRectangle();

	if (r.getPixelSize().x != 0 && r.getPixelSize().y != 0) {
		glm::vec2 bot = pixelToNormal(r.getBottomLeft(), r.getPixelSize());
		glm::vec2 top = pixelToNormal(r.getTopRight(), r.getPixelSize());

		glm::ivec2 newBot = normalToPixel(bot, newScreenRectangle.getPixelSize());
		glm::ivec2 newTop = normalToPixel(top, newScreenRectangle.getPixelSize());

		r.setBottomLeft(newBot);
		r.setTopRight(newTop);
	}

	r.setPixelSize(newScreenRectangle.getPixelSize());
	this->main.get()->updateSize(r);

	this->screenRectangle = this->main.get()->getScreenRectangle();

	return this->screenRectangle;
}
