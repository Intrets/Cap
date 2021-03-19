#pragma once

#include <GL/glew.h>

#include "infos/UIRenderInfo.h"
#include "BufferWrappers.h"
#include "GLStateWrapper.h"

#include "../shaders/UIBackground.vert.inc"
#include "../shaders/UIBackground.frag.inc"

namespace render
{
	struct UIRenderInfo;
	struct CameraInfo;

	class UIBackgroundRenderer
	{
	private:
		bwo::Program program{ UIBackground_vert, UIBackground_frag, "UIBackGroundRenderer" };

		bwo::ArrayBuffer<glm::vec3> quad{ bwo::BufferHint::STATIC_DRAW };

		bwo::ArrayBuffer<SingleUIRenderInfo> infos{ bwo::BufferHint::STREAM_READ };

		bwo::VertexArrayObject<
			bwo::Group<glm::vec3, 0,
			bwo::VInfo<float, 3>>,

			bwo::Group<SingleUIRenderInfo, 1,
			bwo::VInfo<float, 2>,
			bwo::VInfo<float, 3>,
			bwo::VInfo<float, 4>>
			> VAO{ quad, infos };

	public:
		void render(UIRenderInfo const& renderInfo,
					GLuint target,
					CameraInfo const& camInfo
		);

		UIBackgroundRenderer();
		~UIBackgroundRenderer();
	};
}