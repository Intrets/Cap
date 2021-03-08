#pragma once

#include "Base.h"
#include "Enums.h"
#include "impl/Text.h"

namespace ui
{
	class TextDisplay : public BaseEnd
	{
	private:
		bool lineWrap = false;
		int32_t ticksSelected = 0;
		bool clickSupport = false;
		bool shrinkToFit = false;

	public:
		TEXTDISPLAY::MODE mode = TEXTDISPLAY::MODE::NORMAL;

		impl::Text text;

		TextDisplay(Handle self);
		TextDisplay(Handle self, bool lineWrap);

		virtual ~TextDisplay() = default;

		virtual void translate(glm::vec2 p) override;

		virtual	CallBackBindResult runActiveBinds(PlayerInfo& playerInfo) override;

		void setMode(TEXTDISPLAY::MODE mode_);

		void setShrinkToFit(bool b);
		void setClickSupport(bool b);

		void setWrap(bool b);

		void setText(std::string const& text_);
		void setText(std::vector<std::string> const& text_);

		void moveCursor(glm::ivec2 p);
		void setCursor(glm::ivec2 p);

		void moveStartWordForward();
		void moveStartWordBackward();

		void moveEndWord();

		bool yank(ControlState& controlState);
		bool paste(ControlState& controlState);

		bool insertLineAfter();

		void insertText(std::string text_);

		void backspaceChar();
		void deleteChar();

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
		virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
	};
}
