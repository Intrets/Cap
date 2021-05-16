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
structure: AoS
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
struct WeakGameObject;
struct UniqueGameObject;
//# end
//# declaration
struct Everything
{
	std::vector<GameObject> data{ 100 };
	size_t last{ 0 };
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
	inline ~WeakGameObject();
	inline void clear();
	inline bool isNotNull() const;
	inline bool isNull() const;
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
	inline void clear();
	inline bool isNotNull() const;
	inline bool isNull() const;
	NOCOPY(UniqueGameObject);
	UniqueGameObject(UniqueGameObject&& other);
	UniqueGameObject& operator=(UniqueGameObject&& other);
};
//# end
//# implementation
inline GamePosition& Everything::gameposition(SizeAlias i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::GAMEPOSITION));
	return data[i].gameposition;
};
inline GraphicsTile& Everything::graphicstile(SizeAlias i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::GRAPHICSTILE));
	return data[i].graphicstile;
};
inline Brain& Everything::brain(SizeAlias i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::BRAIN));
	return data[i].brain;
};
inline Nutrition& Everything::nutrition(SizeAlias i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::NUTRITION));
	return data[i].nutrition;
};
inline Locomotion& Everything::locomotion(SizeAlias i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::LOCOMOTION));
	return data[i].locomotion;
};
inline Possession& Everything::possession(SizeAlias i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::POSSESSION));
	return data[i].possession;
};
inline Vicinity& Everything::vicinity(SizeAlias i) {
	assert(i < 100);
	assert(signature(i).test(GAMEOBJECT_COMPONENT::VICINITY));
	return data[i].vicinity;
};
inline bool Everything::hasgameposition(SizeAlias i) {
	return signature(i).test(GAMEOBJECT_COMPONENT::GAMEPOSITION);
};
inline bool Everything::hasgraphicstile(SizeAlias i) {
	return signature(i).test(GAMEOBJECT_COMPONENT::GRAPHICSTILE);
};
inline bool Everything::hasbrain(SizeAlias i) {
	return signature(i).test(GAMEOBJECT_COMPONENT::BRAIN);
};
inline bool Everything::hasnutrition(SizeAlias i) {
	return signature(i).test(GAMEOBJECT_COMPONENT::NUTRITION);
};
inline bool Everything::haslocomotion(SizeAlias i) {
	return signature(i).test(GAMEOBJECT_COMPONENT::LOCOMOTION);
};
inline bool Everything::haspossession(SizeAlias i) {
	return signature(i).test(GAMEOBJECT_COMPONENT::POSSESSION);
};
inline bool Everything::hasvicinity(SizeAlias i) {
	return signature(i).test(GAMEOBJECT_COMPONENT::VICINITY);
};
inline Signature<GAMEOBJECT_COMPONENT>& Everything::signature(SizeAlias i) {
	return data[i].signature;
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
inline WeakGameObject::~WeakGameObject() {
	clear();
};
inline void WeakGameObject::clear() {
	proxy->remove(index);
	index = 0;
	proxy = nullptr;
};
inline bool WeakGameObject::isNotNull() const {
	return index != 0;
};
inline bool WeakGameObject::isNull() const {
	return index == 0;
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
	clear();
};
inline void UniqueGameObject::clear() {
	proxy->remove(index);
	index = 0;
	proxy = nullptr;
};
inline bool UniqueGameObject::isNotNull() const {
	return index != 0;
};
inline bool UniqueGameObject::isNull() const {
	return index == 0;
};
//# end
