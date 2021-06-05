#pragma once

#include <wglm/glm.hpp>

#include "BufferWrappers.h"
#include "infos/HighlightRenderInfo.h"

#include "../shaders/Highlight.vert.inc"
#include "../shaders/Highlight.frag.inc"

namespace ogs
{
	class Configuration;
}

namespace render
{
	class HighlightRenderer
	{
	private:
		bwo::Program program{ Highlight_vert, Highlight_frag, "HighlightRenderer" };

		bwo::ArrayBuffer<glm::vec2> quad{ bwo::BufferHint::STATIC_DRAW };

		bwo::ArrayBuffer<SingleHighlightRenderInfo> infos{ bwo::BufferHint::STREAM_DRAW };

		bwo::VertexArrayObject <
			bwo::Group<glm::vec2, 0,
			bwo::VInfo<float, 2>>,

			bwo::Group<SingleHighlightRenderInfo, 1,
			bwo::VInfo<float, 2>,
			bwo::VInfo<float, 2>,
			bwo::VInfo<colors::Color, 1>>
			> VAO{ quad, infos };

		bwo::UniformMatrix4fv VP{ "VP", program };

	public:
		void render(
			HighlightRenderInfo const& highlightInfos,
			bwo::FrameBuffer& target,
			glm::ivec4 viewport,
			glm::mat4 VP
		);

		HighlightRenderer();
		~HighlightRenderer() = default;
	};
}

