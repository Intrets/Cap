#include "List.h"

namespace ui
{
	List::List(DIR dir) : direction(dir) {
	}

	ScreenRectangle List::updateSize(ScreenRectangle newScreenRectangle) {
		ScreenRectangle rec = newScreenRectangle;

		switch (this->direction) {
			case DIR::LEFT:
				for (auto& element : this->elements) {
					ScreenRectangle newRec = element.get()->updateSize(rec);
					glm::vec2 p = rec.getTopRight() - newRec.getTopRight();
					element.get()->translate(p);
					rec.translateRight(-newRec.getWidth());
				}
				newScreenRectangle.setTopLeft(rec.getTopRight());
				break;
			case DIR::RIGHT:
				for (auto& element : this->elements) {
					ScreenRectangle newRec = element.get()->updateSize(rec);
					glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
					element.get()->translate(p);
					rec.translateLeft(newRec.getWidth());
				}
				newScreenRectangle.setTopRight(rec.getTopLeft());
				break;
			case DIR::UP:
				for (auto& element : this->elements) {
					ScreenRectangle newRec = element.get()->updateSize(rec);
					glm::vec2 p = rec.getBottomLeft() - newRec.getBottomLeft();
					element.get()->translate(p);
					rec.translateBot(newRec.getHeight());
				}
				newScreenRectangle.setTopRight(rec.getBottomRight());
				break;
			case DIR::DOWN:
				for (auto& element : this->elements) {
					ScreenRectangle newRec = element.get()->updateSize(rec);
					glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
					element.get()->translate(p);
					rec.translateTop(-newRec.getHeight());
				}
				newScreenRectangle.setBottomLeft(rec.getTopLeft());
				break;

			case DIR::LEFT_REVERSE:
				for (auto it = this->elements.rbegin(); it != this->elements.rend(); ++it) {
					auto& element = *it;
					ScreenRectangle newRec = element.get()->updateSize(rec);
					glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
					element.get()->translate(p);
					rec.translateLeft(newRec.getWidth());
				}
				newScreenRectangle.setTopRight(rec.getTopLeft());
				break;
			case DIR::RIGHT_REVERSE:
				for (auto it = this->elements.rbegin(); it != this->elements.rend(); ++it) {
					auto& element = *it;
					ScreenRectangle newRec = element.get()->updateSize(rec);
					glm::vec2 p = rec.getTopRight() - newRec.getTopRight();
					element.get()->translate(p);
					rec.translateRight(-newRec.getWidth());
				}
				newScreenRectangle.setTopLeft(rec.getTopRight());
				break;
			case DIR::UP_REVERSE:
				for (auto it = this->elements.rbegin(); it != this->elements.rend(); ++it) {
					auto& element = *it;
					ScreenRectangle newRec = element.get()->updateSize(rec);
					glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
					element.get()->translate(p);
					rec.translateTop(-newRec.getHeight());
				}
				newScreenRectangle.setBottomLeft(rec.getTopLeft());
				break;
			case DIR::DOWN_REVERSE:
				for (auto it = this->elements.rbegin(); it != this->elements.rend(); ++it) {
					auto& element = *it;
					ScreenRectangle newRec = element.get()->updateSize(rec);
					glm::vec2 p = rec.getBottomLeft() - newRec.getBottomLeft();
					element.get()->translate(p);
					rec.translateBot(newRec.getHeight());
				}
				newScreenRectangle.setTopRight(rec.getBottomRight());
				break;
			default:
				break;
		}

		this->screenRectangle = newScreenRectangle;

		return this->screenRectangle;
	}

	TYPE List::getUIOType() {
		return TYPE::LIST;
	}
}
