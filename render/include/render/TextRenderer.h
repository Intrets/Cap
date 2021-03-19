#pragma once

#include <gl/glew.h>

namespace render
{
	namespace bwo
	{
		class FrameBuffer;
	}

	struct TextRenderInfo;
	class Fonts;
	struct CameraInfo;

	class TextRenderer
	{
	public:
		void render(TextRenderInfo const& textRenderInfo,
					Fonts const& fonts,
					bwo::FrameBuffer& target);

		TextRenderer();
		~TextRenderer();
	};
}