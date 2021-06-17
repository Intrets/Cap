#include "impl/Text.h"

#include <render/infos/RenderInfo.h>
#include <render/Enums.h>
#include <render/Colors.h>
#include <render/Fonts.h>
#include <misc/Misc.h>
#include <misc/FunctionHelpers.h>
#include <mem/Global.h>

namespace ui
{
	namespace impl
	{
		void Text::validateView() {
			if (!this->cachedRenderInfo.has_value()) {
				this->makeRenderInfo(this->lastScreenRectangle, this->lastFont, this->lastWrap, this->lastClickSupport);
			}

			if (auto const& maybeCursorQuad = this->cachedRenderInfo.value().getCursorPos(cursorIndex)) {
				auto& view = this->viewCache.getVal();

				glm::vec4 const& cursorQuad = maybeCursorQuad.value();
				Rect cursorRect;
				cursorRect.setBottomLeft(glm::vec2(cursorQuad[0], cursorQuad[1]));
				cursorRect.setTopRight(cursorRect.getBottomLeft() + glm::vec2(cursorQuad[2], cursorQuad[3]));

				Rect viewRect;
				viewRect.setBottomLeft(glm::vec2(-1.0f) + view);
				viewRect.setTopRight(glm::vec2(1.0f) + view);

				float leftDist = cursorRect.getLeft() - viewRect.getLeft();
				float rightDist = viewRect.getRight() - cursorRect.getRight();

				float botDist = cursorRect.getBot() - viewRect.getBot();
				float topDist = viewRect.getTop() - cursorRect.getTop();

				if (leftDist < 0.0f) {
					view.x += leftDist;
				}
				else if (rightDist < 0.0f) {
					view.x -= rightDist;
				}

				if (botDist < 0.0f) {
					view.y += botDist;
				}
				else if (topDist < 0.0f) {
					view.y -= topDist;
				}
			}

			this->viewCache.validate();
		}

		glm::vec2 Text::getView() {
			if (!this->viewCache.isValid()) {
				this->validateView();
			}
			return this->viewCache.getVal();
		}

		std::string Text::getSelectedLine() {
			return this->lines[this->getCursor().y];
		}

		std::vector<std::string> const& Text::getLines() const {
			return this->lines;
		}

		std::vector<std::string>& Text::getLinesMutable() {
			return this->lines;
		}

		std::optional<Rect> Text::getCursorQuadScreen() {
			if (!this->cachedRenderInfo.has_value()) {
				return std::nullopt;
			}

			auto const& maybeCursorShape = this->cachedRenderInfo.value().getCursorPos(this->cursorIndex);

			if (!maybeCursorShape.has_value()) {
				return std::nullopt;
			}
			auto const& cursorShape = maybeCursorShape.value();

			glm::vec2 a = (glm::vec2(cursorShape[0], cursorShape[1]) - this->getView()) / 2.0f + 0.5f;
			glm::vec2 b = glm::vec2(cursorShape[2], cursorShape[3]) / 2.0f;

			a *= this->lastScreenRectangle.sizeScreen();
			b *= this->lastScreenRectangle.sizeScreen();

			a += this->lastScreenRectangle.getBottomLeftScreen();
			b += a;

			return Rect{ a, b };
		}

		void Text::invalidateCache() {
			this->cachedRenderInfo = std::nullopt;
			//this->viewCache.invalidate();
			this->cursorCache.invalidate();
		}

		void Text::makeRenderInfo(ScreenRectangle screenRectangle, render::FONT font, bool wrap, bool clickSupport) {
			this->lastScreenRectangle = screenRectangle;

			this->lastFont = font;
			this->lastWrap = wrap;
			this->lastClickSupport = clickSupport;
			render::WindowTextRenderInfo textInfo(screenRectangle, wrap, clickSupport);

			for (auto& line : lines) {
				textInfo.addString(font, line);
			}

			this->lastRenderedSize = textInfo.renderedSize;
			this->cachedRenderInfo = std::move(textInfo);
		}

		int32_t Text::addRenderInfo(
			ScreenRectangle screenRectangle,
			render::RenderInfo& renderInfo,
			render::FONT font,
			int32_t depth,
			bool wrap,
			int32_t tick,
			bool renderCursor,
			bool clickSupport,
			CURSOR::TYPE cursorType) {

			if (!this->cachedRenderInfo.has_value()) {
				this->makeRenderInfo(screenRectangle, font, wrap, clickSupport);
			}

			auto& textRenderInfo = this->cachedRenderInfo.value();

			textRenderInfo.offset = -this->getView();
			textRenderInfo.setDepth(depth++);
			renderInfo.textRenderInfo.windowTextRenderInfos.push_back(textRenderInfo);

			if (renderCursor && periodic(tick, 30, 30)) {
				if (auto const& maybeQuad = this->getCursorQuadScreen()) {
					auto quad = maybeQuad.value();

					switch (cursorType) {
						case CURSOR::TYPE::BLOCK:
							break;
						case CURSOR::TYPE::LINE:
						{
							auto width = quad.getWidth();
							quad.setWidth(quad.getWidth() * 0.4f);
							quad.translate({ -width * 0.2f, 0 });
							break;
						}
						default:
							assert(0);
							break;
					}

					glm::vec2 bottomLeft = glm::max(screenRectangle.getBottomLeftScreen(), glm::min(screenRectangle.getTopRightScreen(), quad.getBottomLeft()));
					glm::vec2 topRight = glm::max(screenRectangle.getBottomLeftScreen(), glm::min(screenRectangle.getTopRightScreen(), quad.getTopRight()));


					renderInfo.uiRenderInfo.addRectangle(bottomLeft, topRight, COLORS::UI::CURSOR, depth++);
				}
			}

			return depth;
		}

		bool Text::deleteChar() {
			auto& cursor = this->cursorCache.getVal();

			if (cursor.y >= this->lines.size() - 1) {
				return false;
			}
			if (cursor.y == this->lines.size() - 2 && cursor.x == this->lines[cursor.y].size() - 1) {
				return false;
			}
			if (cursor.x == this->lines[cursor.y].size() - 1) {
				this->lines[cursor.y].erase(this->lines[cursor.y].end() - 1);
				this->lines[cursor.y] += this->lines[cursor.y + 1];
				this->lines.erase(this->lines.begin() + cursor.y + 1, this->lines.begin() + cursor.y + 2);
			}
			else {
				this->lines[cursor.y].erase(cursor.x, 1);
				cursor.x = glm::max(cursor.x, 0);
				this->cursorCache.invalidate();
			}

			this->invalidateCache();
			return true;
		}

		bool Text::backspaceChar() {
			auto& cursor = this->cursorCache.getVal();

			if (cursor == glm::ivec2(0)) {
				return false;
			}
			if (cursor.x == 0) {
				cursor.x = static_cast<int32_t>(this->lines[cursor.y - 1].size() - 1);
				cursor.x = glm::max(0, cursor.x);
				this->lines[cursor.y - 1].erase(this->lines[cursor.y - 1].end() - 1);
				this->lines[cursor.y - 1] += this->lines[cursor.y];
				this->lines.erase(this->lines.begin() + cursor.y, this->lines.begin() + cursor.y + 1);
				cursor.y--;
			}
			else {
				this->lines[cursor.y].erase(cursor.x - 1, 1);
				cursor.x = glm::max(cursor.x - 1, 0);
				cursorIndex = glm::max(cursorIndex - 1, 0);
			}

			this->invalidateCache();
			return true;
		}

		void Text::insertString(std::string text) {
			auto& cursor = this->cursorCache.getVal();

			for (auto c : text) {
				if (c == '\n') {
					auto first = std::string(this->lines[cursor.y].begin(), this->lines[cursor.y].begin() + cursor.x);
					auto second = std::string(this->lines[cursor.y].begin() + cursor.x, this->lines[cursor.y].end());

					this->lines[cursor.y] = first + "\n";
					cursor.x = 0;
					cursor.y++;
					cursorIndex++;
					this->lines.insert(this->lines.begin() + cursor.y, second);
				}
				else {
					this->lines[cursor.y].insert(cursor.x, std::string(1, c));
					cursor.x++;
					cursorIndex++;
				}
			}

			this->invalidateCache();
		}

		std::optional<glm::ivec2> Text::findPrevious(glm::ivec2 p, std::function<bool(char c)> f) {
			for (int32_t x = p.x; x >= 0; x--) {
				if (f(this->lines[p.y][x])) {
					return { { x, p.y } };
				}
			}

			for (int32_t line = p.y - 1; line >= 0; line--) {
				for (int32_t x = static_cast<int32_t>(this->lines[line].size() - 1); x >= 0; x--) {
					if (f(this->lines[line][x])) {
						return { { x, line } };
					}
				}
			}

			return std::nullopt;
		}

		std::optional<glm::ivec2> Text::findNext(glm::ivec2 p, std::function<bool(char c)> f) {
			for (int32_t x = p.x; x < this->lines[p.y].size(); x++) {
				if (f(this->lines[p.y][x])) {
					return { { x, p.y } };
				}
			}

			for (int32_t line = p.y + 1; line < this->lines.size(); line++) {
				for (int32_t x = 0; x < this->lines[line].size(); x++) {
					if (f(this->lines[line][x])) {
						return { { x, line } };
					}
				}
			}

			return std::nullopt;
		}

		void Text::moveStartWordBackward() {
			this->moveCursor(-1);
			if (auto new1 = this->findPrevious(this->getCursor(), isalnum)) {
				if (auto new2 = this->findPrevious(new1.value(), FUNC_NOT(isalnum))) {
					this->setCursor(new2.value());
				}
			}
			this->moveCursor(1);
		}

		void Text::moveEndWord() {
			this->moveCursor(1);
			if (auto new1 = this->findNext(this->getCursor(), isalnum)) {
				if (auto new2 = this->findNext(new1.value(), FUNC_NOT(isalnum))) {
					this->setCursor(new2.value());
				}
			}
			this->moveCursor(-1);
		}

		bool Text::insertLineAfter() {
			auto cursor = this->getCursor();
			cursor.x = 0;

			if (auto spaces = this->findNext(cursor, FUNC_NOT(isspace))) {
				auto spacesSize = spaces.value().x;
				if (this->moveCursor({ 0,1 })) {
					this->startOfLine();
					std::string line(spacesSize, ' ');
					this->insertString(line + "\n");
					this->moveCursor(-1);
					return true;
				}
			}

			this->setCursor(cursor);
			return false;
		}

		void Text::matchWhiteSpace() {
			auto cursor = this->getCursor();

			if (cursor.y > 0) {
				if (auto l1size = this->findNext({ 0, cursor.y - 1 }, FUNC_NOT(isblank))) {
					if (auto l2size = this->findNext({ 0, cursor.y }, FUNC_NOT(isblank))) {
						if (l2size.value().x < l1size.value().x) {
							auto size = l1size.value().x - l2size.value().x;

							std::string spaces(size, ' ');
							this->startOfLine();
							this->insertString(spaces);
							this->setCursor({ cursor.x + size, cursor.y });
						}
					}
				}
			}
		}

		void Text::hideCursor() {
			this->cursorIndex = -1;
		}

		bool Text::moveCursor(glm::ivec2 p) {
			auto cursor = this->cursorCache.getVal();

			cursor += p;
			cursor = glm::max(cursor, glm::ivec2(0));

			if (this->lines.size() == 1) {
				cursor.y = 0;
			}
			else {
				cursor.y = glm::clamp(cursor.y, 0, static_cast<int32_t>(this->lines.size()) - 2);
			}

			cursor.x = glm::clamp(cursor.x, 0, static_cast<int32_t>(this->lines[cursor.y].size() - 1));

			cursorIndex = 0;
			for (int32_t line = 0; line < cursor.y; line++) {
				cursorIndex += static_cast<int32_t>(this->lines[line].size());
			}
			cursorIndex += cursor.x;

			if (this->cursorCache.getVal() != cursor) {
				this->cursorCache = cursor;
				this->viewCache.invalidate();
				return true;
			}
			else {
				return false;
			}
		}

		bool Text::moveCursor(int32_t p) {
			auto cursor = this->cursorCache.getVal();

			while (p < 0) {
				if (-p <= cursor.x) {
					cursor.x += p;
					p = 0;
				}
				else {
					p += cursor.x;
					cursor.y--;

					if (cursor.y < 0) {
						cursor.x = 0;
						cursor.y = 0;
						break;
					}

					cursor.x = static_cast<int32_t>(this->lines[cursor.y].size());
				}
			}

			while (p > 0) {
				if (p <= static_cast<int32_t>(this->lines[cursor.y].size()) - cursor.x) {
					cursor.x += p;
					p = 0;
				}
				else {
					p -= cursor.x;
					cursor.y++;

					if (cursor.y >= this->lines.size()) {
						cursor.y = static_cast<int32_t>(this->lines.size()) - 1;
						cursor.x = static_cast<int32_t>(this->lines[cursor.y].size()) - 1;
						break;
					}

					cursor.x = 0;
				}
			}

			if (this->cursorCache.getVal() != cursor) {
				this->cursorIndex = 0;
				for (int32_t line = 0; line < cursor.y; line++) {
					this->cursorIndex += static_cast<int32_t>(this->lines[line].size());
				}
				this->cursorIndex += cursor.x;

				this->cursorCache = cursor;
				this->viewCache.invalidate();
				return true;
			}
			else {
				return false;
			}
		}

		void Text::setCursor(glm::ivec2 p) {
			this->zeroCursor();
			this->moveCursor(p);
		}

		void Text::moveView(glm::ivec2 p) {
			int32_t pxHeight = Global<render::Fonts>::ref().getFont(this->lastFont).charSize[0].y;

			if (this->lastScreenRectangle.getPixelSize().y == 0) {
				return;
			}

			auto height = this->lastScreenRectangle.getHeight();

			float lineHeight = 2.0f * static_cast<float>(pxHeight) / height;

			auto& view = this->viewCache.getVal();

			view += lineHeight * glm::vec2(p) * 1.4f;

			// margin from top
			float topStop = this->lastRenderedSize.y * 2.0f / height;

			// margin from bottom
			float botStop = lineHeight;

			if (view.y < 0.0f) {
				if (topStop + view.y < lineHeight) {
					view.y = lineHeight - topStop;
				}
			}
			else {
				if (view.y > botStop - lineHeight) {
					view.y = botStop - lineHeight;
				}
			}
			this->viewCache.validate();
		}

		void Text::startOfLine() {
			auto cursor = this->getCursor();
			cursor.x = 0;
			this->setCursor(cursor);
		}

		void Text::moveStartWordForward() {
			if (auto new1 = this->findNext(this->getCursor(), FUNC_NOT(isalnum))) {
				if (auto new2 = this->findNext(new1.value(), isalnum)) {
					this->setCursor(new2.value());
				}
			}
		}

		void Text::zeroCursor() {
			this->cursorCache = glm::ivec2(0, 0);
			this->viewCache.invalidate();
		}

		glm::ivec2 Text::getCursor() {
			return this->cursorCache.getVal();
		}

		void Text::selectIndex(int32_t index) {
			int32_t lineIndex = 0;
			int32_t i = index;

			for (auto& line : this->lines) {
				if (i - static_cast<int32_t>(line.size()) < 0) {
					this->cursorCache = glm::ivec2(i, lineIndex);
					this->cursorIndex = index;
					return;
				}
				i -= static_cast<int32_t>(line.size());
				lineIndex++;
			}
		}

		void Text::empty() {
			this->lines = { "" };
			this->invalidateCache();
		}

		void Text::addLine(std::string text) {
			this->lines.back().append(text + "\n");
			this->lines.push_back("");
			this->invalidateCache();
		}

		void Text::setString(std::string text) {
			this->empty();
			this->addLine(text);
		}

		void Text::setLines(std::vector<std::string> lines_) {
			this->lines = {};
			for (auto& line : lines_) {
				lines.push_back(line + "\n");
			}
			this->lines.push_back("");

			this->cursorCache = glm::ivec2(0, 0);
			this->cursorIndex = 0;
			this->invalidateCache();
		}

	}
}