#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <optional>
#include <functional>

#include <wglm/glm.hpp>
#include <misc/Misc.h>

#include "Game.h"

using SizeAlias = size_t;

struct GameObjectProxy;
struct WeakGameObject;
struct UniqueGameObject;
struct GetEnum;

struct ActionResult;
struct GameObject;
struct Possession;

class Action
{
public:
	//std::vector<Signature<GAMEOBJECT_COMPONENT>> requirements{};
	//std::vector<Signature<GAMEOBJECT_COMPONENT>> results{};

	//std::function<ActionResult(GameObject* obj)> runFunction;

	// obj should be the object which has access to the requirements in its possession
	//ActionResult run(GameObject* obj);
};

struct ActionResult
{
	bool success = false;

	std::vector<Possession> possessions{};
	std::vector<Action> actions{};
};

struct GamePosition
{
	SizeAlias index = 0;
#ifdef GAMEOBJECT_POINTER_CACHE
	game::Everything::indirection* indirectionCache;
#endif // GAMEOBJECT_POINTER_CACHE

	glm::ivec2 pos;
};

struct GraphicsTile
{
	SizeAlias index = 0;
#ifdef GAMEOBJECT_POINTER_CACHE
	game::Everything::indirection* indirectionCache;
#endif // GAMEOBJECT_POINTER_CACHE

	int32_t blockID;
};

struct Brain
{
	SizeAlias index = 0;
#ifdef GAMEOBJECT_POINTER_CACHE
	game::Everything::indirection* indirectionCache;
#endif // GAMEOBJECT_POINTER_CACHE

	int32_t happiness = 0;
	int32_t energy = 0;

	std::optional<Action> currentAction;

	std::vector<Action> memory;

	//Action const& findAction(std::vector<Signature<GameObject>> const& requirements);

	void merge(std::vector<Action>& other);
};

struct Possession
{
	SizeAlias index = 0;
#ifdef GAMEOBJECT_POINTER_CACHE
	game::Everything::indirection* indirectionCache;
#endif // GAMEOBJECT_POINTER_CACHE

	//std::vector<UniqueReference<Object, Object>> inventory{};

	Possession() = default;

	DEFAULTMOVE(Possession);
	NOCOPY(Possession);
};

struct Vicinity
{
	SizeAlias index = 0;
#ifdef GAMEOBJECT_POINTER_CACHE
	game::Everything::indirection* indirectionCache;
#endif // GAMEOBJECT_POINTER_CACHE

	//std::vector<WeakReference<Object, Object>> vicinity;
	//std::unique_ptr<GameObject> test;
};

struct Nutrition
{
	SizeAlias index = 0;
#ifdef GAMEOBJECT_POINTER_CACHE
	game::Everything::indirection* indirectionCache;
#endif // GAMEOBJECT_POINTER_CACHE

	int32_t energy = 0;
};

struct Locomotion
{
	SizeAlias index = 0;
#ifdef GAMEOBJECT_POINTER_CACHE
	game::Everything::indirection* indirectionCache;
#endif // GAMEOBJECT_POINTER_CACHE

	int32_t cooldown = 0;
	int32_t fitness = 60;

	std::optional<glm::ivec2> target;
};
