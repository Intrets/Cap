#pragma once

#include <GL/glew.h>

namespace render
{
	GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
}