#pragma once

#include "../infos/TextRenderInfo.h"
#include "../infos/UIRenderInfo.h"
#include "../infos/CameraInfo.h"
#include "../infos/TileRenderInfo.h"

namespace render
{
	struct RenderInfo
	{
		glm::ivec2 frameSize;
		CameraInfo cameraInfo;
		TextRenderInfo textRenderInfo;
		UIRenderInfo uiRenderInfo;
		TileRenderInfo tileRenderInfo;
	};
}