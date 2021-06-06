#include "Pad.h"

namespace ui
{
	ScreenRectangle Pad::updateSize(ScreenRectangle newScreenRectangle) {
		this->screenRectangle = newScreenRectangle;

		if (this->right.type == PAD::TYPE::SIZETYPE) {
			newScreenRectangle.translateRight(-this->right.getSizeType().getWidth(newScreenRectangle));
		}
		if (this->top.type == PAD::TYPE::SIZETYPE) {
			newScreenRectangle.translateTop(-this->top.getSizeType().getHeight(newScreenRectangle));
		}
		if (this->left.type == PAD::TYPE::SIZETYPE) {
			newScreenRectangle.translateLeft(this->left.getSizeType().getWidth(newScreenRectangle));
		}
		if (this->bottom.type == PAD::TYPE::SIZETYPE) {
			newScreenRectangle.translateBot(this->bottom.getSizeType().getHeight(newScreenRectangle));
		}

		newScreenRectangle = main.get()->updateSize(newScreenRectangle);

		if (this->right.type == PAD::TYPE::SHRINK) {
			this->screenRectangle.setRight(newScreenRectangle.getRight());
		}
		if (this->top.type == PAD::TYPE::SHRINK) {
			this->screenRectangle.setTop(newScreenRectangle.getTop());
		}
		if (this->left.type == PAD::TYPE::SHRINK) {
			this->screenRectangle.setLeft(newScreenRectangle.getLeft());
		}
		if (this->bottom.type == PAD::TYPE::SHRINK) {
			this->screenRectangle.setBot(newScreenRectangle.getBot());
		}

		return this->screenRectangle;
	}

	TYPE Pad::getUIOType() {
		return TYPE::PAD;
	}

	SizeType const& PadType::getSizeType() const {
		assert(this->type == PAD::TYPE::SIZETYPE);
		return this->size;
	}

	PadType& PadType::operator=(PAD::TYPE const& t) {
		this->type = t;
		return *this;
	}

	PadType& PadType::operator=(SizeType const& s) {
		this->type = PAD::TYPE::SIZETYPE;
		this->size = s;
		return *this;
	}

	PadType::PadType() : PadType(PAD::TYPE::SHRINK) {
	}

	PadType::PadType(PAD::TYPE t) : size(SIZETYPE::ABSOLUTE_HEIGHT, -1.0f), type(t) {
	}

	PadType::PadType(SizeType s) : size(s), type(PAD::TYPE::SIZETYPE) {

	}
}