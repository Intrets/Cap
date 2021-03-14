#include "PathManager.h"

#include <fstream>
#include <string>

#include <render/loaders/TextureLoader.h>
#include <render/loaders/ModelLoader.h>
#include <render/loaders/ShaderLoader.h>
#include <render/BufferWrappers.h>

namespace misc
{
	render::bwo::Texture2D PathManager::LoadFont(std::string name) {
		return render::load2DTexture(this->paths[RESOURCE_PATH::FONTS] + name);
	}

	GLuint PathManager::LoadTextureP(std::string name) {
		return render::loadTexture(this->paths[RESOURCE_PATH::GRAPHICS] + name);
	}

	render::bwo::Texture2D PathManager::LoadTexture2DP(std::string name) {
		return render::load2DTexture(this->paths[RESOURCE_PATH::GRAPHICS] + name);
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

	PathManager::PathManager(std::string const& root) {
		this->paths[RESOURCE_PATH::CONFIG] = root + "/config/";
		this->paths[RESOURCE_PATH::GRAPHICS] = root + "/graphics/";
		this->paths[RESOURCE_PATH::SAVE] = root + "/saves/";
		this->files[RESOURCE_FILE::OPTIONS] = root + "/config/options.txt";
		this->paths[RESOURCE_PATH::FONTS] = root + "/graphics/fonts/";
	}

	PathManager::~PathManager() {
	}
}
