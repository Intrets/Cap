#pragma once

#include "../BufferWrappers.h"

namespace render
{
	class BlockIDTextures
	{
	private:
		bwo::Texture2DArray textureArray;

		int32_t arrayLayers = 0;

		std::unordered_map<std::string, int32_t> textures;

	public:
		int32_t getBlockTextureID(std::string);
		void loadBlockTexture(std::string);
		bwo::Texture2DArray const& getTextureArray();

		NO_COPY_MOVE(BlockIDTextures);

		BlockIDTextures();
		~BlockIDTextures() = default;
	};
}