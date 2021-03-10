#pragma once

#include <vector>
#include <unordered_map>
#include <list>

#include <glm/glm.hpp>

#include <game/player/PlayerInfo.h>
#include <mem/ReferenceManager.h>
#include <misc/Misc.h>

#include "Base.h"

namespace render
{
	struct RenderInfo;
}

namespace ui
{
	class State
	{
	private:
		bool shouldReset_ = false;

		glm::vec2 cursorScreen;
		glm::vec2 cursorWorld;

		CallBackBindResult runFrontBinds(PlayerInfo& playerInfo);

		std::unordered_map<std::string, ManagedReference<Base, Base>> namedUIs;

	public:
		std::list<UniqueReference<Base, Base>> UIs;

		std::vector<UniqueReference<Base, Base>> UIsBuffer;
		std::unordered_map<std::string, UniqueReference<Base, Base>> namedUIsBuffer;

		std::vector<UniqueReference<Base, Base>> closedBuffer;

		glm::vec2 getCursorPositionWorld();
		glm::vec2 getCursorPositionScreen();
		glm::vec2 getCursorPositionScreenClamped(float c);

		void runUIBinds(PlayerInfo& playerInfo);
		void run(PlayerInfo& playerInfo);

		bool updateSize(GLFWwindow* window);
		void updateCursor(GLFWwindow* window, glm::vec2 cam);
		void appendRenderInfo(GameState& gameState, render::RenderInfo& renderInfo);

		void addUI(UniqueReference<Base, Base> ref);

		// Returns if new UI is created or previous one is brought to the front
		// true - new object created
		// false - already exists and brought to the front
		bool addNamedUI(std::string const& name, std::function<UniqueReference<Base, Base>()> f);

		void addNamedUIReplace(std::string const& name, UniqueReference<Base, Base> ref);

		void closeNamedUI(std::string const& name);
		void closeUI(WeakReference<Base, Base> ref);

		void reset();
		bool shouldReset();

		void init();
		void clear();

		State();
		~State() = default;

		NOCOPYMOVE(State);
	};
}