#pragma once

#include "BufferWrappers.h"

#include "../shaders/Debug.vert.inc"
#include "../shaders/Debug.frag.inc"

namespace render
{
	struct DebugRenderInfo;
	struct CameraInfo;

	class DebugRenderer
	{
	private:
		bwo::Program program{ Debug_vert, Debug_frag, "DebugRender" };

		bwo::ArrayBuffer<glm::vec2> points{ bwo::BufferHint::STREAM_DRAW };

		bwo::VertexArrayObject<
			bwo::Group<glm::vec2, 0,
			bwo::VInfo<float, 2>>
			> VAO{ points };

		bwo::UniformMatrix4fv VP{ "VP", program };

	public:
		void render(
			DebugRenderInfo const& info,
			bwo::FrameBuffer& target,
			glm::ivec4 viewport,
			CameraInfo const& cameraInfo
		);
	};
}