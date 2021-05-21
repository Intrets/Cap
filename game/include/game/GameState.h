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

constexpr auto WORLD_SIZE = 200000;

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
	private:
		EverythingS everything2;

	public:
		int32_t tick = 0;

		//std::array<std::array<WeakReference<Object, Object>, WORLD_SIZE>, WORLD_SIZE> world;

		void addRenderInfo(render::RenderInfo& renderInfo);

		void runTick();

		GameState();
		~GameState() = default;
	};
}
