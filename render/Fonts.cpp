#include "Fonts.h"

#include "GLStateWrapper.h"

#include <mem/Global.h>

#include <misc/PathManager.h>

#include <render/BlitRenderer.h>
#include <render/infos/BlitRenderInfo.h>

namespace render
{
	FontInfo Fonts::loadMonospacedFont(std::string name, glm::ivec2 charDim, glm::ivec2 gridDim) {
		auto tex = Global<misc::PathManager>->LoadFont(name);

		render::BlitRenderInfo blitInfo;

		this->laneWidth = glm::max(this->laneWidth, charDim.y / static_cast<float>(this->fontAtlas.size.y));

		glm::ivec2 texSize;
		glBindTexture(GL_TEXTURE_2D, tex.ID);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texSize.x);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texSize.y);

		int count = 0;
		for (int32_t y = 0; y < gridDim.y; y++) {
			for (int32_t x = 0; x < gridDim.x; x++) {
				glm::vec2 uvTop = glm::vec2(charDim.x * x, charDim.y * (y + 1)) / glm::vec2(texSize);
				uvTop.y = 1.0f - uvTop.y;
				glm::vec2 uvSize = glm::vec2(charDim) / glm::vec2(texSize);

				blitInfo.addBlitInfo(
					glm::vec4(uvTop, uvSize),
					glm::vec4(),
					0
				);
				count++;
				if (count == 128) {
					break;
				}
			}
		}

		FontInfo fontInfoResult;
		fontInfoResult.name = name;

		glm::vec2 worldSize = 2.0f * glm::vec2(charDim) / glm::vec2(this->fontAtlas.size);

		for (int32_t i = 0; i < 128; i++) {
			glm::vec2 worldTop = this->pos;
			worldTop.y -= worldSize.y;

			blitInfo.getDataMut()[i].world = glm::vec4(worldTop, worldSize);

			fontInfoResult.charSize[i] = charDim;
			glm::vec2 uv = worldTop;
			fontInfoResult.charUV[i] = glm::vec4((uv + 1.0f) / 2.0f, worldSize / 2.0f);

			this->pos.x += worldSize.x;

			if (this->pos.x + worldSize.x > 1.0f) {
				this->pos.x = -1.0f;
				this->pos.y -= worldSize.y;
			}
			if (this->pos.y < -1.0f) {
				assert(0);
			}
		}

		auto config = ogs::FontAtlasConfiguration();

		Global<render::BlitRenderer>->render(
			config,
			blitInfo,
			this->buffer,
			glm::ivec4(0, 0, this->fontAtlas.size.x, this->fontAtlas.size.y),
			tex,
			std::nullopt,
			true,
			glm::vec2(0.0f)
		);

		return fontInfoResult;
	}

	FontInfo& Fonts::getFont(FONT font) {
		return this->fontInfos[static_cast<int32_t>(font)];
	}

	Fonts::Fonts() :
		fontAtlas(render::bwo::Texture2DHelper::makeNoFiltering(glm::ivec2(256))) {

		this->buffer.bindTextureColor(0, this->fontAtlas, 0);

		this->pos = glm::vec2(-1.0f, 1.0f);

		this->fontInfos[static_cast<size_t>(FONT::ROBOTO_12)] = loadMonospacedFont("roboto_mono_12px_7x17_36x4.dds", { 7,17 }, { 36,4 });
		this->fontInfos[static_cast<size_t>(FONT::ROBOTO_14)] = loadMonospacedFont("roboto_mono_14px_8x19_32x4.dds", { 8,19 }, { 32,4 });
		this->fontInfos[static_cast<size_t>(FONT::ROBOTO_16)] = loadMonospacedFont("roboto_mono_16px_10x22_25x6.dds", { 10,22 }, { 25,6 });
	}

	Fonts::~Fonts() {
	}
}