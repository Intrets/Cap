#include "Game.h"

namespace game
{
	UniqueObject::~UniqueObject() {
		this->proxy->remove(this->index);
	}

	UniqueObject::UniqueObject(WeakObject&& other) {
		this->index = other.index;
		this->proxy = other.proxy;
	}

	UniqueObject::UniqueObject(UniqueObject&& other) {
		this->index = other.index;
		this->proxy = other.proxy;
		other.index = 0;
		other.proxy = nullptr;
	}

	UniqueObject& UniqueObject::operator=(UniqueObject&& other) {
		if (this->proxy != nullptr) {
			this->proxy->remove(this->index);
		}

		this->index = other.index;
		this->proxy = other.proxy;

		return *this;
	}

	void game::Everything::subscribe(ManagedObject* obj, SizeAlias i) {
		assert(i != 0);
		if (i == 0) {
			return;
		}
		this->managedObjects.insert({ i, obj });
	}

	void game::Everything::unsubscribe(ManagedObject* obj, SizeAlias i) {
		assert(i != 0);
		if (i == 0) {
			return;
		}

		auto range = this->managedObjects.equal_range(i);

		auto it = range.first;
		auto end = range.second;

		for (; it != end;) {
			if (it->second == obj) {
				this->managedObjects.erase(it);
				return;
			}
		}
	}

	void Everything::invalidateManagedObjects(SizeAlias i) {
		auto range = this->managedObjects.equal_range(i);

		auto it = range.first;
		auto end = range.second;

		for (; it != end; ++it) {
			it->second->invalidate();
		}

		this->managedObjects.erase(range.first, end);
	}

	void ManagedObject::set(WeakObject const& other) {
		this->index = other.index;
		this->proxy = other.proxy;

		this->proxy->subscribe(this, this->index);
	}

	void ManagedObject::set(UniqueObject const& other) {
		this->index = other.index;
		this->proxy = other.proxy;

		this->proxy->subscribe(this, this->index);
	}

	ManagedObject::ManagedObject(WeakObject&& other) {
		this->index = other.index;
		this->proxy = other.proxy;

		this->proxy->subscribe(this, this->index);
	}

	ManagedObject::~ManagedObject() {
		if (this->proxy != nullptr) {
			this->proxy->unsubscribe(this, this->index);
		}
	}

	void ManagedObject::invalidate() {
		this->index = 0;
		this->proxy = nullptr;
	}

	void WeakObject::deleteObject() {
		if (this->isNotNull()) {
			this->proxy->remove(this->index);
		}
	}

	bool WeakObject::isNull() const {
		return (this->index == 0) || (this->proxy == nullptr);
	}

	bool WeakObject::isNotNull() const {
		return (this->index != 0) && (this->proxy != nullptr);
	}

	WeakObject Everything::make() {
		if (!this->freeIndirections.empty()) {
			size_t i = this->freeIndirections.back();
			this->freeIndirections.pop_back();

			return { i, this };
		}
		else {
			this->signatures.push_back(0);
			for (size_t type = 0; type < this->getTypeCount(); type++) {
				this->dataIndices[type].push_back(0);
			}
			return { this->signatures.size() - 1, this };
		}
	}

	UniqueObject Everything::makeUnique() {
		return this->make();
	}
}
