#pragma once

#include <GL/glew.h>

#include <string>
#include <filesystem>

namespace render
{
	namespace bwo
	{
		class Texture2D;
	}

	/// Filename can be KTX or DDS files
	GLuint loadTexture(const std::string Filename);
	bwo::Texture2D load2DTexture(std::string const Filename);
	bwo::Texture2D load2DTexture(std::filesystem::path path);
}
