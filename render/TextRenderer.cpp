#include "TextRenderer.h"

#include <glm/glm.hpp>
#include <mem/Locator.h>

#include "infos/TextRenderInfo.h"
#include "BufferWrappers.h"
#include "BlitRenderer.h"
#include "Colors.h"

namespace render
{
	void TextRenderer::render(TextRenderInfo const& textRenderInfo,
							  Fonts const& fonts,
							  bwo::FrameBuffer& target,
							  CameraInfo const& cameraInfo) {
		for (auto& info : textRenderInfo.windowTextRenderInfos) {
			glm::ivec2 size = glm::floor(glm::vec2(info.screenRectangle.getPixelSize()) * info.screenRectangle.getAbsSize() / 2.0f);

			glm::vec2 topLeft = glm::floor((info.screenRectangle.getTopLeft() / 2.0f + 0.5f) * glm::vec2(info.screenRectangle.getPixelSize()));

			Locator<BlitRenderer>::ref().render(
				info.textRenderInfo,
				target,
				{ topLeft.x, topLeft.y - size.y, size.x, size.y },
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