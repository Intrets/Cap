#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <optional>
#include <functional>
#include <deque>
#include <map>

#include <wglm/glm.hpp>
#include <misc/Misc.h>

#include "Game.h"

using SizeAlias = size_t;

struct GameObjectProxy;
struct GetEnum;

struct ActionResult;
struct GameObject;
struct Possession;

namespace game
{
	struct WorldGrid;
}

class Action
{
public:
	//std::vector<Signature<GAMEOBJECT_COMPONENT>> requirements{};
	//std::vector<Signature<GAMEOBJECT_COMPONENT>> results{};

	//std::function<ActionResult(GameObject* obj)> runFunction;

	// obj should be the object which has access to the requirements in its possession
	//ActionResult run(GameObject* obj);
};

struct PathFinding
{
	glm::ivec2 target;
	glm::ivec2 current;

	glm::ivec2 extentPosition{ 0,0 };
	glm::ivec2 extendSize{ WORLD_SIZE, WORLD_SIZE };

	std::map<uint64_t, int32_t> visited{};

	std::vector<glm::vec2> path{};

	glm::ivec2 previousDirection = { 1, 1 };

	uint32_t i = 0;

	bool step(game::WorldGrid& grid);
};

struct ActionResult
{
	bool success = false;

	std::vector<Possession> possessions{};
	std::vector<Action> actions{};
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

	//Action const& findAction(std::vector<Signature<GameObject>> const& requirements);

	void merge(std::vector<Action>& other);
};

struct Target
{
	std::deque<glm::ivec2> path;
};

struct Possession
{
	//std::vector<UniqueReference<Object, Object>> inventory{};

	Possession() = default;

	DEFAULTMOVE(Possession);
	NOCOPY(Possession);
};

struct Vicinity
{
	//std::vector<WeakReference<Object, Object>> vicinity;
	//std::unique_ptr<GameObject> test;
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
