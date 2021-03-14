#pragma once

#include <optional>

#include "BufferWrappers.h"

#include "infos/BlitArrayRenderInfo.h"

namespace render
{
	class BlitRendererArrayTexture
	{
	private:
		bwo::VertexArrayObject VAO;
		bwo::Program program;

		bwo::ArrayBuffer<glm::vec2> quad{ bwo::BufferHint::STATIC_DRAW };

		bwo::ArrayBuffer<SingleBlitArrayRenderInfo> infos{ bwo::BufferHint::DYNAMIC_DRAW };

		bwo::UniformTexture2DArray texture_t;
		bwo::Uniform1f depth;
		bwo::UniformMatrix4fv VP;

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
