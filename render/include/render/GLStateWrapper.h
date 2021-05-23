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
	class State;

	class Configuration
	{
	public:
		BLEND blend = BLEND::UNSET;
		BLEND_FUNC blendFunc = BLEND_FUNC::UNSET;
		DEPTH_TEST depthTest = DEPTH_TEST::UNSET;
		DEPTH_FUNC depthFunc = DEPTH_FUNC::UNSET;
		POLYGON_MODE polygonMode = POLYGON_MODE::UNSET;
		float pointSize = 0.0f;

		Configuration(
			BLEND blend,
			BLEND_FUNC blendFunc,
			DEPTH_TEST depthTest,
			DEPTH_FUNC depthFunc,
			POLYGON_MODE polygonMode,
			float pointSize
		);

	private:
		Configuration() = default;

		friend class State;
	};

	Configuration TextConfiguration();
	Configuration UIBackground();
	Configuration FontAtlasConfiguration();
	Configuration WorldTileConfiguration();
	Configuration BlockIDConfiguration();
	Configuration DebugPointConfiguration();
	Configuration DebugLineConfiguration();

	class State
	{
	public:
		Configuration configuration;

		int32_t MAX_COLOR_ATTACHMENTS;

		State();

		void setState(Configuration const& config);

		void setBlend(BLEND b);
		void setBlendFunc(BLEND_FUNC func);
		void setDepthTest(DEPTH_TEST test);
		void setDepthFunc(DEPTH_FUNC func);
		void setPolygonMode(POLYGON_MODE mode, float pointSize);
	};
}
