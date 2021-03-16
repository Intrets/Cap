#pragma once

#include <GL/glew.h>

#include "BufferWrappers.h"

namespace render
{
	struct UIRenderInfo;
	struct CameraInfo;

	class UIBackgroundRenderer
	{
	private:
		bwo::Program program;

		bwo::ArrayBuffer<glm::vec3> quad{ bwo::BufferHint::STATIC_DRAW };
		bwo::ArrayBuffer<glm::vec3> positions{ bwo::BufferHint::STREAM_DRAW };
		bwo::ArrayBuffer<glm::vec2> scales{ bwo::BufferHint::STREAM_DRAW };
		bwo::ArrayBuffer<glm::vec4> colors{ bwo::BufferHint::STREAM_DRAW };

		bwo::VertexArrayObject<
			bwo::Group<glm::vec3, 0,
			bwo::VInfo<float, 3>>,

			bwo::Group<glm::vec3, 1,
			bwo::VInfo<float, 3>>,

			bwo::Group<glm::vec2, 1,
			bwo::VInfo<float, 2>>,

			bwo::Group<glm::vec4, 1,
			bwo::VInfo<float, 4>>
			> VAO{ quad, positions, scales, colors };

	public:
		void render(UIRenderInfo const& renderInfo, GLuint target, CameraInfo const& camInfo);

		UIBackgroundRenderer();
		~UIBackgroundRenderer();
	};
}