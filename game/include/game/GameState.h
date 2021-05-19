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

#define WORLD_SIZE 100

namespace render
{
	struct RenderInfo;
}

namespace game
{
	using SignatureAlias = Signature<GAMEOBJECT_COMPONENT>;

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

	struct Concept
	{
		struct Essence
		{
			float value;
			SignatureAlias signature;
		};

		std::vector<Essence> essences{};

		float value(SignatureAlias const& signature);
	};

	// ---------------------------------------------------

	// Object - SoA or AoS
	// Member_1
	// Member_2
	// |
	// |
	// |
	// Member_n

	// class Everything {
	//     std::array<Object> stuff;
	//
	//     Member_1& member_1(size_t i) {
	//	       return stuff[i].member_1;
	//     };
	//     Member_2& member_2(size_t i);
	// };

	// class Everything {
	//     std::array<member_1> member_1s
	//     std::array<member_2> member_2s
	//     |
	//     |
	//     |
	//     std::array<member_n> member_ns
	//
	//     Member_1& member_1(size_t i) {
	//         return member_1s[i];
	//     };
	//     Member_2& member_2(size_t i);
	// };

	class GameState
	{
	private:
		std::unique_ptr<Everything> everything;

	public:
		int32_t tick = 0;

		//std::array<std::array<WeakReference<Object, Object>, WORLD_SIZE>, WORLD_SIZE> world;

		void addRenderInfo(render::RenderInfo& renderInfo);

		void runTick();

		GameState();
		~GameState() = default;
	};
}
