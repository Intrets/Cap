#pragma once

#include <glm/glm.hpp>

#include <ui/impl/Text.h>

#include "CameraInfo.h"
#include "TextRenderInfo.h"
#include "UIRenderInfo.h"

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