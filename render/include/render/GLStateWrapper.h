#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum class BLEND
{
	UNSET,
	ENABLED,
	DISABLED,
	MAX,
};

enum class BLEND_FUNC
{
	UNSET,
	SRC_ALPHA__ONE_MINUS_SRC_ALPHA,
	SRC_ONE__ONE_MINUS_SRC_ALPHA,
	MAX,
};

enum class DEPTH_TEST
{
	UNSET,
	ENABLED,
	DISABLED,
	MAX,
};

enum class DEPTH_FUNC
{
	UNSET,
	LESS,
	MAX,
};

enum class POLYGON_MODE
{
	UNSET,
	FILL,
	LINE,
	POINT,
	MAX,
};

namespace ogs
{
	struct State;

	struct Configuration
	{
	public:
		BLEND blend;
		BLEND_FUNC blendFunc;
		DEPTH_TEST depthTest;
		DEPTH_FUNC depthFunc;
		POLYGON_MODE polygonMode;

		Configuration(
			BLEND blend,
			BLEND_FUNC blendFunc,
			DEPTH_TEST depthTest,
			DEPTH_FUNC depthFunc,
			POLYGON_MODE polygonMode
		);

	private:
		Configuration();

		friend struct State;
	};

	Configuration TextConfiguration();
	Configuration UIBackground();
	Configuration FontAtlasConfiguration();
	Configuration WorldTileConfiguration();
	Configuration BlockIDConfiguration();

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
