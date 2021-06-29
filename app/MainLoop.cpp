#include "MainLoop.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <game/GameState.h>
#include <game/player/PlayerInfo.h>
#include <game/ui/ConstructDebugUI.h>
#include <game/Merger.h>
#include <game/ui/game_control/EntityInterface.h>

#include <ui/State.h>
#include <ui/ControlState.h>
#include <ui/Constructer.h>

#include <render/Renderer.h>
#include <render/infos/RenderInfo.h>

#include <misc/Option.h>
#include <misc/Timer.h>
#include <misc/Log.h>
#include <misc/PathManager.h>

#include <fstream>
#include <format>
#include <sstream>


#include <tepp/tepp.h>
#include <cstdlib>
#include <cctype>

#include <ranges>
#include <SDL_mixer.h>

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

void prepareRender(
	GLFWwindow* window,
	render::RenderInfo& renderInfo,
	PlayerInfo& playerInfo) {

	auto& gameState = playerInfo.gameState;
	auto& uiState = playerInfo.uiState;

	int32_t frameSizeX, frameSizeY;
	glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
	float ratio = frameSizeX / static_cast<float>(frameSizeY);
	glm::vec2 viewport(ratio, 1.0f);
	viewport *= misc::Option<misc::OPTION::CL_VIEWPORTSCALE, float>::getVal();
	renderInfo.frameSize = { frameSizeX, frameSizeY };
	renderInfo.cameraInfo = { frameSizeX, frameSizeY, playerInfo.pos, glm::vec3(viewport, 200.0f) };

	//Global<Timer>->newTiming("Prepare Debug");
	//renderInfo.debugRenderInfo = *Global<DebugRenderInfo>();
	//Global<DebugRenderInfo>::provide(new DebugRenderInfo());
	//Global<Timer>->endTiming("Prepare Debug");

	gameState.addRenderInfo(renderInfo);

	Global<misc::Timer>->newTiming("Prepare UI");
	uiState.appendRenderInfo(gameState, renderInfo);
	Global<misc::Timer>->endTiming("Prepare UI");
}

void mainLoop(GLFWwindow* window, std::chrono::steady_clock::time_point startTime) {
	game::GameState gameState;
	gameState.init();

	ui::State uiState;

	{
		uiState.addUI(ui::Global::getManager().makeUniqueRef<game::EntityInterface>());
	}

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

	PlayerInfo playerInfo{ { 35, 35 }, {}, gameState, controlState, uiState };

	glfwSetCharCallback(window, char_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	std::chrono::duration<double> startUpDuration = std::chrono::steady_clock::now() - startTime;
	Global<misc::Log>->putLine(std::format("Startup time: {} seconds\n", startUpDuration));

	for (;;) {
		if (uiState.loadGame.has_value()) {
			game::GameState newGameState;

			std::ifstream file;
			Global<misc::PathManager>->openSave(file, uiState.loadGame.value());
			if (!file.good()) {
				Global<misc::Log>->putLine(std::format("failed to open save file: {}", uiState.loadGame.value()));
			}
			else {
				serial::Serializer serializer{ file };

				auto start = glfwGetTime();

				Global<game::NewEverything>.init(&gameState.everything);
				if (!serializer.read(newGameState)) {
					Global<misc::Log>->putLine(std::format("failed to load game from: {}", uiState.loadGame.value()));
				}
				else {
					gameState = std::move(newGameState);
					Global<misc::Log>->putLine(std::format("loaded game from {} in {:.4f} seconds", uiState.loadGame.value(), glfwGetTime() - start));
				}
				Global<game::NewEverything>.destroy();
			}

			uiState.loadGame.reset();
			file.close();
		}

		if (uiState.saveGame.has_value()) {
			std::ofstream file;
			Global<misc::PathManager>->openSave(file, uiState.saveGame.value());
			if (!file.good()) {
				Global<misc::Log>->putLine(std::format("failed to open save file: {}", uiState.saveGame.value()));
			}
			else {
				serial::Serializer serializer{ file };

				auto start = glfwGetTime();
				if (!serializer.write(gameState)) {
					Global<misc::Log>->putLine(std::format("failed to save game to: {}", uiState.saveGame.value()));
				}
				else {
					Global<misc::Log>->putLine(std::format("saved game to {} in {:.4f} seconds", uiState.saveGame.value(), glfwGetTime() - start));
				}
			}

			uiState.saveGame.reset();
			file.close();
		}

		Global<misc::Timer>->newTiming("game logic");
		gameState.runTick();
		Global<misc::Timer>->endTiming("game logic");

		uiState.updateSize(window);

		render::RenderInfo renderInfo;
		Global<misc::Timer>->newTiming("prep render");
		prepareRender(window, renderInfo, playerInfo);
		Global<misc::Timer>->endTiming("prep render");

		controlState.cycleStates();
		glfwPollEvents();

		uiState.updateCursor(window, { playerInfo.pos });

		uiState.run(playerInfo);

		if (glfwWindowShouldClose(window)) {
			break;
		}

		renderer.render(window, renderInfo);
	}
}
