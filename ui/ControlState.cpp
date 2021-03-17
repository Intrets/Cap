#include "ControlState.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <wglm/detail/func_common.hpp>

namespace ui
{
	ControlState::ControlState() {
		this->controlState.fill(0);

		this->keyToControl[GLFW_KEY_A].push_back(CONTROL::KEY::LEFT);
		this->keyToControl[GLFW_KEY_D].push_back(CONTROL::KEY::RIGHT);
		this->keyToControl[GLFW_KEY_W].push_back(CONTROL::KEY::UP);
		this->keyToControl[GLFW_KEY_S].push_back(CONTROL::KEY::DOWN);
		this->keyToControl[GLFW_KEY_Q].push_back(CONTROL::KEY::ACTION_PICK);
		this->keyToControl[GLFW_KEY_F].push_back(CONTROL::KEY::ACTION_DELETE);
		this->keyToControl[GLFW_KEY_E].push_back(CONTROL::KEY::ACTION_USE);
		this->keyToControl[GLFW_KEY_R].push_back(CONTROL::KEY::ROTATER);
		this->keyToControl[GLFW_KEY_Z].push_back(CONTROL::KEY::ACTION_ACTIVATE);
		this->keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_1].push_back(CONTROL::KEY::ACTION0);
		this->keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_2].push_back(CONTROL::KEY::ACTION1);
		this->keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_3].push_back(CONTROL::KEY::ACTION2);
		this->keyToControl[GLFW_KEY_F7].push_back(CONTROL::KEY::TEST_SAVE);
		this->keyToControl[GLFW_KEY_F8].push_back(CONTROL::KEY::TEST_LOAD);
		this->keyToControl[GLFW_KEY_F9].push_back(CONTROL::KEY::TOGGLE_DEBUG);
		this->keyToControl[GLFW_KEY_ESCAPE].push_back(CONTROL::KEY::CANCEL);

		this->keyToControl[GLFW_KEY_UP].push_back(CONTROL::KEY::TEXT_UP);
		this->keyToControl[GLFW_KEY_DOWN].push_back(CONTROL::KEY::TEXT_DOWN);
		this->keyToControl[GLFW_KEY_LEFT].push_back(CONTROL::KEY::TEXT_LEFT);
		this->keyToControl[GLFW_KEY_RIGHT].push_back(CONTROL::KEY::TEXT_RIGHT);

		this->keyToControl[GLFW_KEY_1].push_back(CONTROL::KEY::TOOL_1);
		this->keyToControl[GLFW_KEY_2].push_back(CONTROL::KEY::TOOL_2);
		this->keyToControl[GLFW_KEY_3].push_back(CONTROL::KEY::TOOL_3);
		this->keyToControl[GLFW_KEY_4].push_back(CONTROL::KEY::TOOL_4);
		this->keyToControl[GLFW_KEY_5].push_back(CONTROL::KEY::TOOL_5);
		this->keyToControl[GLFW_KEY_6].push_back(CONTROL::KEY::TOOL_6);
		this->keyToControl[GLFW_KEY_7].push_back(CONTROL::KEY::TOOL_7);
		this->keyToControl[GLFW_KEY_8].push_back(CONTROL::KEY::TOOL_8);
		this->keyToControl[GLFW_KEY_9].push_back(CONTROL::KEY::TOOL_9);
		this->keyToControl[GLFW_KEY_0].push_back(CONTROL::KEY::TOOL_0);

		this->keyToControl[GLFW_KEY_BACKSPACE].push_back(CONTROL::KEY::BACKSPACE);
		this->keyToControl[GLFW_KEY_DELETE].push_back(CONTROL::KEY::DELETE);
		this->keyToControl[GLFW_KEY_TAB].push_back(CONTROL::KEY::TAB);

		this->keyToControl[GLFW_KEY_A].push_back(CONTROL::KEY::A);
		this->keyToControl[GLFW_KEY_B].push_back(CONTROL::KEY::B);
		this->keyToControl[GLFW_KEY_C].push_back(CONTROL::KEY::C);
		this->keyToControl[GLFW_KEY_D].push_back(CONTROL::KEY::D);
		this->keyToControl[GLFW_KEY_E].push_back(CONTROL::KEY::E);
		this->keyToControl[GLFW_KEY_F].push_back(CONTROL::KEY::F);
		this->keyToControl[GLFW_KEY_G].push_back(CONTROL::KEY::G);
		this->keyToControl[GLFW_KEY_H].push_back(CONTROL::KEY::H);
		this->keyToControl[GLFW_KEY_I].push_back(CONTROL::KEY::I);
		this->keyToControl[GLFW_KEY_J].push_back(CONTROL::KEY::J);
		this->keyToControl[GLFW_KEY_K].push_back(CONTROL::KEY::K);
		this->keyToControl[GLFW_KEY_L].push_back(CONTROL::KEY::L);
		this->keyToControl[GLFW_KEY_M].push_back(CONTROL::KEY::M);
		this->keyToControl[GLFW_KEY_N].push_back(CONTROL::KEY::N);
		this->keyToControl[GLFW_KEY_O].push_back(CONTROL::KEY::O);
		this->keyToControl[GLFW_KEY_P].push_back(CONTROL::KEY::P);
		this->keyToControl[GLFW_KEY_Q].push_back(CONTROL::KEY::Q);
		this->keyToControl[GLFW_KEY_R].push_back(CONTROL::KEY::R);
		this->keyToControl[GLFW_KEY_S].push_back(CONTROL::KEY::S);
		this->keyToControl[GLFW_KEY_T].push_back(CONTROL::KEY::T);
		this->keyToControl[GLFW_KEY_U].push_back(CONTROL::KEY::U);
		this->keyToControl[GLFW_KEY_V].push_back(CONTROL::KEY::V);
		this->keyToControl[GLFW_KEY_W].push_back(CONTROL::KEY::W);
		this->keyToControl[GLFW_KEY_X].push_back(CONTROL::KEY::X);
		this->keyToControl[GLFW_KEY_Y].push_back(CONTROL::KEY::Y);
		this->keyToControl[GLFW_KEY_Z].push_back(CONTROL::KEY::Z);
	}

	void ControlState::cycleStates() {
		for (auto& state : this->controlState) {
			state &= ~CONTROL::STATE::PRESSED;
			state &= ~CONTROL::STATE::RELEASED;
			state &= ~CONTROL::STATE::REPEAT;
		}
		this->charBuffer.clear();
		this->scrollDistance = 0;
		this->blockUserInput = false;
		this->controlState[static_cast<int32_t>(CONTROL::KEY::MOUSE_POS_CHANGED)] = CONTROL::STATE::PRESSED;
		this->controlState[static_cast<int32_t>(CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL)] = CONTROL::STATE::PRESSED;
		this->controlState[static_cast<int32_t>(CONTROL::KEY::EVERY_TICK)] = CONTROL::STATE::PRESSED;

		this->consumed.fill(false);
		this->consumedBuffer.fill(false);

		this->blockWorldBinds = false;
	}

	std::string ControlState::getCharBuffer() {
		return this->charBuffer;
	}

	std::string ControlState::getClipboard() {
		return this->clipboard;
	}

	void ControlState::setClipboard(std::string const& s) {
		this->clipboard = s;
	}

	void ControlState::setBlockWorldBinds(bool b) {
		this->blockWorldBinds = b;
	}

	bool ControlState::worldBindsBlocked() {
		return this->blockWorldBinds;
	}

	void ControlState::consumeControl(CONTROL::KEY control) {
		this->consumed[static_cast<size_t>(control)] = true;
	}

	void ControlState::consumeBufferControl(CONTROL::KEY control) {
		this->consumedBuffer[static_cast<size_t>(control)] = true;
	}

	void ControlState::writeConsumedBuffer() {
		for (int32_t i = 0; i < static_cast<int32_t>(CONTROL::KEY::CONTROLS_MAX); i++) {
			this->consumed[i] = this->consumed[i] || this->consumedBuffer[i];
		}
		this->consumedBuffer.fill(false);
	}

	bool ControlState::activated(BindControl bindControl) {
		if (this->consumed[static_cast<size_t>(bindControl.control)]) {
			return false;
		}

		if (this->blockUserInput) {
			switch (bindControl.control) {
				case CONTROL::KEY::RIGHT:
				case CONTROL::KEY::LEFT:
				case CONTROL::KEY::UP:
				case CONTROL::KEY::DOWN:
					return false;
					break;
				default:
					break;
			}
		}

		if ((this->modifiers & bindControl.modifiers) != bindControl.modifiers) {
			return false;
		}

		return this->controlState[static_cast<size_t>(bindControl.control)] & bindControl.state;
	}

	void ControlState::key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
		// unknown key?
		if (key == -1) {
			return;
		}

		if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			this->charBuffer.push_back('\n');
			this->controlState[static_cast<size_t>(CONTROL::KEY::CHAR_BUFFER_CHANGED)] = CONTROL::STATE::PRESSED;
		}

		if ((action == GLFW_PRESS || action == GLFW_REPEAT) && (key == GLFW_KEY_BACKSPACE || key == GLFW_KEY_DELETE || key == GLFW_KEY_ENTER)) {
			this->controlState[static_cast<size_t>(CONTROL::KEY::ANYTHING_TEXT)] = CONTROL::STATE::PRESSED;
		}

		if (action == GLFW_PRESS) {
			this->controlState[static_cast<size_t>(CONTROL::KEY::ANYTHING_KEY)] = CONTROL::STATE::PRESSED;
		}

		switch (key) {
			case GLFW_KEY_LEFT_SHIFT:
			case GLFW_KEY_RIGHT_SHIFT:
				if (action == GLFW_PRESS) {
					this->modifiers &= ~CONTROL::MODIFIER::NONE;
					this->modifiers |= CONTROL::MODIFIER::SHIFT;
				}
				else if (action == GLFW_RELEASE) {
					this->modifiers &= ~CONTROL::MODIFIER::SHIFT;
				}
				break;
			case GLFW_KEY_LEFT_CONTROL:
			case GLFW_KEY_RIGHT_CONTROL:
				if (action == GLFW_PRESS) {
					this->modifiers &= ~CONTROL::MODIFIER::NONE;
					this->modifiers |= CONTROL::MODIFIER::CONTROL;
				}
				else if (action == GLFW_RELEASE) {
					this->modifiers &= ~CONTROL::MODIFIER::CONTROL;
				}
				break;
			case GLFW_KEY_LEFT_ALT:
			case GLFW_KEY_RIGHT_ALT:
				if (action == GLFW_PRESS) {
					this->modifiers &= ~CONTROL::MODIFIER::NONE;
					this->modifiers |= CONTROL::MODIFIER::ALT;
				}
				else if (action == GLFW_RELEASE) {
					this->modifiers &= ~CONTROL::MODIFIER::ALT;
				}
				break;
			default:
				break;
		}

		if (this->modifiers == 0) {
			this->modifiers = CONTROL::MODIFIER::NONE;
		}

		switch (action) {
			case GLFW_REPEAT:
				for (auto control : this->keyToControl[key]) {
					this->controlState[static_cast<size_t>(control)] |= CONTROL::STATE::REPEAT;
				}
				break;
			case GLFW_PRESS:
				for (auto control : this->keyToControl[key]) {
					this->controlState[static_cast<size_t>(control)] |= CONTROL::STATE::DOWN;
					this->controlState[static_cast<size_t>(control)] |= CONTROL::STATE::PRESSED;
				}
				break;
			case GLFW_RELEASE:
				for (auto control : this->keyToControl[key]) {
					this->controlState[static_cast<size_t>(control)] &= ~CONTROL::STATE::DOWN;
					this->controlState[static_cast<size_t>(control)] |= CONTROL::STATE::RELEASED;
				}
				break;
			default:
				break;
		}
	}

	void ControlState::char_callback(GLFWwindow* window, unsigned int character) {
		if (character > 127) {
			return;
		}
		this->controlState[static_cast<size_t>(CONTROL::KEY::CHAR_BUFFER_CHANGED)] = CONTROL::STATE::PRESSED;
		this->controlState[static_cast<size_t>(CONTROL::KEY::ANYTHING_TEXT)] = CONTROL::STATE::PRESSED;
		char c = static_cast<char>(character);
		this->charBuffer.push_back(c);
	}

	void ControlState::scroll_callback(GLFWwindow* w, double xoffset, double yoffset) {
		if (yoffset < 0) {
			this->scrollDistance -= static_cast<int32_t>(floor(glm::abs(yoffset)));
		}
		else if (yoffset > 0) {
			this->scrollDistance += static_cast<int32_t>(floor(glm::abs(yoffset)));
		}

		if (scrollDistance < 0) {
			this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_DOWN)] = CONTROL::STATE::PRESSED;
			this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_UP)] = CONTROL::STATE::UP;
		}
		else if (scrollDistance > 0) {
			this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_UP)] = CONTROL::STATE::PRESSED;
			this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_DOWN)] = CONTROL::STATE::UP;
		}
		else {
			this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_UP)] = CONTROL::STATE::UP;
			this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_DOWN)] = CONTROL::STATE::UP;
		}
	}

	BindControl::BindControl(CONTROL::KEY c) :
		BindControl(c, CONTROL::STATE::PRESSED) {
	}

	BindControl::BindControl(CONTROL::KEY c, int32_t s) :
		BindControl(c, s, CONTROL::MODIFIER::ANY) {
	}

	BindControl::BindControl(CONTROL::KEY c, int32_t s, int32_t m) :
		control(c),
		state(s),
		modifiers(m) {
	}
}
