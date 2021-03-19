#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum class BLEND
{
	ENABLED,
	DISABLED,
	MAX,
};

enum class BLEND_FUNC
{
	SRC_ALPHA__ONE_MINUS_SRC_ALPHA,
	SRC_ONE__ONE_MINUS_SRC_ALPHA,
	MAX,
};

enum class DEPTH_TEST
{
	ENABLED,
	DISABLED,
	MAX,
};

enum class DEPTH_FUNC
{
	LESS,
	MAX,
};

enum class POLYGON_MODE
{
	FILL,
	LINE,
	POINT,
	MAX,
};

namespace ogs
{
	struct Configuration
	{
		BLEND blend;
		BLEND_FUNC blendFunc;
		DEPTH_TEST depthTest;
		DEPTH_FUNC depthFunc;
		POLYGON_MODE polygonMode;
	};

	Configuration TextConfiguration();
	Configuration UIBackground();

	struct State
	{
		Configuration configuration;

		int32_t MAX_COLOR_ATTACHMENTS;

		State();

		void setState(Configuration const& config);

		void setBlend(BLEND b);
		void setBlendFunc(BLEND_FUNC func);
		void setDepthTest(DEPTH_TEST test);
		void setDepthFunc(DEPTH_FUNC func);
		void setPolygonMode(POLYGON_MODE mode);
	};
}
