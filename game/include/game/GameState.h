#pragma once

#include <array>
#include <initializer_list>

#include <mem/ReferenceManager.h>

#include <wglm/glm.hpp>

#include <bitset>
#include <functional>
#include <tuple>

#include "GameObject.h"
#include "Signature.h"
#include "Game.h"
#include "WorldGrid.h"

#include <serial/Serializer.h>

namespace render
{
	struct RenderInfo;
}

namespace game
{
	//enum ACTION
	//{
	//	MOVE,
	//	ASK,
	//};

	//enum TARGET
	//{
	//	HEADING,
	//	LOCATION,
	//};

	// at the top of everything should be driving actions coming from essentials
	// (food, happiness, reproduction)
	// to achieve this, select actions that result in food/happiness/reproduction to start the chain.

	// action :: Requirement -> Result
	// look for Requirement in list of known Actions, and repeat.
	// Requirement is a list of Objects with certain required COMPONENTS
	//										 banned required COMPONENTS
	// Result is also a list of Objects with certain required COMPONENTS
	// the required/banned components with weights of importance

	// memory action to recall for example a location or where to find something
	// template <class T, class B>
	// struct Recall{
	//		T doAction(B);
	// };
	// example:
	// Location location {x,y};
	// Signature food;
	// food.set(COMPONENT::NUTRITION)
	// !!!! how to give it a rating of how good? (how much food, is it a friendly? useful for anything apart eating?)
	// actionList = {
	//		(Location => Food) <= gets rated high on the list driven by the 3 essentials
	// };

	// general action to look for something in vicinity
	// template <class T>
	// struct Wander{
	//		T doAction(Nothing);
	// };

	// template <class T, class B>
	// specialized on B = Brain
	// specialized on T = Location
	// Brain -> Location
	// struct Ask
	// {
	//		T doAction(Brain);
	// };

	// ---------------------------------------------------

	//struct Concept
	//{
	//	struct Essence
	//	{
	//		float value;
	//		SignatureAlias signature;
	//	};

	//	std::vector<Essence> essences{};

	//	float value(SignatureAlias const& signature);
	//};

	class GameState
	{
	public:
		Everything everything;

		std::unique_ptr<WorldGrid> world = std::make_unique<WorldGrid>();

		int32_t tick = 0;

		void addRenderInfo(render::RenderInfo& renderInfo);

		void runTick();

		bool empty(glm::ivec2 p);

		void moveInWorld(WeakObject obj, glm::ivec2 to);

		[[nodiscard]]
		UniqueObject removeFromWorld(WeakObject obj);
		void placeInWorld(UniqueObject&& obj);

		void init();
		void clearWorld();

		GameState() = default;
		~GameState();

		GameState(GameState&& other) = default;
		GameState& operator= (GameState&& other);

		NO_COPY(GameState);
	};
}

template<>
struct serial::Serializable<game::GameState>
{
	template<class Selector>
	static bool serializeEverything(Serializer& serializer, game::GameState&& obj) {
		return serializer.runAll<Selector>(
			ALL(everything),
			ALL(world),
			ALL(tick)
			);
	}

	WRITE_DEF(game::GameState) {
		return serializeEverything<Write>(serializer, std::forward<game::GameState>(obj));
	}

	READ_DEF(game::GameState) {
		return serializeEverything<Read>(serializer, std::forward<game::GameState>(obj));
	}

	PRINT_DEF(game::GameState) {
		return serializeEverything<Print>(serializer, std::forward<game::GameState>(obj));
	}
};
