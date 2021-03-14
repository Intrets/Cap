#include "textures/BlockIDTextures.h"

#include <glm/glm.hpp>

#include <mem/Locator.h>

#include <misc/PathManager.h>

#include "GLEnabler.h"
#include "BlitRenderer.h"

namespace render
{
	int32_t BlockIDTextures::getBlockTextureID(std::string name) {
		auto search = this->textures.find(name);
		if (search == this->textures.end()) {
			// TODO : actually make texture loading work
			this->loadBlockTexture(name);
			this->textures[name] = this->arrayLayers;
			return this->arrayLayers++;
		}
		else {
			return search->second;
		}
	}

	void BlockIDTextures::loadBlockTexture(std::string name) {
		bwo::Texture2D newTex(Locator<misc::PathManager>::get()->LoadTexture2DP(name));

		// Failed to load texture
		if (newTex.ID == 0) {
			printf("Failed to load texture: %s\n\n", name.c_str());
		}

		GLEnabler glEnabler;
		glEnabler.disable(GL_BLEND);

		bwo::FrameBuffer target;
		target.bindTextureLayer(GL_COLOR_ATTACHMENT0, this->textureArray, 0, this->arrayLayers);

		SingleBlitRenderInfo info{
			{ 0.0f, 0.0f, 1.0f, 1.0f },
			{ -1.0f, -1.0f, 2.0f, 2.0f },
			4 };

		Locator<BlitRenderer>::ref().render(
			info,
			target,
			{ 0, 0, this->textureArray.size.x, this->textureArray.size.y },
			newTex,
			std::nullopt,
			true
		);
	}

	GLuint BlockIDTextures::getTextureArrayID() {
		return this->textureArray.ID;
	}

	bwo::Texture2DArray const& BlockIDTextures::getTextureArray() {
		return this->textureArray;
	}

	BlockIDTextures::BlockIDTextures() :
		textureArray(bwo::Texture2DArrayHelper::makeLinearFiltering({ 128, 128, 1024 })) {
		const std::string textures_raw[] = { "weird_ground.dds", "weird_moss.dds" };

		for (auto& s : textures_raw) {
			this->loadBlockTexture(s);
			this->textures[s] = this->arrayLayers++;
		}
	}
}