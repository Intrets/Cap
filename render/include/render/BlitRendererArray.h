#pragma once

#include <optional>

#include "BufferWrappers.h"

#include "infos/BlitArrayRenderInfo.h"

#include "../shaders/BlitArray.vert.inc"
#include "../shaders/BlitArray.frag.inc"

namespace render
{
	class BlitRendererArrayTexture
	{
	private:
		bwo::Program program{ BlitArray_vert, BlitArray_frag, "BlitRendererArrayTexture" };

		bwo::ArrayBuffer<glm::vec2> quad{ bwo::BufferHint::STATIC_DRAW };
		bwo::ArrayBuffer<SingleBlitArrayRenderInfo> infos{ bwo::BufferHint::STREAM_DRAW };

		bwo::VertexArrayObject<
			bwo::Group<glm::vec2, 0,
			bwo::VInfo<float, 2>>,

			bwo::Group<SingleBlitArrayRenderInfo, 1,
			bwo::VInfo<float, 4>,
			bwo::VInfo<int32_t, 1>,
			bwo::VInfo<int32_t, 1>>
			> VAO{ quad, infos };

		bwo::UniformTexture2DArray texture_t{ "texture_t", program, 0 };
		bwo::Uniform1f depth{ "depth", program };
		bwo::UniformMatrix4fv VP{ "VP", program };

	public:
		void render(BlitArrayRenderInfo const& blitInfos,
					bwo::FrameBuffer& target,
					glm::ivec4 viewport,
					bwo::Texture2DArray const& texture,
					std::optional<float> depth_,
					glm::mat4 VP);

		BlitRendererArrayTexture();
		~BlitRendererArrayTexture() = default;
	};
}
