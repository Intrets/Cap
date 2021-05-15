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
/*#
everything_name: Everything
structure: sSoA
indirection_type: pointers
type: Vector
size: 100
component[]: GamePosition
component[]: GraphicsTile
component[]: Brain
component[]: Nutrition
component[]: Locomotion
component[]: Possession
component[]: Vicinity
object_name: GameObject
#*/

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
//# forward
using SizeAlias = size_t;
struct Everything;
struct GameObject;
enum GAMEOBJECT_COMPONENT
{
	GAMEPOSITION,
	GRAPHICSTILE,
	BRAIN,
	NUTRITION,
	LOCOMOTION,
	POSSESSION,
	VICINITY,
	None,
	MAX
};
struct GameObjectProxy;
//# end
//# declaration
struct Everything
{
	std::vector<GameObjectProxy> indirectionMap{ 100 };
	std::vector<GamePosition> gamepositions{ 100 };
	std::vector<GraphicsTile> graphicstiles{ 100 };
	std::vector<Brain> brains{ 100 };
	std::vector<Nutrition> nutritions{ 100 };
	std::vector<Locomotion> locomotions{ 100 };
	std::vector<Possession> possessions{ 100 };
	std::vector<Vicinity> vicinitys{ 100 };
	std::vector<Signature<GAMEOBJECT_COMPONENT>> signatures{ 100 };
	inline GamePosition& gameposition(SizeAlias i);
	inline GraphicsTile& graphicstile(SizeAlias i);
	inline Brain& brain(SizeAlias i);
	inline Nutrition& nutrition(SizeAlias i);
	inline Locomotion& locomotion(SizeAlias i);
	inline Possession& possession(SizeAlias i);
	inline Vicinity& vicinity(SizeAlias i);
	inline Signature<GAMEOBJECT_COMPONENT>& signature(SizeAlias i);
};
struct GameObject
{
	GamePosition gameposition{};
	GraphicsTile graphicstile{};
	Brain brain{};
	Nutrition nutrition{};
	Locomotion locomotion{};
	Possession possession{};
	Vicinity vicinity{};
	Signature<GAMEOBJECT_COMPONENT> signature{};
};
struct GameObjectProxy
{
	GamePosition* gameposition_;
	GraphicsTile* graphicstile_;
	Brain* brain_;
	Nutrition* nutrition_;
	Locomotion* locomotion_;
	Possession* possession_;
	Vicinity* vicinity_;
	inline GamePosition& gameposition();
	inline GraphicsTile& graphicstile();
	inline Brain& brain();
	inline Nutrition& nutrition();
	inline Locomotion& locomotion();
	inline Possession& possession();
	inline Vicinity& vicinity();
};
//# end
//# implementation
inline GamePosition& Everything::gameposition(SizeAlias i) {
	return indirectionMap[i].gameposition();
};
inline GraphicsTile& Everything::graphicstile(SizeAlias i) {
	return indirectionMap[i].graphicstile();
};
inline Brain& Everything::brain(SizeAlias i) {
	return indirectionMap[i].brain();
};
inline Nutrition& Everything::nutrition(SizeAlias i) {
	return indirectionMap[i].nutrition();
};
inline Locomotion& Everything::locomotion(SizeAlias i) {
	return indirectionMap[i].locomotion();
};
inline Possession& Everything::possession(SizeAlias i) {
	return indirectionMap[i].possession();
};
inline Vicinity& Everything::vicinity(SizeAlias i) {
	return indirectionMap[i].vicinity();
};
inline Signature<GAMEOBJECT_COMPONENT>& Everything::signature(SizeAlias i) {
	return signatures[i];
};
inline GamePosition& GameObjectProxy::gameposition() {
	return *gameposition_;
};
inline GraphicsTile& GameObjectProxy::graphicstile() {
	return *graphicstile_;
};
inline Brain& GameObjectProxy::brain() {
	return *brain_;
};
inline Nutrition& GameObjectProxy::nutrition() {
	return *nutrition_;
};
inline Locomotion& GameObjectProxy::locomotion() {
	return *locomotion_;
};
inline Possession& GameObjectProxy::possession() {
	return *possession_;
};
inline Vicinity& GameObjectProxy::vicinity() {
	return *vicinity_;
};
//# end
