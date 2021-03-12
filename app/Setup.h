#pragma once

#include <string>

struct GLFWwindow;

bool initGLFW(GLFWwindow*& window, bool OPENGL_DEBUG);

void initManagers(std::string const& resourceRootPath);