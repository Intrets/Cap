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
Everything SoA
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

//# forward
struct Everything;
struct GameObject;
enum GAMEOBJECT_COMPONENT
{
	None,
	GAMEPOSITION,
	GRAPHICSTILE,
	BRAIN,
	NUTRITION,
	LOCOMOTION,
	POSSESSION,
	VICINITY,
	MAX
};
//# end
//# declaration
struct Everything
{
	std::vector<Signature<GAMEOBJECT_COMPONENT>> signatures{ 100 };
	std::vector<GamePosition> gamepositions{ 100 };
	std::vector<GraphicsTile> graphicstiles{ 100 };
	std::vector<Brain> brains{ 100 };
	std::vector<Nutrition> nutritions{ 100 };
	std::vector<Locomotion> locomotions{ 100 };
	std::vector<Possession> possessions{ 100 };
	std::vector<Vicinity> vicinitys{ 100 };
	inline Signature<GAMEOBJECT_COMPONENT>& signature(size_t i);
	inline GamePosition& gameposition(size_t i);
	inline GraphicsTile& graphicstile(size_t i);
	inline Brain& brain(size_t i);
	inline Nutrition& nutrition(size_t i);
	inline Locomotion& locomotion(size_t i);
	inline Possession& possession(size_t i);
	inline Vicinity& vicinity(size_t i);
};
struct GameObject
{
	Signature<GAMEOBJECT_COMPONENT> signature{};
	GamePosition gameposition{};
	GraphicsTile graphicstile{};
	Brain brain{};
	Nutrition nutrition{};
	Locomotion locomotion{};
	Possession possession{};
	Vicinity vicinity{};
};
//# end
//# implementation
inline Signature<GAMEOBJECT_COMPONENT>& Everything::signature(size_t i) {
	assert(i < 100);
	return signatures[i];
};
inline GamePosition& Everything::gameposition(size_t i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::GAMEPOSITION));
	return gamepositions[i];
};
inline GraphicsTile& Everything::graphicstile(size_t i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::GRAPHICSTILE));
	return graphicstiles[i];
};
inline Brain& Everything::brain(size_t i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::BRAIN));
	return brains[i];
};
inline Nutrition& Everything::nutrition(size_t i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::NUTRITION));
	return nutritions[i];
};
inline Locomotion& Everything::locomotion(size_t i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::LOCOMOTION));
	return locomotions[i];
};
inline Possession& Everything::possession(size_t i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::POSSESSION));
	return possessions[i];
};
inline Vicinity& Everything::vicinity(size_t i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::VICINITY));
	return vicinitys[i];
};
//# end
