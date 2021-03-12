#include "PathManager.h"

#include <fstream>

#include <render/loaders/TextureLoader.h>
#include <render/loaders/ModelLoader.h>
#include <render/loaders/ShaderLoader.h>
#include <render/BufferWrappers.h>

namespace misc
{
	GLuint PathManager::LoadFont(std::string name) {
		return render::loadTexture(this->paths[RESOURCE_PATH::FONTS] + name);
	}

	GLuint PathManager::LoadTextureP(std::string name) {
		return render::loadTexture(this->paths[RESOURCE_PATH::GRAPHICS] + name);
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

	PathManager::PathManager() {
		const std::string r = "C:/Users/Intrets/source/Game/Game/resources/";
		this->paths[RESOURCE_PATH::CONFIG] = r + "config/";
		this->paths[RESOURCE_PATH::GRAPHICS] = r + "graphics/";
		this->paths[RESOURCE_PATH::SAVE] = r + "saves/";
		this->files[RESOURCE_FILE::OPTIONS] = r + "config/options.txt";
		this->paths[RESOURCE_PATH::FONTS] = r + "graphics/fonts/";
	}

	PathManager::~PathManager() {
	}
}
