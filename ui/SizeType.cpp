#include "SizeType.h"

#include <wglm/glm.hpp>
#include <misc/Option.h>

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

	int32_t SizeType::getWidth(ScreenRectangle screenRectangle) const {
		int32_t widthS;
		auto pixelSize = screenRectangle.getPixelSize();
		switch (this->type) {
			case SIZETYPE::PX:
			{
				float scale = misc::Option<misc::OPTION::UI_SCALE, float>::getVal();
				widthS = static_cast<int32_t>(glm::ceil(scale * this->px));
			}
			break;
			case SIZETYPE::FH:
			{
				float scale = misc::Option<misc::OPTION::UI_SCALE, float>::getVal();
				widthS = static_cast<int32_t>(glm::ceil(scale * this->relative * 17));
			}
			break;
			case SIZETYPE::STATIC_PX:
				widthS = this->px;
				break;
			case SIZETYPE::ABSOLUTE_HEIGHT:
				widthS = static_cast<int32_t>(glm::round(this->absolute_height * pixelSize.y));
				break;
			case SIZETYPE::ABSOLUTE_WIDTH:
				widthS = static_cast<int32_t>(glm::round(this->absolute_width * pixelSize.x));
				break;
			case SIZETYPE::RELATIVE_HEIGHT:
				widthS = static_cast<int32_t>(static_cast<float>(screenRectangle.getHeight()) * this->relative_height);
				break;
			case SIZETYPE::RELATIVE_WIDTH:
				widthS = static_cast<int32_t>(static_cast<float>(screenRectangle.getWidth()) * this->relative_width);
				break;
			default:
				widthS = screenRectangle.getWidth();
				break;
		}
		return widthS;
	}

	int32_t SizeType::getHeight(ScreenRectangle screenRectangle) const {
		return this->getWidth(screenRectangle);
	}
}