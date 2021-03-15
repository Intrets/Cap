#pragma once

#include <glm/glm.hpp>
#include <optional>

#include "BufferWrappers.h"
#include "infos/BlitRenderInfo.h"

namespace render
{
	class BlitRenderer
	{
	private:
		bwo::VertexArrayObject VAO;

		bwo::Program program;

		bwo::ArrayBuffer<glm::vec2> quad{ bwo::BufferHint::STATIC_DRAW };

		bwo::ArrayBuffer<SingleBlitRenderInfo> infos{ bwo::BufferHint::STATIC_DRAW };

		bwo::UniformTexture2D texture_t;
		bwo::Uniform1f UVflip;
		bwo::Uniform1f depth;
		bwo::Uniform2fv offset;
		bwo::Uniform4fv color;

	public:
		int32_t const MAX_BATCH_SIZE = 5000;

		void render(BlitRenderInfo const& blitInfos,
					bwo::FrameBuffer& target,
					glm::ivec4 viewport,
					bwo::Texture2D const& texture,
					std::optional<float> depth_,
					bool flipUVvertical,
					glm::vec2 offset,
					std::optional<glm::vec4> color = std::nullopt);

		void render(SingleBlitRenderInfo const& blitInfo,
					bwo::FrameBuffer& target,
					glm::ivec4 viewport,
					bwo::Texture2D const& texture,
					std::optional<float> depth_,
					bool fromTexture,
					std::optional<glm::vec4> color = std::nullopt);

		BlitRenderer();
		~BlitRenderer();
	};
}
