#include "Constructer.h"

#include <game/player/PlayerInfo.h>
#include <misc/StringHelpers.h>

#include "State.h"
#include "SizeType.h"
#include "binds/Binds.h"

#include "Proxy.h"
#include "TextDisplay.h"
#include "SizeType.h"
#include "ConstrainSize.h"
#include "Button.h"
#include "Pad.h"
#include "Window.h"
#include "List.h"
#include "Empty.h"
#include "ColoredBackground.h"
#include "Destructible.h"
#include "AnchoredProxy.h"
#include "Hideable.h"
#include "Grid.h"
#include "FreeSize.h"
#include "InitialSize.h"

namespace ui
{
	std::vector<std::unique_ptr<ConstructerState>> Global::states;
	ReferenceManager<Base> Global::manager;

	void ConstructerState::addSingle(UniqueReference<Base, Base> ref) {
		WeakReference<Base, Base> weakRef = ref;

		if (this->singlesLeaf.isNotNull()) {
			this->singlesLeaf.get()->addElement(std::move(ref));
		}
		else {
			this->singlesRoot = std::move(ref);
		}

		this->singlesLeaf = weakRef;
	}

	void ConstructerState::addSingle(UniqueReference<Base, Base> ref, WeakReference<Base, Base> leaf) {
		if (this->singlesLeaf.isNotNull()) {
			this->singlesLeaf.get()->addElement(std::move(ref));
		}
		else {
			this->singlesRoot = std::move(ref);
		}

		this->singlesLeaf = leaf;
	}

	void ConstructerState::addMulti(UniqueReference<Base, Base> ref) {
		WeakReference<Base, Base> weakRef = ref;

		if (this->singlesLeaf.isNull()) {
			this->stack.back().get()->addElement(std::move(ref));
		}
		else {
			this->singlesLeaf.get()->addElement(std::move(ref));
			this->stack.back().get()->addElement(std::move(this->singlesRoot));

			this->singlesLeaf.clear();
			assert(this->singlesRoot.isNull());
		}
		this->stack.push_back(weakRef);
	}

	void ConstructerState::addEnd(UniqueReference<Base, Base> ref) {
		if (this->singlesLeaf.isNull()) {
			this->stack.back().get()->addElement(std::move(ref));
		}
		else {
			this->singlesLeaf.get()->addElement(std::move(ref));
			this->stack.back().get()->addElement(std::move(this->singlesRoot));
			this->singlesLeaf.clear();
		}
	}

	ReferenceManager<Base>& Global::getManager() {
		return Global::manager;
	}

	ConstructerState* Global::getState() {
		return Global::states.back().get();
	}

	void Global::push() {
		Global::states.push_back(std::make_unique<ConstructerState>());
		auto& state = Global::states.back();

		auto ref = Global::getManager().makeUniqueRef<Proxy>();
		state->stack.push_back(ref);
		state->root = std::move(ref);
	}

	UniqueReference<Base, Base> Global::pop() {
		assert(Global::states.size() > 0);
		auto& state = Global::states.back();

		assert(state->stack.size() == 1);
		assert(state->singlesLeaf.isNull());
		assert(state->singlesRoot.isNull());

		state->singlesLeaf.clear();
		state->stack.clear();

		auto res = std::move(state->root.get()->getMain());

		Global::states.pop_back();

		return res;
	}

	WeakReference<Base, TextDisplay> text(std::string const& t, bool shrinkToFit) {
		auto ref = Global::getManager().makeUniqueRef<TextDisplay>();
		auto res = ref.as<TextDisplay>();

		ref.get()->setText(t);
		ref.get()->text.hideCursor();
		ref.get()->setShrinkToFit(shrinkToFit);

		Global::getState()->addEnd(std::move(ref));

		return res;
	}

	WeakReference<Base, ConstrainSize> constrainHeight(SizeType height) {
		auto ptr = Global::getState()->addOrModifySingle<ConstrainSize>();
		ptr.get()->setHeight(height);
		return ptr;
	}

	WeakReference<Base, ConstrainSize> constrainWidth(SizeType width) {
		auto ptr = Global::getState()->addOrModifySingle<ConstrainSize>();
		ptr.get()->setWidth(width);
		return ptr;
	}

	WeakReference<Base, ConstrainSize> constrainSize(SizeType size) {
		auto ptr = Global::getState()->addOrModifySingle<ConstrainSize>();
		ptr.get()->setWidth(size);
		ptr.get()->setHeight(size);
		return ptr;
	}

	WeakReference<Base, Button> button(bool shrinkToFit) {
		auto ref = Global::getManager().makeUniqueRef<Button>();
		auto res = ref.as<Button>();

		ref.get()->setShrinkToFit(shrinkToFit);

		Global::getState()->addSingle(std::move(ref));
		return res;
	}

	WeakReference<Base, Window> window(std::string const& title, Rect size, int32_t types) {
		const int32_t resizeSliverSize = 7;

		auto mainPad = Global::getManager().makeUniqueRef<Pad>();
		WeakReference<Base, Base> leaf = mainPad;

		auto mainPadPtr = mainPad.get();
		mainPadPtr->top = { SIZETYPE::FH, 1.2f };
		if (types & WINDOW::TYPE::RESIZE) {
			types |= WINDOW::TYPE::RESIZEHORIZONTAL | WINDOW::TYPE::RESIZEVERTICAL;
		}
		if (types & WINDOW::TYPE::RESIZEHORIZONTAL) {
			mainPadPtr->right = { SIZETYPE::PX, resizeSliverSize };
		}
		if (types & WINDOW::TYPE::RESIZEVERTICAL) {
			mainPadPtr->bottom = { SIZETYPE::PX, resizeSliverSize };
		}

		// ------------------
		// Main Window object
		// ------------------

		Global::push();

		initial(size);
		auto windowPtr = makeEnd<Window>();
		windowPtr.get()->addElement(std::move(mainPad));
		BASE::focusable(windowPtr.get());
		BASE::blockWorldBinds(windowPtr.get());

		UniqueReference<Base, Base> windowRef = Global::pop();

		// -------
		// Top Bar
		// -------

		Global::push();

		alignTop();
		constrainHeight({ SIZETYPE::FH, 1.2f });
		auto topBar = startList(DIR::RIGHT_REVERSE);
		BASE::focusable(topBar.get());
		BASE::blockWorldBinds(topBar.get());

		// ----------------------
		// Top Bar Elements setup
		// ----------------------

		// ---------
		// Title Bar
		// ---------

		Global::push();

		if (types & Window::TYPE::MOVE) {
			auto button = textButton(title);
			button.get()->addFocussedBind(
				{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
				[windowPtr = windowPtr.get()](PlayerInfo& playerInfo, Base* self_)->CallBackBindResult
			{
				auto self = static_cast<Button*>(self_);
				if (self->isDown()) {
					// glm::ivec2(-1, 1) <- offset for border
					auto newPos = playerInfo.uiState.getCursor() - self->getMousePressOffset() + glm::ivec2(-1, 1);
					windowPtr->screenRectangle.translate(newPos - windowPtr->screenRectangle.getTopLeft());
				}
				return BIND::RESULT::CONTINUE;
			});
		}
		else {
			pad({ SIZETYPE::PX, 1 });
			textDisplaySingle(title);
		}

		topBar.get()->addElement(Global::pop());

		// --------
		// Minimise
		// --------

		if (types & Window::TYPE::MINIMISE) {
			Global::push();

			constrainSize({ SIZETYPE::FH, 1.2f });
			auto button = textButton(" _");
			button.get()->setOnRelease([windowPtr = windowPtr.get()](PlayerInfo& playerInfo, Base* self_)->CallBackBindResult
			{
				windowPtr->minimized = !windowPtr->minimized;
				return BIND::RESULT::CONTINUE;
			});

			topBar.get()->addElement(Global::pop());
		}

		// ----------
		// Close/Hide
		// ----------

		if (types & Window::TYPE::CLOSE || types & Window::TYPE::HIDE) {
			Global::push();

			constrainSize({ SIZETYPE::FH, 1.2f });
			auto button = textButton(" x");

			if (types & Window::TYPE::CLOSE) {
				BUTTON::close(button.get());
			}
			else if (types & Window::TYPE::HIDE) {
				BUTTON::hide(button.get());

			}

			topBar.get()->addElement(Global::pop());
		}

		endList();
		windowPtr.get()->addElementMulti(Global::pop());
		windowPtr.get()->topBar = topBar;

		// --------------------
		// Resize Buttons setup
		// --------------------

		if (types & Window::TYPE::RESIZEVERTICAL) {
			Global::push();

			alignBottomLeft();
			constrainHeight({ SIZETYPE::PX, resizeSliverSize });
			padRight({ SIZETYPE::PX, types & Window::TYPE::RESIZEHORIZONTAL ? resizeSliverSize : 0 });
			padLeft({ SIZETYPE::PX, 1 });
			padTop({ SIZETYPE::PX, 1 });
			auto button = ui::button();
			makeEnd<Empty>();

			button.get()->addFocussedBind(
				{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
				[windowPtr = windowPtr.get()](PlayerInfo& playerInfo, Base* self_)->CallBackBindResult
			{
				auto self = static_cast<Button*>(self_);
				if (self->isDown()) {
					int32_t bottom = playerInfo.uiState.getCursor().y - self->getMousePressOffset().y - self->screenRectangle.size().y;

					if (windowPtr->screenRectangle.getTop() - bottom < 30) {
						bottom = windowPtr->screenRectangle.getTop() - 30;
					}
					windowPtr->screenRectangle.setBot(bottom);
					windowPtr->updateSize(windowPtr->screenRectangle);
				}
				return BIND::RESULT::CONTINUE;
			});

			windowPtr.get()->addElementMulti(Global::pop());
		}

		if (types & Window::TYPE::RESIZEHORIZONTAL) {
			Global::push();

			alignRight();
			constrainWidth({ SIZETYPE::PX, resizeSliverSize });
			padLeft({ SIZETYPE::PX, 1 });
			padTop({ SIZETYPE::FH, 1.2f });
			padBot({ SIZETYPE::PX, types & Window::TYPE::RESIZEVERTICAL ? resizeSliverSize : 0 });
			auto button = ui::button();
			makeEnd<Empty>();

			button.get()->addFocussedBind(
				{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
				[windowPtr = windowPtr.get()](PlayerInfo& playerInfo, Base* self_)->CallBackBindResult
			{
				auto self = static_cast<Button*>(self_);
				if (self->isDown()) {
					int32_t right = playerInfo.uiState.getCursor().x - self->getMousePressOffset().x + self->screenRectangle.size().x;

					if (right - windowPtr->screenRectangle.getLeft() < 30) {
						right = windowPtr->screenRectangle.getLeft() + 30;
					}
					windowPtr->screenRectangle.setRight(right);
					windowPtr->updateSize(windowPtr->screenRectangle);
				}
				return BIND::RESULT::CONTINUE;
			});

			windowPtr.get()->addElementMulti(Global::pop());
		}

		if (types & Window::TYPE::RESIZE) {
			Global::push();

			alignBottomRight();
			constrainWidth({ SIZETYPE::PX, resizeSliverSize });
			constrainHeight({ SIZETYPE::PX, resizeSliverSize });
			auto button = ui::button();
			makeEnd<Empty>();

			button.get()->addFocussedBind(
				{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
				[windowPtr = windowPtr.get()](PlayerInfo& playerInfo, Base* self_)->CallBackBindResult
			{
				auto self = static_cast<Button*>(self_);
				if (self->isDown()) {
					glm::ivec2 bottomRight = playerInfo.uiState.getCursor() - self->getMousePressOffset() + glm::ivec2(1, -1) * self->screenRectangle.size();

					if (bottomRight.x - windowPtr->screenRectangle.getLeft() < 10) {
						bottomRight.x = windowPtr->screenRectangle.getLeft() + 10;
					}
					if (windowPtr->screenRectangle.getTop() - bottomRight.y < 10) {
						bottomRight.y = windowPtr->screenRectangle.getTop() - 10;
					}
					windowPtr->screenRectangle.setBottomRight(bottomRight);
					windowPtr->updateSize(windowPtr->screenRectangle);
				}
				return BIND::RESULT::CONTINUE;
			});

			windowPtr.get()->addElementMulti(Global::pop());
		}

		Global::getState()->addSingle(std::move(windowRef), leaf);

		return windowPtr;
	}

	WeakReference<Base, Hideable> hideable() {
		auto ref = Global::getManager().makeUniqueRef<Hideable>();
		auto res = ref.as<Hideable>();

		Global::getState()->addSingle(std::move(ref));

		return res;
	}

	WeakReference<Base, ColoredBackground> background(glm::vec4 color) {
		auto ptr = Global::getState()->addSingle<ColoredBackground>();
		ptr.get()->color = color;
		return ptr;
	}

	WeakReference<Base, ConstrainSize> align(ALIGNMENT alignment) {
		auto ptr = Global::getState()->addOrModifySingle<ConstrainSize>();
		ptr.get()->setAlignment(alignment);
		return ptr;
	}

	WeakReference<Base, ConstrainSize> alignCenter() {
		return align(ALIGNMENT::CENTER);
	}

	WeakReference<Base, ConstrainSize> alignTop() {
		return align(ALIGNMENT::TOP);
	}

	WeakReference<Base, ConstrainSize> alignBottom() {
		return align(ALIGNMENT::BOTTOM);
	}

	WeakReference<Base, ConstrainSize> alignLeft() {
		return align(ALIGNMENT::LEFT);
	}

	WeakReference<Base, ConstrainSize> alignRight() {
		return align(ALIGNMENT::RIGHT);
	}

	WeakReference<Base, ConstrainSize> alignBottomLeft() {
		return align(ALIGNMENT::BOTTOMLEFT);
	}

	WeakReference<Base, ConstrainSize> alignBottomRight() {
		return align(ALIGNMENT::BOTTOMRIGHT);
	}

	WeakReference<Base, ConstrainSize> alignTopLeft() {
		return align(ALIGNMENT::TOPLEFT);
	}

	WeakReference<Base, ConstrainSize> alignTopRight() {
		return align(ALIGNMENT::TOPRIGHT);
	}

	WeakReference<Base, FreeSize> free() {
		auto ref = Global::getManager().makeUniqueRef<FreeSize>();
		auto res = ref.as<FreeSize>();

		Global::getState()->addSingle(std::move(ref));

		return res;
	}

	WeakReference<Base, InitialSize> initial(Rect rect) {
		auto ref = Global::getManager().makeUniqueRef<InitialSize>(rect);
		auto res = ref.as<InitialSize>();

		Global::getState()->addSingle(std::move(ref));

		return res;
	}

	WeakReference<Base, Destructible> destructible() {
		auto ref = Global::getManager().makeUniqueRef<Destructible>();
		auto res = ref.as<Destructible>();

		Global::getState()->addSingle(std::move(ref));

		return res;
	}

	WeakReference<Base, Pad> pad(SizeType padding) {
		auto ptr = Global::getState()->addOrModifySingle<Pad>();
		ptr.get()->left = padding;
		ptr.get()->right = padding;
		ptr.get()->top = padding;
		ptr.get()->bottom = padding;
		return ptr;
	}

	WeakReference<Base, Pad> padTop(SizeType padding) {
		auto ptr = Global::getState()->addOrModifySingle<Pad>();
		ptr.get()->top = padding;
		return ptr;
	}

	WeakReference<Base, Pad> padBot(SizeType padding) {
		auto ptr = Global::getState()->addOrModifySingle<Pad>();
		ptr.get()->bottom = padding;
		return ptr;
	}

	WeakReference<Base, Pad> padLeft(SizeType padding) {
		auto ptr = Global::getState()->addOrModifySingle<Pad>();
		ptr.get()->left = padding;
		return ptr;
	}

	WeakReference<Base, Pad> padRight(SizeType padding) {
		auto ptr = Global::getState()->addOrModifySingle<Pad>();
		ptr.get()->right = padding;
		return ptr;
	}

	WeakReference<Base, List> startList(DIR dir) {
		auto ref = Global::getManager().makeUniqueRef<List>(dir);
		auto res = ref.as<List>();

		Global::getState()->addMulti(std::move(ref));

		return res;
	}

	WeakReference<Base, Grid> startGrid(int32_t x, int32_t y) {
		auto ref = Global::getManager().makeUniqueRef<Grid>(glm::ivec2(x, y));
		auto res = ref.as<Grid>();

		Global::getState()->addMulti(std::move(ref));

		return res;
	}

	WeakReference<Base, List> menu(std::string const& text, std::optional<SizeType> width, std::function<void()> f) {
		auto list = startList(DIR::RIGHT);

		auto button = textButton(text);
		if (width.has_value()) {
			constrainWidth(width.value());
		}
		else {
			constrainWidth({ SIZETYPE::FH, 10.0f });
		}

		auto proxy = makeEnd<AnchoredProxy>();

		endList();

		button.get()->setOnPress([proxy = proxy.get(), f](PlayerInfo& playerInfo, Base* self_)->CallBackBindResult
		{
			Global::push();

			padTop({ SIZETYPE::PX, 3 });
			padRight({ SIZETYPE::PX, 3 });
			padLeft({ SIZETYPE::PX, 3 });
			background(COLORS::DARKEN2(COLORS::UI::BACKGROUND));

			f();

			proxy->setProxy(Global::pop(), playerInfo.uiState);

			return BIND::RESULT::CONTINUE;
		});

		return list;
	}

	WeakReference<Base, Button> textButton(std::string const& text) {
		return textButton2(text).first;
	}

	std::pair<WeakReference<Base, Button>, WeakReference<Base, TextDisplay>> textButton2(std::string const& text_) {
		pad({ SIZETYPE::PX, 1 });
		auto button = ui::button();
		alignCenter();
		auto text = textDisplaySingle(text_);

		return { button, text };
	}

	WeakReference<Base, TextDisplay> textEditSingle(std::string const& text) {
		auto res = Global::getManager().makeUniqueRef<TextDisplay>();
		auto ref = res.as<TextDisplay>();
		auto ptr = ref.get();

		ptr->text.addLine(text);

		ptr->mode = TEXTDISPLAY::MODE::INSERT;

		TEXTEDIT::clickSelect(ptr);
		BASE::activatable(ptr);

		TEXTEDIT::left(ptr);
		TEXTEDIT::right(ptr);

		TEXTEDIT::inputNoLineBreaks(ptr);

		TEXTEDIT::backspace(ptr);
		TEXTEDIT::del(ptr);
		TEXTEDIT::tab(ptr);

		makeEnd(std::move(res));
		return ref;
	}

	WeakReference<Base, TextDisplay> textEditMulti(std::vector<std::string> const& text, bool lineWrap) {
		auto ref = Global::getManager().makeUniqueRef<TextDisplay>(lineWrap);
		auto res = ref.as<TextDisplay>();
		auto ptr = ref.get();

		ptr->text.empty();
		for (auto& line : text) {
			ptr->text.addLine(line);
		}

		TEXTEDIT::clickSelect(ptr);
		BASE::activatable(ptr);

		TEXTEDIT::up(ptr);
		TEXTEDIT::down(ptr);
		TEXTEDIT::left(ptr);
		TEXTEDIT::right(ptr);

		TEXTEDIT::normalbinds(ptr);

		TEXTEDIT::normal(ptr);
		TEXTEDIT::insert(ptr);

		TEXTEDIT::input(ptr);

		TEXTEDIT::backspace(ptr);
		TEXTEDIT::del(ptr);
		TEXTEDIT::tab(ptr);

		TEXTEDIT::viewDown(ptr);
		TEXTEDIT::viewUp(ptr);

		makeEnd(std::move(ref));

		return res;
	}

	WeakReference<Base, TextDisplay> textEditMulti(std::string const& text, bool lineWrap) {
		std::vector<std::string> lines;
		misc::split(0, text, lines, '\n', true, false);
		return textEditMulti(lines, lineWrap);
	}

	WeakReference<Base, TextDisplay> textDisplaySingle(std::string const& text, bool shrinkToFit) {
		auto ref = Global::getManager().makeUniqueRef<TextDisplay>(true);
		auto res = ref.as<TextDisplay>();

		auto ptr = ref.get();
		ptr->text.setString(text);
		ptr->text.hideCursor();
		ptr->setShrinkToFit(shrinkToFit);

		makeEnd(std::move(ref));

		return res;
	}

	WeakReference<Base, TextDisplay> textDisplayMulti(std::vector<std::string> const& text, bool lineWrap) {
		auto ref = Global::getManager().makeUniqueRef<TextDisplay>(lineWrap);
		auto res = ref.as<TextDisplay>();

		auto ptr = ref.get();
		ptr->text.setLines(text);

		TEXTEDIT::viewDown(ptr);
		TEXTEDIT::viewUp(ptr);

		makeEnd(std::move(ref));

		return res;
	}

	WeakReference<Base, TextDisplay> textDisplayMulti(std::string const& text, bool lineWrap) {
		std::vector<std::string> temp{ text };
		return textDisplayMulti(temp, lineWrap);
	}

	void endList() {
		Global::getState()->pop<List>();
	}

	void endGrid() {
		Global::getState()->pop<Grid>();
	}
}
