#pragma once

#include <array>
#include <initializer_list>

#include <mem/ReferenceManager.h>

#include <wglm/glm.hpp>

#include <bitset>
#include <functional>
#include <tuple>

#define WORLD_SIZE 200

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

	class Object;
	class Signature;
	struct Possession;
	struct Vicinity;
	struct ActionResult;

	class Action
	{
	public:
		std::vector<Signature> requirements{};
		std::vector<Signature> results{};

		std::function<ActionResult(Object* obj)> runFunction;

		// obj should be the object which has access to the requirements in its possession
		ActionResult run(Object* obj);
	};

	struct ActionResult
	{
		bool success = false;

		std::vector<Possession> possessions{};
		std::vector<Action> actions{};
	};

	enum COMPONENT
	{
		GAME_POSITION,
		GRAPHIS_TILE,
		BRAIN,
		LOCOMOTION,
		NUTRITION,
		POSSESSION,
		VICINITY,
		MAX
	};

	struct GamePosition
	{
		glm::ivec2 pos;
	};

	struct GraphicsTile
	{
		int32_t blockID;
	};

	struct Brain
	{
		int32_t happiness = 0;
		int32_t energy = 0;

		std::optional<Action> currentAction;

		std::vector<Action> memory;

		Action const& findAction(std::vector<Signature> const& requirements);

		void merge(std::vector<Action>& other);
	};

	struct Possession
	{
		std::vector<UniqueReference<Object, Object>> inventory{};

		Possession() = default;

		DEFAULTMOVE(Possession);
		NOCOPY(Possession);
	};

	struct Vicinity
	{
		std::vector<WeakReference<Object, Object>> vicinity;
	};

	struct Nutrition
	{
		int32_t energy = 0;
	};

	struct Locomotion
	{
		int32_t cooldown = 0;
		int32_t fitness = 60;

		std::optional<glm::ivec2> target;
	};

	// ---------------------------------------------------

	class Signature
	{
	private:
		std::bitset<COMPONENT::MAX> data;

		friend class Signature;

	public:
		Signature& set(COMPONENT component);
		void set(std::initializer_list<COMPONENT> components);
		bool test(COMPONENT component);

		bool contains(Signature const& other) const;
	};

	struct Concept
	{
		struct Essence
		{
			float value;
			Signature signature;
		};

		std::vector<Essence> essences{};

		float value(Signature const& signature);
	};

	// ---------------------------------------------------

	class Object
	{
	private:
		GamePosition gamePosition_{};
		GraphicsTile graphicsTile_{};
		Brain brain_{};
		Nutrition nutrition_{};
		Locomotion locomotion_{};
		Possession possession_{};
		Vicinity vicinity_{};

	public:
		Handle selfHandle;

		Signature signature;

		inline GamePosition& gamePosition();
		inline Brain& brain();
		inline GraphicsTile& graphicsTile();
		inline Locomotion& locomotion();
		inline Nutrition& nutrition();
		inline Possession& possession();
		inline Vicinity& vicinity();

		Object(Handle s) : selfHandle(s) {
		}
	};

	class GameState
	{
	private:
		ReferenceManager<Object> refMan{ 100'000 };

	public:
		int32_t tick = 0;

		std::array<std::array<WeakReference<Object, Object>, WORLD_SIZE>, WORLD_SIZE> world;

		void addRenderInfo(render::RenderInfo& renderInfo);

		void runTick();

		GameState();
		~GameState() = default;
	};

	inline Brain& Object::brain() {
		assert(this->signature.test(COMPONENT::BRAIN));
		return this->brain_;
	}

	inline GraphicsTile& Object::graphicsTile() {
		assert(this->signature.test(COMPONENT::GRAPHIS_TILE));
		return this->graphicsTile_;
	}

	inline Locomotion& Object::locomotion() {
		assert(this->signature.test(COMPONENT::LOCOMOTION));
		return this->locomotion_;
	}

	inline Nutrition& Object::nutrition() {
		assert(this->signature.test(COMPONENT::NUTRITION));
		return this->nutrition_;
	}

	inline Possession& Object::possession() {
		assert(this->signature.test(COMPONENT::POSSESSION));
		return this->possession_;
	}

	inline Vicinity& Object::vicinity() {
		assert(this->signature.test(COMPONENT::VICINITY));
		return this->vicinity_;
	}

	inline GamePosition& Object::gamePosition() {
		assert(this->signature.test(COMPONENT::GAME_POSITION));
		return this->gamePosition_;
	}
}
