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
indirection_type: integers
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
struct WeakGameObject;
struct UniqueGameObject;
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
	SizeAlias gamepositionlast{ 0 };
	SizeAlias graphicstilelast{ 0 };
	SizeAlias brainlast{ 0 };
	SizeAlias nutritionlast{ 0 };
	SizeAlias locomotionlast{ 0 };
	SizeAlias possessionlast{ 0 };
	SizeAlias vicinitylast{ 0 };
	void remove(SizeAlias i);
	inline GamePosition& gameposition(SizeAlias i);
	inline GraphicsTile& graphicstile(SizeAlias i);
	inline Brain& brain(SizeAlias i);
	inline Nutrition& nutrition(SizeAlias i);
	inline Locomotion& locomotion(SizeAlias i);
	inline Possession& possession(SizeAlias i);
	inline Vicinity& vicinity(SizeAlias i);
	inline bool hasgameposition(SizeAlias i);
	inline bool hasgraphicstile(SizeAlias i);
	inline bool hasbrain(SizeAlias i);
	inline bool hasnutrition(SizeAlias i);
	inline bool haslocomotion(SizeAlias i);
	inline bool haspossession(SizeAlias i);
	inline bool hasvicinity(SizeAlias i);
	inline Signature<GAMEOBJECT_COMPONENT>& signature(SizeAlias i);
};
struct GameObjectProxy
{
	Everything* proxy;
	size_t gameposition_;
	size_t graphicstile_;
	size_t brain_;
	size_t nutrition_;
	size_t locomotion_;
	size_t possession_;
	size_t vicinity_;
	inline GamePosition& gameposition();
	inline GraphicsTile& graphicstile();
	inline Brain& brain();
	inline Nutrition& nutrition();
	inline Locomotion& locomotion();
	inline Possession& possession();
	inline Vicinity& vicinity();
};
struct WeakGameObject
{
	size_t index{ 0 };
	Everything* proxy{ nullptr };
	inline GamePosition& gameposition();
	inline GraphicsTile& graphicstile();
	inline Brain& brain();
	inline Nutrition& nutrition();
	inline Locomotion& locomotion();
	inline Possession& possession();
	inline Vicinity& vicinity();
};
struct UniqueGameObject
{
	size_t index{ 0 };
	Everything* proxy{ nullptr };
	inline GamePosition& gameposition();
	inline GraphicsTile& graphicstile();
	inline Brain& brain();
	inline Nutrition& nutrition();
	inline Locomotion& locomotion();
	inline Possession& possession();
	inline Vicinity& vicinity();
	inline ~UniqueGameObject();
};
//# end
//# implementation
void Everything::remove(SizeAlias i) {
	if (hasgameposition(i)) {
		gamepositions[indirectionMap[i].gameposition_] = std::move(gamepositions[indirectionMap[gamepositionlast].gameposition_]);
	}
	if (hasgraphicstile(i)) {
		graphicstiles[indirectionMap[i].graphicstile_] = std::move(graphicstiles[indirectionMap[graphicstilelast].graphicstile_]);
	}
	if (hasbrain(i)) {
		brains[indirectionMap[i].brain_] = std::move(brains[indirectionMap[brainlast].brain_]);
	}
	if (hasnutrition(i)) {
		nutritions[indirectionMap[i].nutrition_] = std::move(nutritions[indirectionMap[nutritionlast].nutrition_]);
	}
	if (haslocomotion(i)) {
		locomotions[indirectionMap[i].locomotion_] = std::move(locomotions[indirectionMap[locomotionlast].locomotion_]);
	}
	if (haspossession(i)) {
		possessions[indirectionMap[i].possession_] = std::move(possessions[indirectionMap[possessionlast].possession_]);
	}
	if (hasvicinity(i)) {
		vicinitys[indirectionMap[i].vicinity_] = std::move(vicinitys[indirectionMap[vicinitylast].vicinity_]);
	}
};
inline GamePosition& Everything::gameposition(SizeAlias i) {
	return gamepositions[indirectionMap[i].gameposition_];
};
inline GraphicsTile& Everything::graphicstile(SizeAlias i) {
	return graphicstiles[indirectionMap[i].graphicstile_];
};
inline Brain& Everything::brain(SizeAlias i) {
	return brains[indirectionMap[i].brain_];
};
inline Nutrition& Everything::nutrition(SizeAlias i) {
	return nutritions[indirectionMap[i].nutrition_];
};
inline Locomotion& Everything::locomotion(SizeAlias i) {
	return locomotions[indirectionMap[i].locomotion_];
};
inline Possession& Everything::possession(SizeAlias i) {
	return possessions[indirectionMap[i].possession_];
};
inline Vicinity& Everything::vicinity(SizeAlias i) {
	return vicinitys[indirectionMap[i].vicinity_];
};
inline bool Everything::hasgameposition(SizeAlias i) {
	return indirectionMap[i].gameposition_ != 0;
};
inline bool Everything::hasgraphicstile(SizeAlias i) {
	return indirectionMap[i].graphicstile_ != 0;
};
inline bool Everything::hasbrain(SizeAlias i) {
	return indirectionMap[i].brain_ != 0;
};
inline bool Everything::hasnutrition(SizeAlias i) {
	return indirectionMap[i].nutrition_ != 0;
};
inline bool Everything::haslocomotion(SizeAlias i) {
	return indirectionMap[i].locomotion_ != 0;
};
inline bool Everything::haspossession(SizeAlias i) {
	return indirectionMap[i].possession_ != 0;
};
inline bool Everything::hasvicinity(SizeAlias i) {
	return indirectionMap[i].vicinity_ != 0;
};
inline Signature<GAMEOBJECT_COMPONENT>& Everything::signature(SizeAlias i) {
	return signatures[i];
};
inline GamePosition& GameObjectProxy::gameposition() {
	return proxy->gamepositions[gameposition_];
};
inline GraphicsTile& GameObjectProxy::graphicstile() {
	return proxy->graphicstiles[graphicstile_];
};
inline Brain& GameObjectProxy::brain() {
	return proxy->brains[brain_];
};
inline Nutrition& GameObjectProxy::nutrition() {
	return proxy->nutritions[nutrition_];
};
inline Locomotion& GameObjectProxy::locomotion() {
	return proxy->locomotions[locomotion_];
};
inline Possession& GameObjectProxy::possession() {
	return proxy->possessions[possession_];
};
inline Vicinity& GameObjectProxy::vicinity() {
	return proxy->vicinitys[vicinity_];
};
inline GamePosition& WeakGameObject::gameposition() {
	return proxy->gameposition(index);
};
inline GraphicsTile& WeakGameObject::graphicstile() {
	return proxy->graphicstile(index);
};
inline Brain& WeakGameObject::brain() {
	return proxy->brain(index);
};
inline Nutrition& WeakGameObject::nutrition() {
	return proxy->nutrition(index);
};
inline Locomotion& WeakGameObject::locomotion() {
	return proxy->locomotion(index);
};
inline Possession& WeakGameObject::possession() {
	return proxy->possession(index);
};
inline Vicinity& WeakGameObject::vicinity() {
	return proxy->vicinity(index);
};
inline GamePosition& UniqueGameObject::gameposition() {
	return proxy->gameposition(index);
};
inline GraphicsTile& UniqueGameObject::graphicstile() {
	return proxy->graphicstile(index);
};
inline Brain& UniqueGameObject::brain() {
	return proxy->brain(index);
};
inline Nutrition& UniqueGameObject::nutrition() {
	return proxy->nutrition(index);
};
inline Locomotion& UniqueGameObject::locomotion() {
	return proxy->locomotion(index);
};
inline Possession& UniqueGameObject::possession() {
	return proxy->possession(index);
};
inline Vicinity& UniqueGameObject::vicinity() {
	return proxy->vicinity(index);
};
inline UniqueGameObject::~UniqueGameObject() {
	// TODO
};
//# end
