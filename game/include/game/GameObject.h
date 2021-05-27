#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <optional>
#include <functional>
#include <deque>
#include <map>
#include <queue>

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

struct Front
{
	glm::ivec2 current;
	uint32_t D = 9999999;
	bool collided = false;
	size_t direction = 0;
	int32_t winding = 0;
	bool clockwise = false;
	std::vector<glm::ivec2> path{};
	std::vector<glm::ivec2> waypoints{};
};

struct PathFinding
{
	int stage = 0;

	std::queue<Front> front;

	glm::ivec2 target;
	glm::ivec2 start;
	bool found = false;
	//glm::ivec2 current;

	glm::ivec2 extentPosition{ 0,0 };
	glm::ivec2 extendSize{ WORLD_SIZE, WORLD_SIZE };

	std::map<uint64_t, int32_t> visited{};

	//std::vector<glm::vec2> path{};
	std::vector<glm::vec2> searched{};


	//bool collided = false;
	//size_t direction = 0;
	//bool previousClockWise = false;

	//int32_t winding = 0;

	uint32_t tick = 0;

	int32_t count = 0;

	// --------

	std::vector<glm::ivec2> path{};
	std::vector<glm::ivec2> prunedPath{};
	std::vector<glm::ivec2> waypoints{};
	std::deque<glm::ivec2> newWaypoints{};
	glm::ivec2 lastP = { 0,0 };
	bool cutoff = false;

	int32_t lastD = 999999;

	// --------

	std::vector<glm::ivec2> finalPath{};

	glm::ivec2 highlight1;
	glm::ivec2 highlight2;

	std::unique_ptr<PathFinding> F;

	bool stage1(game::WorldGrid& grid);
	bool stage2(game::WorldGrid& grid);
	bool stage3(game::WorldGrid& grid);

	bool step(game::WorldGrid& grid);
	void debugRender();
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
