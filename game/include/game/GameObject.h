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
#include <templates/Everything.h>

#include "Signature.h"
/*#
everything_name: Everything
indirection_type: integers
type: Vector
size: 100000
component[]: GamePosition
component[]: GraphicsTile
component[]: Brain
component[]: Nutrition
component[]: Locomotion
component[]: Possession
component[]: Vicinity
object_name: GameObject
#*/

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
struct GetEnum;
//# end
using SizeAlias = size_t;

struct ActionResult;
struct GameObject;
struct Possession;

enum GAMEOBJECT_COMPONENT;

struct LoopTest
{
	template<class F, class L, class... Args>
	static inline void run(F f, Args... args) {
		if constexpr (L::is_empty) {
			f(args...);
		}
		else {
			for (size_t i = 0; i < 50; i++) {
				LoopTest::run<F, L::tail, Args...>(f, i, args...);
			}
		}
	};
};

struct Loop
{
	template<class F>
	static inline void run(Everything& e, F f) {
		using A = typename te::reverse_t<te::unwrap_std_fun<decltype(f)>::args>;
		run<decltype(f), A>(e, f);
	}

	template<class F, class L, class... Args>
	static inline void run(Everything& e, F f, Args... args) {
		if constexpr (L::is_empty) {
			f(args...);
		}
		else {
			using head_stripped_ref = std::remove_reference_t<L::head>;
			head_stripped_ref::run<F, typename L::tail, Args...>(e, f, args...);
		}
	}
};

struct GameObjectProxy;
struct Everything;

using SignatureType = decltype(Signature<GAMEOBJECT_COMPONENT>::data);

template<class M, class... Ms>
struct Match
{
	GameObjectProxy proxy;

	static inline SignatureType initialize_sig() {
		SignatureType res;

		if constexpr (sizeof...(Ms) == 0) {
			res.set(static_cast<size_t>(GetEnum::val<M>()));
		}
		else {
			for (auto s : { GetEnum::val<M>(), GetEnum::val<Ms>()... }) {
				res.set(static_cast<size_t>(s));
			}
		}
		return res;
	}

	inline static SignatureType sig = initialize_sig();

	template<class T>
	inline T& get() {
		static_assert(te::Contains<T, te::List<M, Ms...>>::val());
		return proxy.get<T>();
	};

	template<class F, class L, class... Args>
	static inline void run(Everything& e, F f, Args... args) {
		size_t end = e.getlast<M>();
		for (size_t i = 1; i < end; i++) 		{
			auto const& h = e.get<M>(i);
			if (e.signature(h.index).contains(sig)) {
				Loop::run<F, L, Match<M, Ms...>, Args...>(e, f, Match<M, Ms...>{e.indirectionMap[h.index]}, args...);
			}
		}
	};
};

template<class F, class L>
struct ForEach
{
	static void run() {
		if constexpr (!L::is_empty) {
			F::run<L::head>();
			ForEach<F, L::tail>::run();
		}
	};
};

class Action
{
public:
	std::vector<Signature<GAMEOBJECT_COMPONENT>> requirements{};
	std::vector<Signature<GAMEOBJECT_COMPONENT>> results{};

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
	glm::ivec2 pos;
};

struct GraphicsTile
{
	SizeAlias index = 0;
	int32_t blockID;
};

struct Brain
{
	SizeAlias index = 0;
	int32_t happiness = 0;
	int32_t energy = 0;

	std::optional<Action> currentAction;

	std::vector<Action> memory;

	Action const& findAction(std::vector<Signature<GameObject>> const& requirements);

	void merge(std::vector<Action>& other);
};

struct Possession
{
	SizeAlias index = 0;
	//std::vector<UniqueReference<Object, Object>> inventory{};

	Possession() = default;

	DEFAULTMOVE(Possession);
	NOCOPY(Possession);
};

struct Vicinity
{
	SizeAlias index = 0;
	//std::vector<WeakReference<Object, Object>> vicinity;
	//std::unique_ptr<GameObject> test;
};

struct Nutrition
{
	SizeAlias index = 0;
	int32_t energy = 0;
};

struct Locomotion
{
	SizeAlias index = 0;
	int32_t cooldown = 0;
	int32_t fitness = 60;

	std::optional<glm::ivec2> target;
};
//# declaration
struct Everything
{
	std::vector<GameObjectProxy> indirectionMap{ 100000 };
	std::vector<GamePosition> gamepositions{ 100000 };
	std::vector<GraphicsTile> graphicstiles{ 100000 };
	std::vector<Brain> brains{ 100000 };
	std::vector<Nutrition> nutritions{ 100000 };
	std::vector<Locomotion> locomotions{ 100000 };
	std::vector<Possession> possessions{ 100000 };
	std::vector<Vicinity> vicinitys{ 100000 };
	std::vector<Signature<GAMEOBJECT_COMPONENT>> signatures{ 100000 };
	SizeAlias gamepositionlast{ 1 };
	SizeAlias graphicstilelast{ 1 };
	SizeAlias brainlast{ 1 };
	SizeAlias nutritionlast{ 1 };
	SizeAlias locomotionlast{ 1 };
	SizeAlias possessionlast{ 1 };
	SizeAlias vicinitylast{ 1 };
	size_t last{ 1 };
	template<class F>
	inline void runsimple(F f);
	template<class... Args>
	inline void runsimpleStd(std::function<void(Args...)> f);
	template<class F>
	inline void run(F f);
	inline size_t takeFreeIndex();
	inline UniqueGameObject makeUnique();
	inline WeakGameObject makeWeak();
	void remove(SizeAlias i);
	template<class T>
	inline std::vector<T>& gets();
	template<class T>
	inline size_t getlast();
	template<class T>
	inline T& get(SizeAlias i);
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
	inline void addgameposition(SizeAlias i);
	inline void addgraphicstile(SizeAlias i);
	inline void addbrain(SizeAlias i);
	inline void addnutrition(SizeAlias i);
	inline void addlocomotion(SizeAlias i);
	inline void addpossession(SizeAlias i);
	inline void addvicinity(SizeAlias i);
	inline Signature<GAMEOBJECT_COMPONENT>& signature(SizeAlias i);
};
struct GameObjectProxy
{
	Everything* proxy{ nullptr };
	size_t gameposition_{ 0 };
	GamePosition* gamepositionPtr{ nullptr };
	size_t graphicstile_{ 0 };
	GraphicsTile* graphicstilePtr{ nullptr };
	size_t brain_{ 0 };
	Brain* brainPtr{ nullptr };
	size_t nutrition_{ 0 };
	Nutrition* nutritionPtr{ nullptr };
	size_t locomotion_{ 0 };
	Locomotion* locomotionPtr{ nullptr };
	size_t possession_{ 0 };
	Possession* possessionPtr{ nullptr };
	size_t vicinity_{ 0 };
	Vicinity* vicinityPtr{ nullptr };
	inline GameObjectProxy(Everything* ptr);
	inline GameObjectProxy() = default;
	template<class T>
	inline T& get();
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
	inline bool hasgameposition();
	inline bool hasgraphicstile();
	inline bool hasbrain();
	inline bool hasnutrition();
	inline bool haslocomotion();
	inline bool haspossession();
	inline bool hasvicinity();
	inline void addgameposition();
	inline void addgraphicstile();
	inline void addbrain();
	inline void addnutrition();
	inline void addlocomotion();
	inline void addpossession();
	inline void addvicinity();
	inline GamePosition& gameposition();
	inline GraphicsTile& graphicstile();
	inline Brain& brain();
	inline Nutrition& nutrition();
	inline Locomotion& locomotion();
	inline Possession& possession();
	inline Vicinity& vicinity();
	inline Signature<GAMEOBJECT_COMPONENT>& signature();
	inline void clear();
	inline bool isNotNull() const;
	inline bool isNull() const;
};
struct UniqueGameObject
{
	size_t index{ 0 };
	Everything* proxy{ nullptr };
	inline bool hasgameposition();
	inline bool hasgraphicstile();
	inline bool hasbrain();
	inline bool hasnutrition();
	inline bool haslocomotion();
	inline bool haspossession();
	inline bool hasvicinity();
	inline void addgameposition();
	inline void addgraphicstile();
	inline void addbrain();
	inline void addnutrition();
	inline void addlocomotion();
	inline void addpossession();
	inline void addvicinity();
	inline GamePosition& gameposition();
	inline GraphicsTile& graphicstile();
	inline Brain& brain();
	inline Nutrition& nutrition();
	inline Locomotion& locomotion();
	inline Possession& possession();
	inline Vicinity& vicinity();
	inline Signature<GAMEOBJECT_COMPONENT>& signature();
	inline void clear();
	inline bool isNotNull() const;
	inline bool isNull() const;
	inline ~UniqueGameObject();
	UniqueGameObject() = default;
	NOCOPY(UniqueGameObject);
	UniqueGameObject(UniqueGameObject&& other);
	UniqueGameObject& operator=(UniqueGameObject&& other);
};
struct GetEnum
{
	template<class T>
	inline static constexpr GAMEOBJECT_COMPONENT val();
};
//# end
//# implementation
template<class F>
inline void Everything::runsimple(F f) {
	this->run2(te::wrap_in_std_fun(f));
};
template<class... Args>
inline void Everything::runsimpleStd(std::function<void(Args...)> f) {
	decltype(Signature<GAMEOBJECT_COMPONENT>::data) sig;
	for (auto s : { GetEnum::val<std::remove_reference_t<Args>>()... }) {
		sig.set(static_cast<size_t>(s));
	}
	using H = typename Head<Args...>::val;
	for (auto& h : this->gets<std::remove_reference_t<H>>()) {
		if (this->signature(h.index).contains(sig)) {
			f(this->get<std::remove_reference_t<Args>>(h.index)...);
		}
	}
};
template<class F>
inline void Everything::run(F f) {
	Loop::run(*this, te::wrap_in_std_fun(f));
};
inline size_t Everything::takeFreeIndex() {
	return last++;
};
inline UniqueGameObject Everything::makeUnique() {
	auto res = UniqueGameObject();
	res.index = takeFreeIndex();
	res.proxy = this;
	res.signature().reset();
	indirectionMap[res.index] = { this };
	return res;
};
inline WeakGameObject Everything::makeWeak() {
	auto res = WeakGameObject();
	res.index = takeFreeIndex();
	res.proxy = this;
	res.signature().reset();
	indirectionMap[res.index] = { this };
	return res;
};
inline GamePosition& Everything::gameposition(SizeAlias i) {
	return gamepositions[indirectionMap[i].gameposition_];
};
template<>
inline GamePosition& Everything::get<GamePosition>(SizeAlias i) {
	return gameposition(i);
};
template<>
inline std::vector<GamePosition>& Everything::gets<GamePosition>() {
	return gamepositions;
};
template<>
inline size_t Everything::getlast<GamePosition>() {
	return gamepositionlast;
};
inline GraphicsTile& Everything::graphicstile(SizeAlias i) {
	return graphicstiles[indirectionMap[i].graphicstile_];
};
template<>
inline GraphicsTile& Everything::get<GraphicsTile>(SizeAlias i) {
	return graphicstile(i);
};
template<>
inline std::vector<GraphicsTile>& Everything::gets<GraphicsTile>() {
	return graphicstiles;
};
template<>
inline size_t Everything::getlast<GraphicsTile>() {
	return graphicstilelast;
};
inline Brain& Everything::brain(SizeAlias i) {
	return brains[indirectionMap[i].brain_];
};
template<>
inline Brain& Everything::get<Brain>(SizeAlias i) {
	return brain(i);
};
template<>
inline std::vector<Brain>& Everything::gets<Brain>() {
	return brains;
};
template<>
inline size_t Everything::getlast<Brain>() {
	return brainlast;
};
inline Nutrition& Everything::nutrition(SizeAlias i) {
	return nutritions[indirectionMap[i].nutrition_];
};
template<>
inline Nutrition& Everything::get<Nutrition>(SizeAlias i) {
	return nutrition(i);
};
template<>
inline std::vector<Nutrition>& Everything::gets<Nutrition>() {
	return nutritions;
};
template<>
inline size_t Everything::getlast<Nutrition>() {
	return nutritionlast;
};
inline Locomotion& Everything::locomotion(SizeAlias i) {
	return locomotions[indirectionMap[i].locomotion_];
};
template<>
inline Locomotion& Everything::get<Locomotion>(SizeAlias i) {
	return locomotion(i);
};
template<>
inline std::vector<Locomotion>& Everything::gets<Locomotion>() {
	return locomotions;
};
template<>
inline size_t Everything::getlast<Locomotion>() {
	return locomotionlast;
};
inline Possession& Everything::possession(SizeAlias i) {
	return possessions[indirectionMap[i].possession_];
};
template<>
inline Possession& Everything::get<Possession>(SizeAlias i) {
	return possession(i);
};
template<>
inline std::vector<Possession>& Everything::gets<Possession>() {
	return possessions;
};
template<>
inline size_t Everything::getlast<Possession>() {
	return possessionlast;
};
inline Vicinity& Everything::vicinity(SizeAlias i) {
	return vicinitys[indirectionMap[i].vicinity_];
};
template<>
inline Vicinity& Everything::get<Vicinity>(SizeAlias i) {
	return vicinity(i);
};
template<>
inline std::vector<Vicinity>& Everything::gets<Vicinity>() {
	return vicinitys;
};
template<>
inline size_t Everything::getlast<Vicinity>() {
	return vicinitylast;
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
inline void Everything::addgameposition(SizeAlias i) {
	signature(i).set(GAMEPOSITION);
	size_t index = gamepositionlast++;
	indirectionMap[i].gameposition_ = index;
	indirectionMap[i].gamepositionPtr = &gets<GamePosition>()[index];
	indirectionMap[i].gameposition() = {};
	indirectionMap[i].gameposition().index = i;
};
inline void Everything::addgraphicstile(SizeAlias i) {
	signature(i).set(GRAPHICSTILE);
	size_t index = graphicstilelast++;
	indirectionMap[i].graphicstile_ = index;
	indirectionMap[i].graphicstilePtr = &gets<GraphicsTile>()[index];
	indirectionMap[i].graphicstile() = {};
	indirectionMap[i].graphicstile().index = i;
};
inline void Everything::addbrain(SizeAlias i) {
	signature(i).set(BRAIN);
	size_t index = brainlast++;
	indirectionMap[i].brain_ = index;
	indirectionMap[i].brainPtr = &gets<Brain>()[index];
	indirectionMap[i].brain() = {};
	indirectionMap[i].brain().index = i;
};
inline void Everything::addnutrition(SizeAlias i) {
	signature(i).set(NUTRITION);
	size_t index = nutritionlast++;
	indirectionMap[i].nutrition_ = index;
	indirectionMap[i].nutritionPtr = &gets<Nutrition>()[index];
	indirectionMap[i].nutrition() = {};
	indirectionMap[i].nutrition().index = i;
};
inline void Everything::addlocomotion(SizeAlias i) {
	signature(i).set(LOCOMOTION);
	size_t index = locomotionlast++;
	indirectionMap[i].locomotion_ = index;
	indirectionMap[i].locomotionPtr = &gets<Locomotion>()[index];
	indirectionMap[i].locomotion() = {};
	indirectionMap[i].locomotion().index = i;
};
inline void Everything::addpossession(SizeAlias i) {
	signature(i).set(POSSESSION);
	size_t index = possessionlast++;
	indirectionMap[i].possession_ = index;
	indirectionMap[i].possessionPtr = &gets<Possession>()[index];
	indirectionMap[i].possession() = {};
	indirectionMap[i].possession().index = i;
};
inline void Everything::addvicinity(SizeAlias i) {
	signature(i).set(VICINITY);
	size_t index = vicinitylast++;
	indirectionMap[i].vicinity_ = index;
	indirectionMap[i].vicinityPtr = &gets<Vicinity>()[index];
	indirectionMap[i].vicinity() = {};
	indirectionMap[i].vicinity().index = i;
};
inline Signature<GAMEOBJECT_COMPONENT>& Everything::signature(SizeAlias i) {
	return signatures[i];
};
inline GameObjectProxy::GameObjectProxy(Everything* ptr) {
	proxy = ptr;
};
inline GamePosition& GameObjectProxy::gameposition() {
	return *gamepositionPtr;
};
template<>
inline GamePosition& GameObjectProxy::get<GamePosition>() {
	return *gamepositionPtr;
};
inline GraphicsTile& GameObjectProxy::graphicstile() {
	return *graphicstilePtr;
};
template<>
inline GraphicsTile& GameObjectProxy::get<GraphicsTile>() {
	return *graphicstilePtr;
};
inline Brain& GameObjectProxy::brain() {
	return *brainPtr;
};
template<>
inline Brain& GameObjectProxy::get<Brain>() {
	return *brainPtr;
};
inline Nutrition& GameObjectProxy::nutrition() {
	return *nutritionPtr;
};
template<>
inline Nutrition& GameObjectProxy::get<Nutrition>() {
	return *nutritionPtr;
};
inline Locomotion& GameObjectProxy::locomotion() {
	return *locomotionPtr;
};
template<>
inline Locomotion& GameObjectProxy::get<Locomotion>() {
	return *locomotionPtr;
};
inline Possession& GameObjectProxy::possession() {
	return *possessionPtr;
};
template<>
inline Possession& GameObjectProxy::get<Possession>() {
	return *possessionPtr;
};
inline Vicinity& GameObjectProxy::vicinity() {
	return *vicinityPtr;
};
template<>
inline Vicinity& GameObjectProxy::get<Vicinity>() {
	return *vicinityPtr;
};
inline bool WeakGameObject::hasgameposition() {
	return proxy->hasgameposition(index);
};
inline bool WeakGameObject::hasgraphicstile() {
	return proxy->hasgraphicstile(index);
};
inline bool WeakGameObject::hasbrain() {
	return proxy->hasbrain(index);
};
inline bool WeakGameObject::hasnutrition() {
	return proxy->hasnutrition(index);
};
inline bool WeakGameObject::haslocomotion() {
	return proxy->haslocomotion(index);
};
inline bool WeakGameObject::haspossession() {
	return proxy->haspossession(index);
};
inline bool WeakGameObject::hasvicinity() {
	return proxy->hasvicinity(index);
};
inline void WeakGameObject::addgameposition() {
	proxy->addgameposition(index);
};
inline void WeakGameObject::addgraphicstile() {
	proxy->addgraphicstile(index);
};
inline void WeakGameObject::addbrain() {
	proxy->addbrain(index);
};
inline void WeakGameObject::addnutrition() {
	proxy->addnutrition(index);
};
inline void WeakGameObject::addlocomotion() {
	proxy->addlocomotion(index);
};
inline void WeakGameObject::addpossession() {
	proxy->addpossession(index);
};
inline void WeakGameObject::addvicinity() {
	proxy->addvicinity(index);
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
inline Signature<GAMEOBJECT_COMPONENT>& WeakGameObject::signature() {
	return proxy->signature(index);
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
inline bool UniqueGameObject::hasgameposition() {
	return proxy->hasgameposition(index);
};
inline bool UniqueGameObject::hasgraphicstile() {
	return proxy->hasgraphicstile(index);
};
inline bool UniqueGameObject::hasbrain() {
	return proxy->hasbrain(index);
};
inline bool UniqueGameObject::hasnutrition() {
	return proxy->hasnutrition(index);
};
inline bool UniqueGameObject::haslocomotion() {
	return proxy->haslocomotion(index);
};
inline bool UniqueGameObject::haspossession() {
	return proxy->haspossession(index);
};
inline bool UniqueGameObject::hasvicinity() {
	return proxy->hasvicinity(index);
};
inline void UniqueGameObject::addgameposition() {
	proxy->addgameposition(index);
};
inline void UniqueGameObject::addgraphicstile() {
	proxy->addgraphicstile(index);
};
inline void UniqueGameObject::addbrain() {
	proxy->addbrain(index);
};
inline void UniqueGameObject::addnutrition() {
	proxy->addnutrition(index);
};
inline void UniqueGameObject::addlocomotion() {
	proxy->addlocomotion(index);
};
inline void UniqueGameObject::addpossession() {
	proxy->addpossession(index);
};
inline void UniqueGameObject::addvicinity() {
	proxy->addvicinity(index);
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
inline Signature<GAMEOBJECT_COMPONENT>& UniqueGameObject::signature() {
	return proxy->signature(index);
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
inline UniqueGameObject::~UniqueGameObject() {
	clear();
};
template<>
inline constexpr GAMEOBJECT_COMPONENT GetEnum::val<GamePosition>() {
	return GAMEPOSITION;
};
template<>
inline constexpr GAMEOBJECT_COMPONENT GetEnum::val<GraphicsTile>() {
	return GRAPHICSTILE;
};
template<>
inline constexpr GAMEOBJECT_COMPONENT GetEnum::val<Brain>() {
	return BRAIN;
};
template<>
inline constexpr GAMEOBJECT_COMPONENT GetEnum::val<Nutrition>() {
	return NUTRITION;
};
template<>
inline constexpr GAMEOBJECT_COMPONENT GetEnum::val<Locomotion>() {
	return LOCOMOTION;
};
template<>
inline constexpr GAMEOBJECT_COMPONENT GetEnum::val<Possession>() {
	return POSSESSION;
};
template<>
inline constexpr GAMEOBJECT_COMPONENT GetEnum::val<Vicinity>() {
	return VICINITY;
};
//# end
