#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>

#include <vector>
#include <string>

namespace render
{
	bool loadOBJ(
		std::string& path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals
	);

	bool loadModel(std::string& path,
				   GLuint& vertexbuffer,
				   GLuint& uvbuffer,
				   GLuint& normalbuffer,
				   GLuint& indexbuffer,
				   int32_t& indexsize
	);
}