#pragma once

#include <iostream>
#include <cassert>
#include <type_traits>
#include <array>
#include <vector>
#include <optional>
#include <functional>

#include <wglm/glm.hpp>
#include <misc/Misc.h>

#include "Signature.h"

struct ActionResult;
struct GameObject;
struct Possession;

class Action
{
public:
	std::vector<Signature<GameObject>> requirements{};
	std::vector<Signature<GameObject>> results{};

	std::function<ActionResult(GameObject* obj)> runFunction;

	// obj should be the object which has access to the requirements in its possession
	ActionResult run(GameObject* obj);
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

	Action const& findAction(std::vector<Signature<GameObject>> const& requirements);

	void merge(std::vector<Action>& other);
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

/*#
Everything AoS
Array 1000
GameObject
int brain
int locomotion
#*/

//# Start GameObject
enum GAMEOBJECT_COMPONENT
{
	BRAIN,
	LOCOMOTION,
	MAX
};

struct GameObject
{
	Signature<GAMEOBJECT_COMPONENT> signature;
	int brain;
	int locomotion;
};

struct Everything;
struct GameObjectProxy
{
	size_t i;
	Everything& proxy;
	inline int& brain();
	inline int& locomotion();
};
struct Everything
{
	size_t count;
	std::array<GameObject, 1000> data;

	inline void add(GameObject&& obj) {
		assert(count < 1000);
		if constexpr (std::is_trivially_copyable_v<GameObject>) {
			data[count] = obj;
		}
		else {
			data[count] = std::move(obj);
		}
		count++;
	};
	inline void add(GameObject const& obj) {
		assert(count < 1000);
		data[count] = obj;
		count++;
	};
	inline void remove(size_t i) {
		assert(i < count);
		data[i] = data[count - 1];
	};
	inline GameObjectProxy get(size_t i) {
		return { i, *this };
	};
	inline Signature<GAMEOBJECT_COMPONENT>& signature(size_t i) {
		return data[i].signature;
	};
	inline int& brain(size_t i) {
		assert(signature(i).test(GAMEOBJECT_COMPONENT::BRAIN));
		return data[i].brain;
	};
	inline int& locomotion(size_t i) {
		assert(signature(i).test(GAMEOBJECT_COMPONENT::LOCOMOTION));
		return data[i].locomotion;
	};
};

inline int& GameObjectProxy::brain() {
	return proxy.brain(i);
};
inline int& GameObjectProxy::locomotion() {
	return proxy.locomotion(i);
};
//# End GameObject
