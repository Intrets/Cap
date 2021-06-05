#pragma once

#include "../infos/TextRenderInfo.h"
#include "../infos/UIRenderInfo.h"
#include "../infos/CameraInfo.h"
#include "../infos/BlitArrayRenderInfo.h"
#include "../infos/HighlightRenderInfo.h"

namespace render
{
	struct RenderInfo
	{
		glm::ivec2 frameSize;
		CameraInfo cameraInfo;
		TextRenderInfo textRenderInfo;
		UIRenderInfo uiRenderInfo;
		BlitArrayRenderInfo tileRenderInfo;
		HighlightRenderInfo highlightRenderInfo;
	};
}