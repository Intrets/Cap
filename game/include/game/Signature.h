#pragma once

#include <bitset>
#include <initializer_list>

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
