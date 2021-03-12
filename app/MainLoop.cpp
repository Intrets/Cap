#include "MainLoop.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <game/GameState.h>
#include <ui/State.h>
#include <ui/ControlState.h>

#include <render/Renderer.h>
#include <render/infos/RenderInfo.h>

ui::ControlState controlState;

static void key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	controlState.key_callback(w, key, scancode, action, mods);
}

static void mouse_callback(GLFWwindow* w, int32_t key, int32_t action, int32_t mods) {
	key_callback(w, key + GLFW_KEY_LAST, 0, action, mods);
}

void char_callback(GLFWwindow* w, unsigned int character) {
	controlState.char_callback(w, character);
}

void scroll_callback(GLFWwindow* w, double xoffset, double yoffset) {
	controlState.scroll_callback(w, xoffset, yoffset);
}

void mainLoop(GLFWwindow* window) {
	game::GameState gameState;
	ui::State uiState;

	render::Renderer renderer;

	glfwSetCharCallback(window, char_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	for (;;) {
		controlState.cycleStates();
		glfwPollEvents();

		if (glfwWindowShouldClose(window)) {
			break;
		}

		render::RenderInfo renderInfo;

		renderer.render(window, renderInfo);
	}
}
