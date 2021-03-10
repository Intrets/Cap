#include "State.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <render/infos/RenderInfo.h>

namespace ui
{
	CallBackBindResult State::runFrontBinds(PlayerInfo& playerInfo) {
		if (this->UIs.size() == 0) {
			return BIND::RESULT::CONTINUE;
		}

		CallBackBindResult activeResult =
			this->UIs.front().get()->runOnHoverBinds(playerInfo) |
			this->UIs.front().get()->runFocussedBinds(playerInfo) |
			this->UIs.front().get()->runActiveBinds(playerInfo) |
			this->UIs.front().get()->runGlobalBinds(playerInfo);
		CallBackBindResult res = 0;

		bool shouldExit = false;
		if (activeResult & BIND::RESULT::CLOSE) {
			this->UIs.pop_front();
			res |= BIND::RESULT::CLOSE;
			shouldExit = true;
		}
		if (activeResult & BIND::RESULT::STOP) {
			res |= BIND::RESULT::STOP;
			shouldExit = true;
		}

		if (shouldExit) {
			return res;
		}

		return BIND::RESULT::CONTINUE;
	}

	glm::vec2 State::getCursorPositionWorld() {
		return this->cursorWorld;
	}

	glm::vec2 State::getCursorPositionScreen() {
		return this->cursorScreen;
	}

	glm::vec2 State::getCursorPositionScreenClamped(float c) {
		return glm::clamp(this->getCursorPositionScreen(), -c, c);
	}

	void State::runUIBinds(PlayerInfo& playerInfo) {
		auto front = this->runFrontBinds(playerInfo);

		if (front & BIND::RESULT::STOP) {
			return;
		}

		if (this->UIs.empty()) {
			return;
		}

		playerInfo.controlState.writeConsumedBuffer();

		if (this->UIs.size() > 1) {
			auto it = this->UIs.begin(), last = this->UIs.end();
			if (front & BIND::RESULT::CLOSE) {
				front &= ~BIND::RESULT::CLOSE;
			}
			else {
				++it;
			}
			for (; it != last;) {
				auto& UI = *it;
				CallBackBindResult res = UI.get()->runOnHoverBinds(playerInfo) | UI.get()->runGlobalBinds(playerInfo);

				if (res & BIND::RESULT::CLOSE) {
					it = this->UIs.erase(it);
				}
				else if (res & BIND::RESULT::FOCUS) {
					auto temp = std::move(UI);
					it = this->UIs.erase(it);
					this->UIs.push_front(std::move(temp));
				}
				else {
					it++;
				}
				if (res & BIND::RESULT::STOP) {
					return;
				}
				playerInfo.controlState.writeConsumedBuffer();
			}
		}

		if (!playerInfo.controlState.worldBindsBlocked()) {
			for (auto it = this->UIs.begin(); it != this->UIs.end();) {
				auto& UI = *it;
				CallBackBindResult res = UI.get()->runGameWorldBinds(playerInfo);

				if (res & BIND::RESULT::CLOSE) {
					it = this->UIs.erase(it);
				}
				else if (res & BIND::RESULT::FOCUS) {
					auto temp = std::move(UI);
					it = this->UIs.erase(it);
					this->UIs.push_front(std::move(temp));
				}
				else {
					it++;
				}
				if (res & BIND::RESULT::STOP) {
					return;
				}
				playerInfo.controlState.writeConsumedBuffer();
			}
		}
	}

	void State::run(PlayerInfo& playerInfo) {
		this->runUIBinds(playerInfo);

		for (auto it = this->namedUIs.begin(), last = this->namedUIs.end(); it != last;) {
			if (!it->second.isValid()) {
				it = this->namedUIs.erase(it);
			}
			else {
				++it;
			}
		}

		for (auto& UI : this->UIsBuffer) {
			this->UIs.push_front(std::move(UI));
		}
		this->UIsBuffer.clear();

		for (auto& [name, ui] : this->namedUIsBuffer) {
			this->namedUIs.emplace(std::make_pair(name, ManagedReference<Base, Base>(ui)));
			this->UIs.push_front(std::move(ui));
		}
		this->namedUIsBuffer.clear();

		this->closedBuffer.clear();
	}

	bool State::updateSize(GLFWwindow* window) {
		int32_t x;
		int32_t y;
		glfwGetWindowSize(window, &x, &y);

		if (x <= 0 || y <= 0) {
			return false;
		}

		ScreenRectangle r;
		r.setPixelSize(glm::ivec2(x, y));
		r.set({ -1,-1 }, { 1,1 });
		for (auto& UI : this->UIs) {
			UI.get()->updateSize(r);
		}
		return true;
	}

	void State::updateCursor(GLFWwindow* window, glm::vec2 cam) {
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);

		int32_t frameSizeX, frameSizeY;
		glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
		x = x / frameSizeX;
		y = y / frameSizeY;
		y = 1 - y;
		x = 2 * x - 1;
		y = 2 * y - 1;

		float ratio = frameSizeX / static_cast<float>(frameSizeY);
		glm::vec2 viewport(ratio, 1.0f);
		// TODO: re-add
		// viewport *= Option<OPTION::CL_VIEWPORTSCALE, float>::getVal();

		this->cursorScreen = glm::clamp(glm::vec2(x, y), -1.0f, 1.0f);
		this->cursorWorld = cam + this->cursorScreen * viewport;
	}

	void State::appendRenderInfo(GameState& gameState, render::RenderInfo& renderInfo) {
		int32_t depth = 10;
		for (auto& UI : this->UIs) {
			depth = UI.get()->addRenderInfo(gameState, renderInfo, depth);
		}
	}

	void State::addUI(UniqueReference<Base, Base> ref) {
		ref.get()->addOnHoverBind({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL }, [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		{
			return BIND::RESULT::CONSUME;
		});
		this->UIsBuffer.push_back(std::move(ref));
	}

	bool State::addNamedUI(std::string const& name, std::function<UniqueReference<Base, Base>()> f) {
		auto namedUI = this->namedUIs.find(name);

		if (namedUI != this->namedUIs.end() && namedUI->second.isValid()) {
			if (auto ui = namedUI->second.getRef()) {
				for (auto it = this->UIs.begin(); it != this->UIs.end(); it++) {
					if (*it == ui) {
						this->UIsBuffer.push_back(std::move(*it));
						this->UIs.erase(it);
						break;
					}
				}
			}
			return false;
		}

		auto namedUIBuffered = this->namedUIsBuffer.find(name);

		if (namedUIBuffered != this->namedUIsBuffer.end()) {
			return false;
		}

		this->namedUIsBuffer[name] = f();
		this->namedUIsBuffer[name].get()->addOnHoverBind({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL }, [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		{
			return BIND::RESULT::CONSUME;
		});
		return true;
	}

	void State::addNamedUIReplace(std::string const& name, UniqueReference<Base, Base> ref) {
		this->closeNamedUI(name);

		ref.get()->addOnHoverBind({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL }, [](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		{
			return BIND::RESULT::CONSUME;
		});

		this->namedUIsBuffer[name] = std::move(ref);
	}

	void State::closeNamedUI(std::string const& name) {
		auto namedUI = this->namedUIs.find(name);

		if (namedUI != this->namedUIs.end()) {
			if (auto ui = namedUI->second.getRef()) {
				for (auto it = this->UIs.begin(); it != this->UIs.end(); it++) {
					if (*it == ui) {
						this->closedBuffer.push_back(std::move(*it));
						this->UIs.erase(it);
						break;
					}
				}
			}
			this->namedUIs.erase(namedUI);
		}

		auto namedUIBuffered = this->namedUIsBuffer.find(name);

		if (namedUIBuffered != this->namedUIsBuffer.end()) {
			this->closedBuffer.push_back(std::move(namedUIBuffered->second));
			this->namedUIsBuffer.erase(namedUIBuffered);
		}
	}

	void State::closeUI(WeakReference<Base, Base> ref) {
		for (auto it = this->UIs.begin(); it != this->UIs.end(); it++) {
			if (*it == ref) {
				this->closedBuffer.push_back(std::move(*it));
				this->UIs.erase(it);
				break;
			}
		}
	}

	void State::reset() {
		this->shouldReset_ = true;
	}

	bool State::shouldReset() {
		return this->shouldReset_;
	}

	void State::init() {
		//// Inventory
		//{
		//	UIO2::Global::push();

		//	UIO2::hideable();
		//	UIO2::window("Inventory", { {0.5f - 0.04f, -0.1f - 0.04f}, {1.0f - 0.04f, 1.0f - 0.04f} },
		//				 UIOWindow::TYPE::MINIMISE |
		//				 UIOWindow::TYPE::MOVE |
		//				 UIOWindow::TYPE::HIDE);
		//	UIO2::makeEnd<UIOInventory>();

		//	this->addUI(UIO2::Global::pop());
		//}

		//// Cursor renderer
		//{
		//	UIO2::Global::push();

		//	UIO2::makeEnd<UIOCursor>();

		//	this->addUI(UIO2::Global::pop());
		//}

		//// Hotbar
		//{
		//	UIO2::Global::push();

		//	UIO2::constrainHeight({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.05f });
		//	UIO2::constrainWidth({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.5f });
		//	UIO2::alignBottom();
		//	UIO2::background(COLORS::UI::WINDOWBACKGROUND);
		//	UIO2::makeEnd<UIOHotbar>();

		//	this->addUI(UIO2::Global::pop());
		//}

		//// save/load and other stuff
		//{
		//	UIO2::Global::push();

		//	UIO2::window("Debug Info", { {-1.0f, -0.8f}, {-0.7f, 1.0f} },
		//				 UIOWindow::TYPE::MINIMISE |
		//				 UIOWindow::TYPE::RESIZEVERTICAL |
		//				 UIOWindow::TYPE::RESIZEHORIZONTAL |
		//				 UIOWindow::TYPE::RESIZE |
		//				 UIOWindow::TYPE::MOVE);
		//	UIO2::constructDebugInfo();

		//	this->addUI(UIO2::Global::pop());
		//}

		//// wasd movement in world
		//{
		//	UniqueReference<Base, UIOInvisible> movement = UIO2::Global::getManager().makeUniqueRef<UIOInvisible>();

		//	movement.get()->addGlobalBind({ CONTROL::KEY::LEFT, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		//	{
		//		playerInfo.getPlayer().pos.x -= 1.0f;
		//		return BIND::RESULT::CONTINUE;
		//	});

		//	movement.get()->addGlobalBind({ CONTROL::KEY::RIGHT, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		//	{
		//		playerInfo.getPlayer().pos.x += 1.0f;
		//		return BIND::RESULT::CONTINUE;
		//	});

		//	movement.get()->addGlobalBind({ CONTROL::KEY::DOWN, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		//	{
		//		playerInfo.getPlayer().pos.y -= 1.0f;
		//		return BIND::RESULT::CONTINUE;
		//	});

		//	movement.get()->addGlobalBind({ CONTROL::KEY::UP, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		//	{
		//		playerInfo.getPlayer().pos.y += 1.0f;
		//		return BIND::RESULT::CONTINUE;
		//	});

		//	movement.get()->addGlobalBind({ CONTROL::KEY::SCROLL_UP }, [&](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		//	{
		//		using viewport = Option<OPTION::CL_VIEWPORTSCALE, float>;
		//		viewport::setVal(viewport::getVal() * 0.8f);
		//		return BIND::RESULT::CONTINUE;
		//	});

		//	movement.get()->addGlobalBind({ CONTROL::KEY::SCROLL_DOWN }, [&](PlayerInfo& playerInfo, Base* self_) -> CallBackBindResult
		//	{
		//		using viewport = Option<OPTION::CL_VIEWPORTSCALE, float>;
		//		viewport::setVal(viewport::getVal() / 0.8f);
		//		return BIND::RESULT::CONTINUE;
		//	});

		//	this->UIs.push_back(std::move(movement));
		//}
	}

	void State::clear() {
		this->shouldReset_ = false;
		this->namedUIs.clear();
		this->UIs.clear();
		this->UIsBuffer.clear();
	}

	State::State() {
		this->init();
	}
}
