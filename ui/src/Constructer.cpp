#include "Constructer.h"

#include "Proxy.h"
#include "TextDisplay.h"

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
		mainPadPtr->top = { ::SIZETYPE::FH, 1.2f };
		if (types & Window::TYPE::RESIZE) {
			types |= Window::TYPE::RESIZEHORIZONTAL | Window::TYPE::RESIZEVERTICAL;
		}
		if (types & Window::TYPE::RESIZEHORIZONTAL) {
			mainPadPtr->right = { ::SIZETYPE::PX, resizeSliverSize };
		}
		if (types & Window::TYPE::RESIZEVERTICAL) {
			mainPadPtr->bottom = { ::SIZETYPE::PX, resizeSliverSize };
		}

		// ------------------
		// Main Window object
		// ------------------

		Global::push();

		free();
		auto windowPtr = makeEnd<Window>();
		windowPtr.get()->addElement(std::move(mainPad));
		Binds::Base::focusable(windowPtr.get());
		Binds::Base::blockWorldBinds(windowPtr.get());
		windowPtr.get()->screenRectangle.set(size);

		UniqueReference<Base, Base> windowRef = Global::pop();

		// -------
		// Top Bar
		// -------

		Global::push();

		alignTop();
		constrainHeight({ ::SIZETYPE::FH, 1.2f });
		auto topBar = startList(::DIR::RIGHT_REVERSE);
		Binds::Base::focusable(topBar.get());
		Binds::Base::blockWorldBinds(topBar.get());

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
				[windowPtr = windowPtr.get()](PlayerState& playerState, Base* self_)->CallBackBindResult
			{
				auto self = static_cast<Button*>(self_);
				if (self->isDown()) {
					windowPtr->moveTopLeftTo(playerState.uiState.getCursorPositionScreenClamped(0.99f) - self->getMousePressOffset());
				}
				return BIND::RESULT::CONTINUE;
			});
		}
		else {
			pad({ ::SIZETYPE::PX, 1 });
			textDisplaySingle(title);
		}

		topBar.get()->addElement(Global::pop());

		// --------
		// Minimise
		// --------

		if (types & Window::TYPE::MINIMISE) {
			Global::push();

			constrainSize({ ::SIZETYPE::FH, 1.2f });
			auto button = textButton(" _");
			button.get()->setOnRelease([windowPtr = windowPtr.get()](PlayerState& playerState, Base* self_)->CallBackBindResult
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

			constrainSize({ ::SIZETYPE::FH, 1.2f });
			auto button = textButton(" x");

			if (types & Window::TYPE::CLOSE) {
				Binds::Button::close(button.get());
			}
			else if (types & Window::TYPE::HIDE) {
				Binds::Button::hide(button.get());

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
			constrainHeight({ ::SIZETYPE::PX, resizeSliverSize });
			padRight({ ::SIZETYPE::PX, types & Window::TYPE::RESIZEHORIZONTAL ? resizeSliverSize : 0 });
			padLeft({ ::SIZETYPE::PX, 1 });
			padTop({ ::SIZETYPE::PX, 1 });
			auto button = button();
			makeEnd<Empty>();

			button.get()->addFocussedBind(
				{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
				[windowPtr = windowPtr.get()](PlayerState& playerState, Base* self_)->CallBackBindResult
			{
				auto self = static_cast<Button*>(self_);
				if (self->isDown()) {
					auto bottomRight = windowPtr->screenRectangle.getBottomRight();
					bottomRight.y =
						playerState.uiState.getCursorPositionScreenClamped(0.99f).y
						- self->getMousePressOffset().y - self->screenRectangle.getAbsSize().y;
					if (windowPtr->screenRectangle.getTop() - bottomRight.y < 0.2f) {
						bottomRight.y = windowPtr->screenRectangle.getTop() - 0.2f;
					}
					windowPtr->screenRectangle.setBottomRight(bottomRight);
					windowPtr->updateSize(windowPtr->screenRectangle);
				}
				return BIND::RESULT::CONTINUE;
			});

			windowPtr.get()->addElementMulti(Global::pop());
		}

		if (types & Window::TYPE::RESIZEHORIZONTAL) {
			Global::push();

			alignRight();
			constrainWidth({ ::SIZETYPE::PX, resizeSliverSize });
			padLeft({ ::SIZETYPE::PX, 1 });
			padTop({ ::SIZETYPE::FH, 1.2f });
			padBot({ ::SIZETYPE::PX, types & Window::TYPE::RESIZEVERTICAL ? resizeSliverSize : 0 });
			auto button = button();
			makeEnd<Empty>();

			button.get()->addFocussedBind(
				{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
				[windowPtr = windowPtr.get()](PlayerState& playerState, Base* self_)->CallBackBindResult
			{
				auto self = static_cast<Button*>(self_);
				if (self->isDown()) {
					auto bottomRight = windowPtr->screenRectangle.getBottomRight();
					bottomRight.x =
						playerState.uiState.getCursorPositionScreenClamped(0.99f).x
						- self->getMousePressOffset().x + self->screenRectangle.getAbsSize().x;
					if (bottomRight.x - windowPtr->screenRectangle.getLeft() < 0.2f) {
						bottomRight.x = windowPtr->screenRectangle.getLeft() + 0.2f;
					}
					windowPtr->screenRectangle.setBottomRight(bottomRight);
					windowPtr->updateSize(windowPtr->screenRectangle);
				}
				return BIND::RESULT::CONTINUE;
			});

			windowPtr.get()->addElementMulti(Global::pop());
		}

		if (types & Window::TYPE::RESIZE) {
			Global::push();

			alignBottomRight();
			constrainWidth({ ::SIZETYPE::PX, resizeSliverSize });
			constrainHeight({ ::SIZETYPE::PX, resizeSliverSize });
			auto button = button();
			makeEnd<Empty>();

			button.get()->addFocussedBind(
				{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
				[windowPtr = windowPtr.get()](PlayerState& playerState, Base* self_)->CallBackBindResult
			{
				auto self = static_cast<Button*>(self_);
				if (self->isDown()) {
					auto bottomRight =
						playerState.uiState.getCursorPositionScreenClamped(0.99f)
						- self->getMousePressOffset() + glm::vec2(1.0f, -1.0f) * self->screenRectangle.getAbsSize();
					if (bottomRight.x - windowPtr->screenRectangle.getLeft() < 0.2f) {
						bottomRight.x = windowPtr->screenRectangle.getLeft() + 0.2f;
					}
					if (windowPtr->screenRectangle.getTop() - bottomRight.y < 0.2f) {
						bottomRight.y = windowPtr->screenRectangle.getTop() - 0.2f;
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

	WeakReference<Base, ConstrainSize> align(ui::ALIGNMENT alignment) {
		auto ptr = Global::getState()->addOrModifySingle<ConstrainSize>();
		ptr.get()->setAlignment(alignment);
		return ptr;
	}

	WeakReference<Base, ConstrainSize> alignCenter() {
		return align(ui::ALIGNMENT::CENTER);
	}

	WeakReference<Base, ConstrainSize> alignTop() {
		return align(ui::ALIGNMENT::TOP);
	}

	WeakReference<Base, ConstrainSize> alignBottom() {
		return align(ui::ALIGNMENT::BOTTOM);
	}

	WeakReference<Base, ConstrainSize> alignLeft() {
		return align(ui::ALIGNMENT::LEFT);
	}

	WeakReference<Base, ConstrainSize> alignRight() {
		return align(ui::ALIGNMENT::RIGHT);
	}

	WeakReference<Base, ConstrainSize> alignBottomLeft() {
		return align(ui::ALIGNMENT::BOTTOMLEFT);
	}

	WeakReference<Base, ConstrainSize> alignBottomRight() {
		return align(ui::ALIGNMENT::BOTTOMRIGHT);
	}

	WeakReference<Base, ConstrainSize> alignTopLeft() {
		return align(ui::ALIGNMENT::TOPLEFT);
	}

	WeakReference<Base, ConstrainSize> alignTopRight() {
		return align(ui::ALIGNMENT::TOPRIGHT);
	}

	WeakReference<Base, FreeSize> free() {
		auto ref = Global::getManager().makeUniqueRef<FreeSize>();
		auto res = ref.as<FreeSize>();

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

	WeakReference<Base, List> startList(ui::DIR dir) {
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
		auto list = startList(::DIR::RIGHT);

		auto button = textButton(text);
		if (width.has_value()) {
			constrainWidth(width.value());
		}
		else {
			constrainWidth({ ::SIZETYPE::FH, 10.0f });
		}

		auto proxy = makeEnd<AnchoredProxy>();

		endList();

		button.get()->setOnPress([proxy = proxy.get(), f](PlayerState& playerState, Base* self_)->CallBackBindResult
		{
			Global::push();

			padTop({ ::SIZETYPE::PX, 3 });
			padRight({ ::SIZETYPE::PX, 3 });
			padLeft({ ::SIZETYPE::PX, 3 });
			background(COLORS::DARKEN2(COLORS::UI::BACKGROUND));

			f();

			proxy->setProxy(Global::pop(), playerState.uiState);

			return BIND::RESULT::CONTINUE;
		});

		return list;
	}

	WeakReference<Base, Button> textButton(std::string const& text) {
		return textButton2(text).first;
	}

	std::pair<WeakReference<Base, Button>, WeakReference<Base, TextDisplay>> textButton2(std::string const& text_) {
		pad({ ::SIZETYPE::PX, 1 });
		auto button = button();
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

		Binds::TextEdit::clickSelect(ptr);
		Binds::Base::activatable(ptr);

		Binds::TextEdit::left(ptr);
		Binds::TextEdit::right(ptr);

		Binds::TextEdit::inputNoLineBreaks(ptr);

		Binds::TextEdit::backspace(ptr);
		Binds::TextEdit::del(ptr);
		Binds::TextEdit::tab(ptr);

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

		Binds::TextEdit::clickSelect(ptr);
		Binds::Base::activatable(ptr);

		Binds::TextEdit::up(ptr);
		Binds::TextEdit::down(ptr);
		Binds::TextEdit::left(ptr);
		Binds::TextEdit::right(ptr);

		Binds::TextEdit::normalbinds(ptr);

		Binds::TextEdit::normal(ptr);
		Binds::TextEdit::insert(ptr);

		Binds::TextEdit::input(ptr);

		Binds::TextEdit::backspace(ptr);
		Binds::TextEdit::del(ptr);
		Binds::TextEdit::tab(ptr);

		Binds::TextEdit::viewDown(ptr);
		Binds::TextEdit::viewUp(ptr);

		makeEnd(std::move(ref));

		return res;
	}

	WeakReference<Base, TextDisplay> textEditMulti(std::string const& text, bool lineWrap) {
		std::vector<std::string> lines;
		split(0, text, lines, '\n', true);
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

		Binds::TextEdit::viewDown(ptr);
		Binds::TextEdit::viewUp(ptr);

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
