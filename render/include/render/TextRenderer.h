#pragma once

#include <gl/glew.h>

namespace render
{
	struct TextRenderInfo;
	class Fonts;
	struct CameraInfo;

	class TextRenderer
	{
	public:
		void render(TextRenderInfo const& textRenderInfo, Fonts const& fonts, GLuint target, CameraInfo const& camInfo);

		TextRenderer();
		~TextRenderer();
	};
}