#pragma once

#include <GL/glew.h>

#include <iostream>
#include <unordered_map>

#include "Enums.h"

namespace render
{
	namespace bwo
	{
		class Texture2D;
	}
}

namespace misc
{
	class PathManager
	{
	private:
		std::unordered_map<int32_t, std::string> paths;
		std::unordered_map<int32_t, std::string> files;

	public:
		enum RESOURCE_PATH
		{
			CONFIG,
			GRAPHICS,
			SAVE,
			FONTS,
		};

		GLuint LoadFont(std::string name);
		GLuint LoadTextureP(std::string name);

		void LoadModelP(std::string& path, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& indexbuffer, int32_t& indexsize);
		bool openFile(std::ifstream& file, RESOURCE_FILE t);
		bool openFile(std::ofstream& file, RESOURCE_FILE t);
		bool openSave(std::ifstream& file, std::string name);
		bool openSave(std::ofstream& file, std::string name);

		PathManager();
		~PathManager();
	};
}
