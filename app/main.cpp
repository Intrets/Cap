#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>

#include "MainLoop.h"
#include "Setup.h"

// TODO: keep runtime option, get value from config/command line argument
bool OPENGL_DEBUG = true;

GLFWwindow* window;

int main(int argc, char* argv[]) {
	std::filesystem::path resourcesPath;
	if (argc > 1) {
		resourcesPath = std::filesystem::path(argv[1]);
	}
	else {
		std::cout << "No custom path specified, assuming current dir has the resources folder.\n";
		std::cout << "Can take a single nameless argument to set resources folder.";
		resourcesPath = std::filesystem::current_path().append("resources");
	}

	std::cout << "Resources path: " << std::filesystem::absolute(resourcesPath) << "\n";

	if (!std::filesystem::exists(resourcesPath)) {
		std::cerr << "Fatal error: resources path does not specify an existing folder.\n";
		return 0;
	}

	initGLFW(window, OPENGL_DEBUG);
	initManagers();
	mainLoop(window);
}
