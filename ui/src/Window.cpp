#include "Window.h"

#include <render/infos/RenderInfo.h>
#include <render/Colors.h>

namespace ui
{
	Window::Window(Handle self) {
		this->selfHandle = self;
	}

	int32_t Window::addRenderInfo(GameState& gameState, render::RenderInfo& renderInfo, int32_t depth) {
		glm::vec2 px = glm::vec2(1.0f) / glm::vec2(this->screenRectangle.getPixelSize());
		if (this->minimized) {
			depth = this->topBar.get()->addRenderInfo(gameState, renderInfo, depth++);
			renderInfo.uiRenderInfo.addRectangle(this->topBar.get()->getScreenRectangle().getBottomLeft() - px, this->topBar.get()->getScreenRectangle().getTopRight() + 2.0f * px, COLORS::UI::WINDOWBACKGROUND, depth++);
			return depth;
		}
		else {
			depth = this->BaseMulti::addRenderInfo(gameState, renderInfo, depth++);
			renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft() - px, this->screenRectangle.getTopRight() + 2.0f * px, COLORS::UI::WINDOWBACKGROUND, depth++);
			return depth;
		}
	}

	CallBackBindResult Window::runGlobalBinds(PlayerInfo& playerInfo) {
		if (this->minimized) {
			return this->topBar.get()->runGlobalBinds(playerInfo);
		}
		else {
			return this->BaseMulti::runGlobalBinds(playerInfo);
		}
	}

	CallBackBindResult Window::runGameWorldBinds(PlayerInfo& playerInfo) {
		if (this->minimized) {
			return this->topBar.get()->runGameWorldBinds(playerInfo);
		}
		else {
			return this->BaseMulti::runGameWorldBinds(playerInfo);
		}
	}

	void Window::addElement(UniqueReference<Base, Base> element) {
		assert(this->main == nullptr);
		this->main = element;
		this->addElementMulti(std::move(element));
	}

	void Window::addElementMulti(UniqueReference<Base, Base> element) {
		this->BaseMulti::addElement(std::move(element));
	}

	CallBackBindResult Window::runFocussedBinds(PlayerInfo& playerInfo) {
		if (this->minimized) {
			return this->topBar.get()->runFocussedBinds(playerInfo);
		}
		else {
			return this->BaseMulti::runFocussedBinds(playerInfo);
		}
	}

	CallBackBindResult Window::runOnHoverBinds(PlayerInfo& playerInfo) {
		if (this->minimized) {
			return this->topBar.get()->runOnHoverBinds(playerInfo);
		}
		else {
			return  this->BaseMulti::runOnHoverBinds(playerInfo);
		}
	}

	CallBackBindResult Window::runActiveBinds(PlayerInfo& playerInfo) {
		if (this->minimized) {
			return this->topBar.get()->runActiveBinds(playerInfo);
		}
		else {
			return this->BaseMulti::runActiveBinds(playerInfo);
		}
	}
}