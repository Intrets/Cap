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

enum GAMEOBJECT_COMPONENT;

class Action
{
public:
	std::vector<Signature<GAMEOBJECT_COMPONENT>> requirements{};
	std::vector<Signature<GAMEOBJECT_COMPONENT>> results{};

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

/*#
Everything AoS
Vector 100
GameObject
GamePosition
GraphicsTile
Brain
Nutrition
Locomotion
Possession
Vicinity
#*/

//# Start GameObject
enum GAMEOBJECT_COMPONENT
{
	GAMEPOSITION,
	GRAPHICSTILE,
	BRAIN,
	NUTRITION,
	LOCOMOTION,
	POSSESSION,
	VICINITY,
	MAX
};

struct GameObject
{
	Signature<GAMEOBJECT_COMPONENT> signature;
	GamePosition gameposition;
	GraphicsTile graphicstile;
	Brain brain;
	Nutrition nutrition;
	Locomotion locomotion;
	Possession possession;
	Vicinity vicinity;
};

struct Everything;
struct GameObjectProxy
{
	size_t i;
	Everything& proxy;
	inline GamePosition& gameposition();
	inline GraphicsTile& graphicstile();
	inline Brain& brain();
	inline Nutrition& nutrition();
	inline Locomotion& locomotion();
	inline Possession& possession();
	inline Vicinity& vicinity();
};
struct Everything
{
	size_t count = 0;
	std::vector<GameObject> data{ 100 };

	template<int t = 0>
	inline void add(GameObject&& obj) {
		assert(count < 100);
		if constexpr (std::is_copyable_v<GameObject>) {
			data[count] = obj;
		}
		else {
			data[count] = std::move(obj);
		}
		count++;
	};
	template<int t = 0>
	inline void add(GameObject const& obj) {
		assert(count < 100);
		data[count] = obj;
		count++;
	};
	template<int t=0>
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
	inline GamePosition& gameposition(size_t i) {
		assert(signature(i).test(GAMEOBJECT_COMPONENT::GAMEPOSITION));
		return data[i].gameposition;
	};
	inline GraphicsTile& graphicstile(size_t i) {
		assert(signature(i).test(GAMEOBJECT_COMPONENT::GRAPHICSTILE));
		return data[i].graphicstile;
	};
	inline Brain& brain(size_t i) {
		assert(signature(i).test(GAMEOBJECT_COMPONENT::BRAIN));
		return data[i].brain;
	};
	inline Nutrition& nutrition(size_t i) {
		assert(signature(i).test(GAMEOBJECT_COMPONENT::NUTRITION));
		return data[i].nutrition;
	};
	inline Locomotion& locomotion(size_t i) {
		assert(signature(i).test(GAMEOBJECT_COMPONENT::LOCOMOTION));
		return data[i].locomotion;
	};
	inline Possession& possession(size_t i) {
		assert(signature(i).test(GAMEOBJECT_COMPONENT::POSSESSION));
		return data[i].possession;
	};
	inline Vicinity& vicinity(size_t i) {
		assert(signature(i).test(GAMEOBJECT_COMPONENT::VICINITY));
		return data[i].vicinity;
	};
};

inline GamePosition& GameObjectProxy::gameposition() {
	return proxy.gameposition(i);
};
inline GraphicsTile& GameObjectProxy::graphicstile() {
	return proxy.graphicstile(i);
};
inline Brain& GameObjectProxy::brain() {
	return proxy.brain(i);
};
inline Nutrition& GameObjectProxy::nutrition() {
	return proxy.nutrition(i);
};
inline Locomotion& GameObjectProxy::locomotion() {
	return proxy.locomotion(i);
};
inline Possession& GameObjectProxy::possession() {
	return proxy.possession(i);
};
inline Vicinity& GameObjectProxy::vicinity() {
	return proxy.vicinity(i);
};
//# End GameObject
