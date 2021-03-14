#pragma once

#include "BufferWrappers.h"

namespace render
{
	class BlitRendererArrayTexture
	{
	private:
		bwo::VertexArrayObject VAO;
		bwo::Program program;

		bwo::ArrayBuffer<glm::vec2> quad{ bwo::BufferHint::STATIC_DRAW };
		bwo::ArrayBuffer<int32_t> rotation{ bwo::BufferHint::DYNAMIC_DRAW };
		bwo::ArrayBuffer<int32_t> layer{ bwo::BufferHint::DYNAMIC_DRAW };
		bwo::ArrayBuffer<glm::vec2> position{ bwo::BufferHint::DYNAMIC_DRAW };

		bwo::UniformTexture2DArray texture_t;
		bwo::Uniform1f depth;
		bwo::UniformMatrix4fv VP;

	public:
		void render(std::vector<glm::vec2> const& positions,
					std::vector<int32_t> const& rotations,
					std::vector<int32_t> const& layers,
					bwo::FrameBuffer& target,
					glm::ivec4 viewport,
					bwo::Texture2DArray const& texture,
					std::optional<float> depth_,
					glm::mat4 VP);

		BlitRendererArrayTexture();
		~BlitRendererArrayTexture() = default;
	};
}
