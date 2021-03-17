#pragma once

#include <wglm/glm.hpp>
#include <vector>
#include <map>
#include <optional>
#include <string>

#include <misc/Rectangle.h>

#include "../Fonts.h"
#include "../infos/BlitRenderInfo.h"

namespace render
{
	struct WindowTextRenderInfo
	{
	public:
		ScreenRectangle screenRectangle;

		// layering for UI rendering
		float depth = 0.0f;

		// view offset for render info
		glm::vec2 offset;

		// total size of entire text in [-1, 1] in screenRectangle space
		// value in [0, inf)
		glm::vec2 renderedSize{ 0.0f, 0.0f };

		BlitRenderInfo textRenderInfo;

		// next position for character in screenRectangle space
		glm::vec2 nextPos = { -1.0f, 1.0f };

		// current line width
		float laneHeight = 0.0f;

		// render with linewrap
		bool lineWrap;

		struct cmp
		{
			bool operator()(const glm::vec2& a, const glm::vec2& b) const {
				return (a[0] < b[0]) && (a[1] < b[1]);
			}
		};

		// click to select characters support
		bool clickSelect;

		// lines for click cursor selecting
		std::map<glm::vec2, std::map<glm::vec2, int32_t, cmp>, cmp> lines;

		WindowTextRenderInfo(ScreenRectangle rect, bool lineWrap = false, bool clickSupport_ = false);

		void addString(render::FONT font, std::string text);
		void newLine();
		void setDepth(int32_t layer);
		void setDepth(float depth_);

		glm::vec2 getRenderedScreenSize();

		std::optional<int32_t> getIndex(glm::vec2 p);

		// first 2 Coordinates in [-1, 1] this->screenRectangle space.
		// second 2 size in same space.
		std::optional<glm::vec4> getCursorPos(int32_t index) const;
	};

	struct TextRenderInfo
	{
		std::vector<WindowTextRenderInfo> windowTextRenderInfos;
	};
}