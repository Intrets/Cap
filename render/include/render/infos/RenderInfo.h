#pragma once

#include "../infos/TextRenderInfo.h"
#include "../infos/UIRenderInfo.h"

namespace render
{
	struct RenderInfo
	{
		TextRenderInfo textRenderInfo;
		UIRenderInfo uiRenderInfo;
	};
}