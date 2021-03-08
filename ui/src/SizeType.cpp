#include "SizeType.h"

namespace ui
{
	SizeType::SizeType(SIZETYPE t, int32_t val) {
		this->type = t;
		assert(t == SIZETYPE::STATIC_PX || t == SIZETYPE::PX);
		this->px = val;
	}

	SizeType::SizeType(SIZETYPE t, float val) {
		this->type = t;
		assert(t != SIZETYPE::STATIC_PX);
		this->absolute_height = val;
	}

	float SizeType::getWidth(ScreenRectangle screenRectangle) const {
		float widthS;
		auto pixelSize = screenRectangle.getPixelSize();
		switch (this->type) {
			case SIZETYPE::PX:
				widthS = Option<OPTION::UI_SCALE, float>::getVal() * 2.0f * static_cast<float>(this->px) / pixelSize.x;
				break;
			case SIZETYPE::FH:
				widthS = Option<OPTION::UI_SCALE, float>::getVal() * this->relative * 2.0f * 17.0f / pixelSize.x;
				break;
			case SIZETYPE::STATIC_PX:
				widthS = 2 * static_cast<float>(this->px) / pixelSize.x;
				break;
			case SIZETYPE::ABSOLUTE_HEIGHT:
				widthS = (this->absolute_height * pixelSize.y) / pixelSize.x;
				break;
			case SIZETYPE::ABSOLUTE_WIDTH:
				widthS = this->absolute_width;
				break;
			case SIZETYPE::RELATIVE_HEIGHT:
				widthS = (screenRectangle.getHeight() * this->relative_height * pixelSize.y) / pixelSize.x;
				break;
			case SIZETYPE::RELATIVE_WIDTH:
				widthS = screenRectangle.getWidth() * this->relative_width;
				break;
			default:
				widthS = screenRectangle.getWidth();
				break;
		}
		return widthS;
	}

	float SizeType::getHeight(ScreenRectangle screenRectangle) const {
		float heightS;
		auto pixelSize = screenRectangle.getPixelSize();
		switch (type) {
			case SIZETYPE::PX:
				heightS = Option<OPTION::UI_SCALE, float>::getVal() * 2.0f * static_cast<float>(this->px) / pixelSize.y;
				break;
			case SIZETYPE::FH:
				heightS = Option<OPTION::UI_SCALE, float>::getVal() * this->relative * 2.0f * 17.0f / pixelSize.y;
				break;
			case SIZETYPE::STATIC_PX:
				heightS = 2 * static_cast<float>(this->px) / pixelSize.y;
				break;
			case SIZETYPE::ABSOLUTE_HEIGHT:
				heightS = this->absolute_height;
				break;
			case SIZETYPE::ABSOLUTE_WIDTH:
				heightS = (this->absolute_width * pixelSize.x) / pixelSize.y;
				break;
			case SIZETYPE::RELATIVE_HEIGHT:
				heightS = screenRectangle.getHeight() * this->relative_height;
				break;
			case SIZETYPE::RELATIVE_WIDTH:
				heightS = (screenRectangle.getWidth() * this->relative_width * pixelSize.x) / pixelSize.y;
				break;
			default:
				heightS = screenRectangle.getHeight();
				break;
		}
		return heightS;
	}
}