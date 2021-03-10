#pragma once

#include <misc/Misc.h>

template <class T>
class UpdatedCache
{
private:
	T val;
	bool updated = false;

public:
	T const& getVal() const;
	T& getVal();

	bool isValid();
	void validate();
	void invalidate();

	UpdatedCache& operator=(const T&);
	UpdatedCache& operator=(T&&);

	UpdatedCache(T const& v);
	UpdatedCache(T&& v);

	NOCOPYMOVE(UpdatedCache);

private:
	UpdatedCache() = default;
};

template<class T>
inline T const& UpdatedCache<T>::getVal() const {
	return this->val;
}

template<class T>
inline T& UpdatedCache<T>::getVal() {
	return this->val;
}

template<class T>
inline bool UpdatedCache<T>::isValid() {
	return !this->updated;
}

template<class T>
inline void UpdatedCache<T>::validate() {
	this->updated = false;
}

template<class T>
inline void UpdatedCache<T>::invalidate() {
	this->updated = true;
}

template<class T>
inline UpdatedCache<T>& UpdatedCache<T>::operator=(const T& v) {
	this->val = v;
	this->updated = true;
	return *this;
}

template<class T>
inline UpdatedCache<T>& UpdatedCache<T>::operator=(T&& v) {
	this->val = std::move(v);
	this->updated = true;
	return *this;
}

template<class T>
inline UpdatedCache<T>::UpdatedCache(T const& v) {
	this->val = v;
}

template<class T>
inline UpdatedCache<T>::UpdatedCache(T&& v) {
	this->val = std::move(v);
}
