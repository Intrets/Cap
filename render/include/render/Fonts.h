#pragma once

#include <glm/glm.hpp>

#include <string>
#include <array>

#include "Enums.h"
#include "BufferWrappers.h"

namespace render
{
	struct FontInfo
	{
		std::string name;
		std::array<glm::vec4, 128> charUV;
		std::array<glm::ivec2, 128> charSize;
	};

	class Fonts
	{
	private:
		glm::vec2 pos;
		float laneWidth;

		FontInfo loadMonospacedFont(std::string name, glm::ivec2 charDim, glm::ivec2 gridDim);

	public:
		bwo::FrameBuffer buffer;
		bwo::Texture2D fontAtlas;

		std::array<FontInfo, static_cast<int32_t>(FONT::FONT_MAX)> fontInfos;

		FontInfo& getFont(FONT font);

		Fonts();
		~Fonts();
	};
}
