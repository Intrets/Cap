#include "MainLoop.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <game/GameState.h>
#include <game/player/PlayerInfo.h>
#include <game/ui/ConstructDebugUI.h>

#include <ui/State.h>
#include <ui/ControlState.h>
#include <ui/Constructer.h>

#include <render/Renderer.h>
#include <render/infos/RenderInfo.h>

#include <misc/Option.h>
#include <misc/Timer.h>

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
	renderInfo.cameraInfo = { frameSizeX, frameSizeY, playerInfo.pos, glm::vec3(viewport, 200.0f) };

	//Locator<Timer>::ref().newTiming("Prepare Debug");
	//renderInfo.debugRenderInfo = *Locator<DebugRenderInfo>::get();
	//Locator<DebugRenderInfo>::provide(new DebugRenderInfo());
	//Locator<Timer>::ref().endTiming("Prepare Debug");

	gameState.addRenderInfo(renderInfo);

	Locator<misc::Timer>::ref().newTiming("Prepare UI");
	uiState.appendRenderInfo(gameState, renderInfo);
	Locator<misc::Timer>::ref().endTiming("Prepare UI");
}

void mainLoop(GLFWwindow* window) {
	game::GameState gameState;
	ui::State uiState;

	{
		ui::Global::push();
		ui::window("Debug Info",
				   { 0.0f, 0.0f, 0.13f, 1.0f },
				   ui::WINDOW::TYPE::MINIMISE |
				   ui::WINDOW::TYPE::RESIZEVERTICAL |
				   ui::WINDOW::TYPE::RESIZEHORIZONTAL |
				   ui::WINDOW::TYPE::RESIZE |
				   ui::WINDOW::TYPE::MOVE);
		game::constructDebugUI();

		uiState.UIs.push_back(ui::Global::pop());
	}

	render::Renderer renderer;

	PlayerInfo playerInfo{ { 30, 30 }, gameState, controlState, uiState };

	glfwSetCharCallback(window, char_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	for (;;) {
		Locator<misc::Timer>::ref().newTiming("game logic");
		gameState.runTick();
		Locator<misc::Timer>::ref().endTiming("game logic");

		uiState.updateSize(window);

		render::RenderInfo renderInfo;
		Locator<misc::Timer>::ref().newTiming("prep render");
		prepareRender(window, renderInfo, playerInfo);
		Locator<misc::Timer>::ref().endTiming("prep render");

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
