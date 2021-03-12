#include "Setup.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// TODO: keep runtime option, get value from config/command line argument
bool OPENGL_DEBUG = true;

GLFWwindow* window;

int main() {
	initGLFW(window, OPENGL_DEBUG);
}
