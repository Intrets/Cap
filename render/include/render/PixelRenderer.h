#pragma once

#include <wglm/glm.hpp>
#include <optional>

#include "BufferWrappers.h"
#include "infos/BlitRenderInfo.h"
#include "GLStateWrapper.h"

#include "../shaders/Pixel.vert.inc"
#include "../shaders/Pixel.frag.inc"

namespace render
{
	class PixelRenderer
	{
	private:
		bwo::Program program{ Pixel_vert, Pixel_frag, "PixelRenderer" };

		bwo::ArrayBuffer<glm::vec2> quad{ bwo::BufferHint::STATIC_DRAW };
		bwo::ArrayBuffer<SinglePixelRenderInfo> infos{ bwo::BufferHint::STREAM_DRAW };

		bwo::VertexArrayObject<
			bwo::Group<glm::vec2, 0,
			bwo::VInfo<float, 2>>,

			bwo::Group<SinglePixelRenderInfo, 1,
			bwo::VInfo<float, 4>,
			bwo::VInfo<float, 4>,
			bwo::VInfo<float, 2>,
			bwo::VInfo<float, 2>,
			bwo::VInfo<int32_t, 1>>
			> VAO{ quad, infos };

		bwo::UniformTexture2D texture_t{ "texture_t", program, 0 };
		bwo::Uniform2fv UVflip{ "UVflip", program };
		bwo::Uniform1f depth{ "depth", program };
		bwo::Uniform2fv offset{ "offset", program };
		bwo::Uniform4fv color{ "c", program };

	public:
		int32_t const MAX_BATCH_SIZE = 5000;

		void render(ogs::Configuration const& config,
					PixelRenderInfo const& blitInfos,
					bwo::FrameBuffer& target,
					glm::ivec4 viewport,
					bwo::Texture2D const& texture,
					std::optional<float> depth_,
					bool flipUVvertical,
					glm::vec2 offset,
					std::optional<glm::vec4> color = std::nullopt);

		void render(ogs::Configuration const&,
					SinglePixelRenderInfo const& blitInfo,
					bwo::FrameBuffer& target,
					glm::ivec4 viewport,
					bwo::Texture2D const& texture,
					std::optional<float> depth_,
					bool fromTexture,
					std::optional<glm::vec4> color = std::nullopt);

		PixelRenderer();
		~PixelRenderer();
	};
}
