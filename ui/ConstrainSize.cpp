#include "ConstrainSize.h"

namespace ui
{
	void ConstrainSize::setHeight(SizeType height) {
		assert(!this->maybeHeight.has_value());
		this->maybeHeight = height;
	}

	void ConstrainSize::setWidth(SizeType width) {
		assert(!this->maybeWidth.has_value());
		this->maybeWidth = width;
	}

	void ConstrainSize::setAlignment(ALIGNMENT alignment_) {
		this->alignment = alignment_;
	}

	ConstrainSize::ConstrainSize(Handle self) {
		this->selfHandle = self;
	}

	TYPE ConstrainSize::getUIOType() {
		return TYPE::CONSTRAIN_SIZE;
	}

	ScreenRectangle ConstrainSize::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = newScreenRectangle;

		if (this->maybeWidth) {
			newScreenRectangle.setWidth(this->maybeWidth.value().getWidth(this->screenRectangle));
		}
		if (this->maybeHeight) {
			newScreenRectangle.setHeight(this->maybeHeight.value().getHeight(this->screenRectangle));
		}

		newScreenRectangle = this->main.get()->updateSize(newScreenRectangle);

		glm::vec2 p;
		switch (this->alignment) {
			case ALIGNMENT::TOP:
				p.x += this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
				[[fallthrough]];
			case ALIGNMENT::TOPLEFT:
				p += this->screenRectangle.getTopLeft() - newScreenRectangle.getTopLeft();
				break;

			case ALIGNMENT::RIGHT:
				p.y -= this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
				[[fallthrough]];
			case ALIGNMENT::TOPRIGHT:
				p += this->screenRectangle.getTopRight() - newScreenRectangle.getTopRight();
				break;

			case ALIGNMENT::BOTTOM:
				p.x -= this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
				[[fallthrough]];
			case ALIGNMENT::BOTTOMRIGHT:
				p += this->screenRectangle.getBottomRight() - newScreenRectangle.getBottomRight();
				break;

			case ALIGNMENT::LEFT:
				p.y += this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
				[[fallthrough]];
			case ALIGNMENT::BOTTOMLEFT:
				p += this->screenRectangle.getBottomLeft() - newScreenRectangle.getBottomLeft();
				break;

			case ALIGNMENT::CENTER:
				p.y -= this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
				p.x += this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
				break;

			default:
				break;
		}

		// Aligns to bottom left if the containing element does not fit in newScreenRectangle
		p = glm::max(this->screenRectangle.getBottomLeft(), newScreenRectangle.getBottomLeft() + p) - newScreenRectangle.getBottomLeft();

		this->screenRectangle = newScreenRectangle;
		this->translate(p);

		return this->screenRectangle;
	}
}