#pragma once

#include <iostream>
#include <unordered_map>
#include <filesystem>

namespace render
{
	namespace bwo
	{
		class Texture2D;
	}
}

typedef unsigned int GLuint;

namespace misc
{
	enum class RESOURCE_PATH
	{
		CONFIG,
		GRAPHICS,
		SAVE,
		FONTS,
		SOUNDS,
	};

	enum class RESOURCE_FILE
	{
		OPTIONS,
	};

	class PathManager
	{
	public:
	private:
		std::unordered_map<RESOURCE_PATH, std::filesystem::path> paths;
		std::unordered_map<RESOURCE_FILE, std::filesystem::path> files;
	public:

		render::bwo::Texture2D LoadFont(std::string name);
		GLuint LoadTextureP(std::string name);
		render::bwo::Texture2D LoadTexture2DP(std::string name);

		void LoadModelP(std::string& path, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& indexbuffer, int32_t& indexsize);
		bool openFile(std::ifstream& file, RESOURCE_FILE t);
		bool openFile(std::ofstream& file, RESOURCE_FILE t);
		bool openSave(std::ifstream& file, std::string name);
		bool openSave(std::ofstream& file, std::string name);
		std::filesystem::path getSoundsPath();

		PathManager(std::string const& root);
		~PathManager();
	};
}
