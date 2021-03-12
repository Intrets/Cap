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
		bwo::VertexArrayObject VAO;
		bwo::Program program;

		bwo::ArrayBuffer<glm::vec3> quad{ bwo::BufferHint::STATIC_DRAW };
		bwo::ArrayBuffer<glm::vec3> positions{ bwo::BufferHint::STREAM_DRAW };
		bwo::ArrayBuffer<glm::vec2> scales{ bwo::BufferHint::STREAM_DRAW };
		bwo::ArrayBuffer<glm::vec4> colors{ bwo::BufferHint::STREAM_DRAW };

	public:
		void render(UIRenderInfo const& renderInfo, GLuint target, CameraInfo const& camInfo);

		UIBackgroundRenderer();
		~UIBackgroundRenderer();
	};
}