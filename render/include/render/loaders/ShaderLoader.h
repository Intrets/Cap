#pragma once

#include <GL/glew.h>

namespace render
{
	GLuint LoadShaders(const char* vertex_raw, const char* fragment_raw);
}