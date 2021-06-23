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

template<class F>
struct RWrapper
{
	F f;

	inline constexpr auto operator()(auto x) const {
		return f(x);
	}
};

template<class F>
RWrapper(F const&)->RWrapper<F>;

template<class F, class G>
constexpr inline auto operator|(RWrapper<F> f, RWrapper<G> g) {
	auto fun = [=](auto x) { return f(g(x)); };
	return RWrapper<decltype(fun)>{fun};
};

template<class F, class G>
constexpr inline auto operator&&(RWrapper<F> f, RWrapper<G> g) {
	auto fun = [=](auto x) { return f(x) && g(x); };
	return RWrapper<decltype(fun)>{fun};
}

inline bool weirdFun(int x, int min, int max) {
	constexpr auto rNot = RWrapper{ [](bool x) {return !x; } };
	auto rBiggerThan = RWrapper{ [=](int x) {return x > min; } };
	auto rSmallerThan = RWrapper{ [=](int x) {return x < max; } };

	auto fun = rNot | (rBiggerThan && rSmallerThan);

	return fun(x);
}

inline bool normalFun(int x, int min, int max) {
	return !((x > min) && (x < max));
}

inline double runTestNormal(std::vector<std::tuple<int, int, int>>& data) {
	auto start = glfwGetTime();

	uint64_t count = 0;
	for (size_t i = 0; i < 10000; i++) {
		for (auto [x, min, max] : data) {
			count += normalFun(x, min, max);
		}
	}

	auto duration = glfwGetTime() - start;
	std::cout << "normal: " << duration << " count: " << count << "\n";
	return duration;
}

inline double runTestWeird(std::vector<std::tuple<int, int, int>>& data) {
	auto start = glfwGetTime();

	uint64_t count = 0;
	for (size_t i = 0; i < 10000; i++) {
		for (auto [x, min, max] : data) {
			count += weirdFun(x, min, max);
		}
	}

	auto duration = glfwGetTime() - start;
	std::cout << "weird: " << duration << " count: " << count << "\n";
	return duration;
}

//namespace tt
//{
//	template<class F, class G>
//	constexpr auto operator|(F f, G g) {
//		return [=](auto x) { return f(g(x)); };
//	}
//
//}

//template<class F, class = std::void_t<typename F::template operator() < int > > >
template<class F>
concept lambda_function = requires (F f) {
	std::function(f);
};

void mainLoop(GLFWwindow* window, std::chrono::steady_clock::time_point startTime) {
	//using namespace tt;
	system("cls");
	[[maybe_unused]]
	constexpr auto funcNot = [](bool x) {
		return !x;
	};

	constexpr int aaaa = 12;
	[[maybe_unused]]
	constexpr auto biggerThan10 = [=](auto x) {
		return x > aaaa;
	};

	//constexpr auto f = funcNot | biggerThan10;
	//constexpr auto b1 = f(10);
	//constexpr auto b2 = f(11);

	//auto constexpr test = lambda_function<decltype(biggerThan10)>;

	//std::function(biggerThan10);


	//auto cursed = decltype(biggerThan10)::operator() < std::string > ;

	//(biggerThan10.*cursed)(1);




	if (rand() > 0.0000000001) {
		exit(0);
	}

	constexpr auto biggerThan = RWrapper{ [](auto x) { return RWrapper{ [=](auto i) { return i == x; } }; } };
	static_assert(!(biggerThan(4) && biggerThan(10))(5));




	game::GameState gameState;
	gameState.init();

	ui::State uiState;

	std::stringstream stream;

	serial::Serializer serializer2;
	serializer2.writeStream = &stream;
	serializer2.print(gameState);

	std::cout << stream.str() << "\n";

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
