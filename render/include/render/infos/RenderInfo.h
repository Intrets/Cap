#pragma once

#include "../infos/TextRenderInfo.h"
#include "../infos/UIRenderInfo.h"
#include "../infos/CameraInfo.h"

namespace render
{
	struct RenderInfo
	{
		TextRenderInfo textRenderInfo;
		UIRenderInfo uiRenderInfo;
		CameraInfo cameraInfo;
	};
}