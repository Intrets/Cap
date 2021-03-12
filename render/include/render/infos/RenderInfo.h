#pragma once

#include "../infos/TextRenderInfo.h"
#include "../infos/UIRenderInfo.h"
#include "../infos/CameraInfo.h"

namespace render
{
	struct RenderInfo
	{
		glm::ivec2 frameSize;
		CameraInfo cameraInfo;
		TextRenderInfo textRenderInfo;
		UIRenderInfo uiRenderInfo;
	};
}