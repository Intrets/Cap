#pragma once

#include "BufferWrappers.h"

#include "../shaders/Debug.vert.inc"
#include "../shaders/Debug.frag.inc"

#include "infos/DebugRenderInfo.h"

namespace render
{
	struct DebugRenderInfo;
	struct CameraInfo;

	class DebugRenderer
	{
	private:
		bwo::Program program{ Debug_vert, Debug_frag, "DebugRender" };

		using VertexDataType = DebugRenderInfo::Data::Point;

		bwo::ArrayBuffer<VertexDataType> points{ bwo::BufferHint::STREAM_DRAW };

		bwo::VertexArrayObject<
			bwo::Group<VertexDataType, 0,
			bwo::VInfo<float, 2>,
			bwo::VInfo<colors::Color, 1>>
			> VAO{ points };

		bwo::UniformMatrix4fv VP{ "VP", program };
		bwo::Uniform1f pointSize{ "pointSize", program };

	public:
		void render(
			DebugRenderInfo const& info,
			bwo::FrameBuffer& target,
			glm::ivec4 viewport,
			CameraInfo const& cameraInfo
		);
	};
}