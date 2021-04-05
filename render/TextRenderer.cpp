#include "TextRenderer.h"

#include <wglm/glm.hpp>
#include <mem/Locator.h>

#include "infos/TextRenderInfo.h"
#include "BufferWrappers.h"
#include "BlitRenderer.h"
#include "Colors.h"

namespace render
{
	void TextRenderer::render(TextRenderInfo const& textRenderInfo,
							  Fonts const& fonts,
							  bwo::FrameBuffer& target) {
		for (auto& info : textRenderInfo.windowTextRenderInfos) {
			glm::ivec2 size = info.screenRectangle.getSize();

			glm::ivec2 bottomLeft = info.screenRectangle.getBottomLeft();

			ogs::Configuration config = ogs::TextConfiguration();

			Locator<BlitRenderer>::ref().render(
				config,
				info.textRenderInfo,
				target,
				{ bottomLeft.x, bottomLeft.y, size.x, size.y },
				fonts.fontAtlas,
				info.depth,
				false,
				info.offset,
				COLORS::UI::TEXT
			);
		}
	}

	TextRenderer::TextRenderer() {
	}

	TextRenderer::~TextRenderer() {
	}
}