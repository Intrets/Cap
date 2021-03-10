#include "PathManager.h"

#include <fstream>

#include "loaders/TextureLoader.h"
#include "loaders/ModelLoader.h"
#include "loaders/ShaderLoader.h"

#include "BufferWrappers.h"

namespace misc
{
	GLuint PathManager::LoadFont(std::string name) {
		return render::loadTexture(this->paths[RESOURCE_PATH::FONTS] + name);
	}

	GLuint PathManager::LoadTextureP(std::string name) {
		return render::loadTexture(this->paths[RESOURCE_PATH::GRAPHICS] + name);
	}

	render::bwo::Texture2D PathManager::LoadTexture2DP(std::string name) {
		return render::load2DTexture(this->paths[RESOURCE_PATH::GRAPHICS] + name);
	}

	GLuint PathManager::LoadShadersP(std::string vertex, std::string fragment) {
		std::string fullPathVertex = this->paths[RESOURCE_PATH::SHADERS] + vertex;
		std::string fullPathFragment = this->paths[RESOURCE_PATH::SHADERS] + fragment;
		return render::LoadShaders(fullPathVertex.c_str(), fullPathFragment.c_str());
	}

	GLuint PathManager::LoadShadersP(std::string name) {
		return this->LoadShadersP(name + ".vert", name + ".frag");
	}

	void PathManager::LoadModelP(std::string& name, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& indexbuffer, int32_t& indexsize) {
		std::string path = this->paths[RESOURCE_PATH::GRAPHICS] + name;
		render::loadModel(path, vertexbuffer, uvbuffer, normalbuffer, indexbuffer, indexsize);
	}

	bool PathManager::openFile(std::ifstream& file, RESOURCE_FILE t) {
		file.open(this->files[t]);
		return file.is_open();
	}

	bool PathManager::openFile(std::ofstream& file, RESOURCE_FILE t) {
		file.open(this->files[t]);
		return file.is_open();
	}

	bool PathManager::openSave(std::ifstream& file, std::string name) {
		file.open(this->paths[RESOURCE_PATH::SAVE] + name, std::ifstream::binary);
		return file.is_open();
	}

	bool PathManager::openSave(std::ofstream& file, std::string name) {
		file.open(this->paths[RESOURCE_PATH::SAVE] + name, std::ofstream::binary);
		return file.is_open();
	}

	bool PathManager::openLUA(std::ifstream& file, std::string name) {
		file.open(this->paths[RESOURCE_PATH::LUA] + name);
		return file.is_open();
	}

	bool PathManager::openLUA(std::ofstream& file, std::string name) {
		file.open(this->paths[RESOURCE_PATH::LUA] + name);
		return file.is_open();
	}

	bool PathManager::openBlockData(std::ifstream& file) {
		file.open(this->paths[RESOURCE_PATH::BLOCKDATA]);
		return file.is_open();
	}

	PathManager::PathManager() {
		std::string r = "C:/Users/Intrets/source/Repos/Bam/Bam/";
		this->paths[RESOURCE_PATH::CONFIG] = r + "config/";
		this->paths[RESOURCE_PATH::GRAPHICS] = r + "Resources/";
		this->paths[RESOURCE_PATH::SHADERS] = r;
		this->paths[RESOURCE_PATH::SAVE] = r + "saves/";
		this->files[RESOURCE_FILE::OPTIONS] = r + "config/options.txt";
		this->paths[RESOURCE_PATH::FONTS] = r + "Resources/";
		this->paths[RESOURCE_PATH::LUA] = r + "saves/LUA/";
		this->paths[RESOURCE_PATH::BLOCKDATA] = r + "Resources/blocks.txt";
	}

	PathManager::~PathManager() {
	}
}
