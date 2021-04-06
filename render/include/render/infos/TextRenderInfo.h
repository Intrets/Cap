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

		glm::ivec2 renderedSize{ 0, 0 };

		BlitRenderInfo textRenderInfo;

		glm::ivec2 nextPos = { 0,0 };

		int32_t laneHeight = 0;

		bool lineWrap;

		struct cmp
		{
			bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
				return (a[0] < b[0]) && (a[1] < b[1]);
			}
		};

		// click to select characters support
		bool clickSelect;

		// lines for click cursor selecting
		std::map<glm::ivec2, std::map<glm::ivec2, int32_t, cmp>, cmp> lines;

		WindowTextRenderInfo(ScreenRectangle rect, bool lineWrap = false, bool clickSupport_ = false);

		void addString(render::FONT font, std::string text);
		void newLine();
		void setDepth(int32_t layer);
		void setDepth(float depth_);

		glm::ivec2 getRenderedScreenSize();

		std::optional<int32_t> getIndex(glm::ivec2 p);

		// first 2 Coordinates in [-1, 1] this->screenRectangle space.
		// second 2 size in same space.
		std::optional<glm::vec4> getCursorPos(int32_t index) const;
	};

	struct TextRenderInfo
	{
		std::vector<WindowTextRenderInfo> windowTextRenderInfos;
	};
}