#include "GameObject.h"
//# start
void Everything::remove(SizeAlias i) {
	if (i == 0) return;
};
UniqueGameObject::UniqueGameObject(UniqueGameObject&& other) {
	this->proxy->remove(this->index);
	this->index = other.index;
	this->proxy = other.proxy;
	other.index = 0;
	other.proxy = nullptr;
};
UniqueGameObject& UniqueGameObject::operator=(UniqueGameObject&& other) {
	this->index = other.index;
	this->proxy = other.proxy;
	other.index = 0;
	other.proxy = nullptr;
	return *this;
};
//# end
