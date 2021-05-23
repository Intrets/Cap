#pragma once

#include <string>
#include <array>
#include <vector>

struct GLFWwindow;

namespace ui
{
	struct BindControl;

	namespace CONTROL
	{
		enum class KEY
		{
			NOTHING,
			UP,
			DOWN,
			LEFT,
			RIGHT,
			JUMP,
			ZOOM_IN,
			ZOOM_OUT,
			ROTATEL,
			ROTATER,
			ACTION_PICK,
			ACTION_DELETE,
			ACTION_USE,
			DESTROYBLOCK,
			PLACEBLOCK,
			ACTION1,
			TEST_RETRACT,
			TEST_EXTEND,
			ACTION0,
			ACTION3,
			ACTION4,
			ACTION5,
			ACTION6,
			ACTION_ACTIVATE,
			SELECTACTIVITYROOT,
			TEST_CYCLEBLOCK,
			TEST_SAVE,
			TEST_LOAD,
			TEST_EXIT,
			TOGGLE_DEBUG_RENDER,
			MOUSE_POS_CHANGED,
			MOUSE_POS_CHANGED_TOPLEVEL,
			ACTION2,
			TOOL_0,
			TOOL_1,
			TOOL_2,
			TOOL_3,
			TOOL_4,
			TOOL_5,
			TOOL_6,
			TOOL_7,
			TOOL_8,
			TOOL_9,
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			SCROLL_UP,
			SCROLL_DOWN,
			BACKSPACE,
			DELETE,
			TAB,
			CHAR_BUFFER_CHANGED,
			ANYTHING_TEXT,
			ANYTHING_KEY,
			EVERY_TICK,
			CANCEL,
			TEXT_UP,
			TEXT_DOWN,
			TEXT_LEFT,
			TEXT_RIGHT,
			CONTROLS_MAX,
		};

		enum STATE
		{
			UP = 0,
			DOWN = 1 << 0,
			PRESSED = 1 << 1,
			RELEASED = 1 << 2,
			REPEAT = 1 << 3,
		};

		enum MODIFIER
		{
			ANY = 0,
			SHIFT = 1 << 0,
			CONTROL = 1 << 1,
			ALT = 1 << 2,
			NONE = 1 << 3,
		};
	}

	class ControlState
	{
	public:
		ControlState();

		void cycleStates();

		std::string getCharBuffer();
		int32_t scrollDistance = 0;
		bool blockUserInput = false;

		std::string getClipboard();
		void setClipboard(std::string const& s);

		void setBlockWorldBinds(bool b);
		bool worldBindsBlocked();

		void consumeControl(CONTROL::KEY control);
		void consumeBufferControl(CONTROL::KEY control);

		void writeConsumedBuffer();

		bool activated(BindControl bindControl);

		void key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods);
		void char_callback(GLFWwindow* window, unsigned int character);
		void scroll_callback(GLFWwindow* w, double xoffset, double yoffset);

	private:
		// hard coded values from GLFW so header isnt included in this file
		std::array<std::vector<CONTROL::KEY>, 348 + 7> keyToControl;

		std::array<int32_t, static_cast<size_t>(CONTROL::KEY::CONTROLS_MAX)> controlState;

		std::array<bool, static_cast<size_t>(CONTROL::KEY::CONTROLS_MAX)> consumed;
		std::array<bool, static_cast<size_t>(CONTROL::KEY::CONTROLS_MAX)> consumedBuffer;

		std::string clipboard = "";

		int32_t modifiers = 0;

		std::string charBuffer;

		bool blockWorldBinds = false;
	};

	struct BindControl
	{
		CONTROL::KEY control;
		int32_t modifiers;
		int32_t state;

		BindControl(CONTROL::KEY control);
		BindControl(CONTROL::KEY control, int32_t state);
		BindControl(CONTROL::KEY control, int32_t state, int32_t modifiers);

		bool operator== (const BindControl& other) const {
			return this->control == other.control && this->state == other.state;
		}
	};
}
