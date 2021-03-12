#pragma once

namespace ui
{
	class Base;
	class TextDisplay;
	class Button;

	namespace BASE
	{
		void activatable(Base* ptr);
		void focusable(Base* ptr);

		// Mouse is over UI, dont run world binds
		void blockWorldBinds(Base* ptr);
	}

	namespace TEXTEDIT
	{
		void clickSelect(TextDisplay* ptr);
		void input(TextDisplay* ptr);
		void inputNoLineBreaks(TextDisplay* ptr);
		void backspace(TextDisplay* ptr);
		void tab(TextDisplay* ptr);
		void del(TextDisplay* ptr);
		void down(TextDisplay* ptr);
		void up(TextDisplay* ptr);
		void right(TextDisplay* ptr);
		void left(TextDisplay* ptr);
		void viewUp(TextDisplay* ptr);
		void viewDown(TextDisplay* ptr);
		void wrap(TextDisplay* ptr);
		void nowrap(TextDisplay* ptr);

		void normalbinds(TextDisplay* ptr);

		void normal(TextDisplay* ptr);
		void insert(TextDisplay* ptr);
	}

	namespace BUTTON
	{
		void close(Button* ptr);
		void hide(Button* ptr);
	}
}
