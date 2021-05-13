#pragma once

#include <iostream>
#include "GameState.h"

#include <bitset>
#include <initializer_list>

using namespace game;

template<class T>
class Signature
{
private:
	std::bitset<T::MAX> data;

	friend class Signature<T>;

public:
	Signature<T>& set(T component);
	void set(std::initializer_list<T> components);
	bool test(T component);

	bool contains(Signature const& other) const;
};

/*#
Everything AoS
Array 1000
GameObject
Brain
Locomotion
Nutrition
#*/

//# Start GameObject
enum GAMEOBJECT_COMPONENT
{
	BRAIN,
	LOCOMOTION,
	NUTRITION,
	MAX
};

struct GameObject
{
	Signature<GAMEOBJECT_COMPONENT> signature;
	Brain brain;
	Locomotion locomotion;
	Nutrition nutrition;
};

struct Everything
{
	std::array<GameObject, 1000> data;

	inline GameObjectProxy get(size_t i) {
		return {i, *this};
	};
	inline Brain& brain(size_t i) {
		return data[i].brain;
	};
	inline Locomotion& locomotion(size_t i) {
		return data[i].locomotion;
	};
	inline Nutrition& nutrition(size_t i) {
		return data[i].nutrition;
	};
};

struct GameObjectProxy
{
	size_t i;
	Everything& proxy;
	inline Brain& brain() {
		return proxy.brain(i);
	};
	inline Locomotion& locomotion() {
		return proxy.locomotion(i);
	};
	inline Nutrition& nutrition() {
		return proxy.nutrition(i);
	};
};
//# End GameObject

template<class T>
inline Signature<T>& Signature<T>::set(T component) {
	this->data.set(component);
	return *this;
}

template<class T>
inline void Signature<T>::set(std::initializer_list<T> components) {
	for (auto comp : components) {
		this->data.set(comp);
	}
}

template<class T>
inline bool Signature<T>::test(T component) {
	return this->data.test(component);
}

template<class T>
inline bool Signature<T>::contains(Signature const& other) const {
	return (this->data & other.data) == other.data;
}
