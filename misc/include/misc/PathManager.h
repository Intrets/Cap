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
			SHADERS,
			SAVE,
			FONTS,
			BLOCKDATA,
			LUA,
		};

		GLuint LoadFont(std::string name);
		GLuint LoadTextureP(std::string name);
		render::bwo::Texture2D LoadTexture2DP(std::string name);
		GLuint LoadShadersP(std::string vertex, std::string fragment);
		GLuint LoadShadersP(std::string name);

		void LoadModelP(std::string& path, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& indexbuffer, int32_t& indexsize);
		bool openFile(std::ifstream& file, RESOURCE_FILE t);
		bool openFile(std::ofstream& file, RESOURCE_FILE t);
		bool openSave(std::ifstream& file, std::string name);
		bool openSave(std::ofstream& file, std::string name);

		bool openLUA(std::ifstream& file, std::string name);
		bool openLUA(std::ofstream& file, std::string name);

		bool openBlockData(std::ifstream& file);

		PathManager();
		~PathManager();
	};
}
