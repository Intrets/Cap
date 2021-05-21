#pragma once

#include <bitset>
#include <initializer_list>

template<class T>
class Signature
{
public:
	//std::bitset<T::MAX> data;
	std::bitset<100> data;

	friend class Signature<T>;

	Signature<T>& set(T component);
	void set(std::initializer_list<T> components);
	bool test(T component) const;
	void reset();

	bool contains(Signature const& other) const;
	bool contains(decltype(Signature<T>::data) const& other) const;
};

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
inline bool Signature<T>::test(T component) const {
	return this->data.test(component);
}

template<class T>
inline void Signature<T>::reset() {
	this->data.reset();
}

template<class T>
inline bool Signature<T>::contains(Signature const& other) const {
	return (this->data & other.data) == other.data;
}

template<class T>
inline bool Signature<T>::contains(decltype(Signature<T>::data) const& other) const {
	return (this->data & other) == other;
}
