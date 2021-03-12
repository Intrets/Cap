#include "MainLoop.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <game/GameState.h>
#include <game/player/PlayerInfo.h>

#include <ui/State.h>
#include <ui/ControlState.h>

#include <render/Renderer.h>
#include <render/infos/RenderInfo.h>

#include <misc/Option.h>

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


void prepareRender(GLFWwindow* window, render::RenderInfo& renderInfo, PlayerInfo& playerInfo) {
	auto& gameState = playerInfo.gameState;
	auto& uiState = playerInfo.uiState;

	int32_t frameSizeX, frameSizeY;
	glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
	float ratio = frameSizeX / static_cast<float>(frameSizeY);
	glm::vec2 viewport(ratio, 1.0f);
	viewport *= misc::Option<misc::OPTION::CL_VIEWPORTSCALE, float>::getVal();
	renderInfo.frameSize = { frameSizeX, frameSizeY };
	renderInfo.cameraInfo = { frameSizeX, frameSizeY, {0, 0}, glm::vec3(viewport, 200.0f) };

	//Locator<Timer>::ref().newTiming("Prepare Debug");
	//renderInfo.debugRenderInfo = *Locator<DebugRenderInfo>::get();
	//Locator<DebugRenderInfo>::provide(new DebugRenderInfo());
	//Locator<Timer>::ref().endTiming("Prepare Debug");

	//gameState.appendStaticRenderInfo(renderInfo);

	//Locator<Timer>::ref().newTiming("Prepare UI");
	uiState.appendRenderInfo(gameState, renderInfo);
	//Locator<Timer>::ref().endTiming("Prepare UI");
}

void mainLoop(GLFWwindow* window) {
	game::GameState gameState;
	ui::State uiState;

	render::Renderer renderer;

	PlayerInfo playerInfo{ gameState, controlState, uiState };

	glfwSetCharCallback(window, char_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	for (;;) {
		uiState.updateSize(window);

		render::RenderInfo renderInfo;
		prepareRender(window, renderInfo, playerInfo);

		controlState.cycleStates();
		glfwPollEvents();

		uiState.updateCursor(window, { 0,0 });

		uiState.run(playerInfo);

		if (glfwWindowShouldClose(window)) {
			break;
		}


		renderer.render(window, renderInfo);
	}
}
