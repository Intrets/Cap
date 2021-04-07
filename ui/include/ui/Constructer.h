#pragma once

#include <vector>

#include <mem/ReferenceManager.h>

#include "Base.h"
#include "Enums.h"
#include "Proxy.h"
#include "List.h"

namespace ui
{
	struct SizeType;
	class TextDisplay;
	class Button;
	class ConstrainSize;
	class Window;
	class Hideable;
	class Grid;
	class ColoredBackground;
	class FreeSize;
	class Pad;
	class Destructible;
	class InitialSize;

	struct ConstructerState
	{
		UniqueReference<Base, Proxy> root;
		std::vector<WeakReference<Base, Base>> stack;

		UniqueReference<Base, Base> singlesRoot;
		WeakReference<Base, Base> singlesLeaf;

		void addSingle(UniqueReference<Base, Base> ref);
		void addSingle(UniqueReference<Base, Base> ref, WeakReference<Base, Base> leaf);
		void addMulti(UniqueReference<Base, Base> ref);
		void addEnd(UniqueReference<Base, Base> ref);

		template<class T>
		void pop();

		template<class T>
		WeakReference<Base, T> addOrModifySingle();

		template<class T>
		WeakReference<Base, T> addSingle();
	};

	class Global
	{
	private:
		static ReferenceManager<Base> manager;

	public:
		static std::vector<std::unique_ptr<ConstructerState>> states;

		static ReferenceManager<Base>& getManager();

		static ConstructerState* getState();

		static void push();

		static UniqueReference<Base, Base> pop();
	};

	WeakReference<Base, TextDisplay> text(std::string const& t, bool shrinkToFit = true);
	WeakReference<Base, Button> button(bool shrinkToFit = false);
	WeakReference<Base, Window> window(std::string const& title, Rect size, int32_t types);
	WeakReference<Base, Hideable> hideable();

	WeakReference<Base, ColoredBackground> background(glm::vec4 color);

	WeakReference<Base, ConstrainSize> constrainHeight(SizeType height);
	WeakReference<Base, ConstrainSize> constrainWidth(SizeType width);
	WeakReference<Base, ConstrainSize> constrainSize(SizeType size);
	WeakReference<Base, ConstrainSize> align(ui::ALIGNMENT alignment);
	WeakReference<Base, ConstrainSize> alignCenter();
	WeakReference<Base, ConstrainSize> alignTop();
	WeakReference<Base, ConstrainSize> alignBottom();
	WeakReference<Base, ConstrainSize> alignLeft();
	WeakReference<Base, ConstrainSize> alignRight();
	WeakReference<Base, ConstrainSize> alignBottomLeft();
	WeakReference<Base, ConstrainSize> alignBottomRight();
	WeakReference<Base, ConstrainSize> alignTopLeft();
	WeakReference<Base, ConstrainSize> alignTopRight();

	WeakReference<Base, FreeSize> free();
	WeakReference<Base, InitialSize> initial(Rect rect);

	WeakReference<Base, Destructible> destructible();

	WeakReference<Base, Pad> pad(SizeType padding);
	WeakReference<Base, Pad> padTop(SizeType padding);
	WeakReference<Base, Pad> padBot(SizeType padding);
	WeakReference<Base, Pad> padLeft(SizeType padding);
	WeakReference<Base, Pad> padRight(SizeType padding);

	WeakReference<Base, List> startList(ui::DIR dir);
	WeakReference<Base, Grid> startGrid(int32_t x, int32_t y);
	WeakReference<Base, List> menu(std::string const& text, std::optional<SizeType> width, std::function<void()> f);

	WeakReference<Base, Button> textButton(std::string const& text);
	std::pair<WeakReference<Base, Button>, WeakReference<Base, TextDisplay>> textButton2(std::string const& text);

	WeakReference<Base, TextDisplay> textEditSingle(std::string const& text);
	WeakReference<Base, TextDisplay> textEditMulti(std::vector<std::string> const& text, bool lineWrap = true);
	WeakReference<Base, TextDisplay> textEditMulti(std::string const& text, bool lineWrap = true);
	WeakReference<Base, TextDisplay> textDisplaySingle(std::string const& text, bool shrinkToFit = true);
	WeakReference<Base, TextDisplay> textDisplayMulti(std::vector<std::string> const& text, bool lineWrap = true);
	WeakReference<Base, TextDisplay> textDisplayMulti(std::string const& text, bool lineWrap = true);

	void endList();
	void endGrid();

	template<class T, class... Args> WeakReference<Base, T> makeSingle(Args&&... args);

	template<class T> WeakReference<Base, T> makeEnd(UniqueReference<Base, T> ref);

	template<class T, class... Args> WeakReference<Base, T> makeEnd(Args&&... args);


	template<class T, class... Args>
	WeakReference<Base, T> makeSingle(Args&&... args) {
		static_assert(std::is_base_of<Base, T>::value);
		auto ref = ui::Global::getManager().makeUniqueRef<T>(std::forward<Args>(args)...);
		auto ptr = ref.get();

		Global::getState()->addSingle(std::move(ref));

		return ptr;
	}

	template<class T>
	WeakReference<Base, T> makeEnd(UniqueReference<Base, T> ref) {
		static_assert(std::is_base_of<Base, T>::value);
		auto res = ref.as<T>();

		Global::getState()->addEnd(std::move(ref));

		return res;
	}

	template<class T, class... Args>
	WeakReference<Base, T> makeEnd(Args&&... args) {
		static_assert(std::is_base_of<Base, T>::value);
		auto ref = Global::getManager().makeUniqueRef<T>(std::forward<Args>(args)...);
		auto res = ref.as<T>();

		Global::getState()->addEnd(std::move(ref));

		return res;
	}

	template<class T>
	inline void ConstructerState::pop() {
		assert(this->stack.size() > 1);
		assert(this->stack.back().get()->getUIOType() == GET_TYPE<T>());

		ConstructerState::stack.pop_back();
	}

	template<class T>
	WeakReference<Base, T> ConstructerState::addOrModifySingle() {
		static_assert(GET_TYPE<T>() != TYPE::UNSPECIFIED);

		WeakReference<Base, Base> leaf;

		if (this->singlesLeaf.isNull() ||
			this->singlesLeaf.get()->getUIOType() != GET_TYPE<T>()) {

			this->addSingle(std::move(Global::getManager().makeUniqueRef<T>()));
		}

		return this->singlesLeaf.as<T>();
	}

	template<class T>
	inline WeakReference<Base, T> ConstructerState::addSingle() {
		auto ref = Global::getManager().makeUniqueRef<T>();
		auto res = ref.as<T>();

		this->addSingle(std::move(ref));

		return res;
	}
}
