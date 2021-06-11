#include "Game.h"

namespace game
{
	UniqueObject::~UniqueObject() {
		this->proxy->remove(this->index);
	}

	UniqueObject::operator WeakObject() const {
		return *this;
	}

	UniqueObject::UniqueObject(WeakObject&& other) {
		this->index = other.index;
		this->proxy = other.proxy;
	}

	UniqueObject::UniqueObject(UniqueObject&& other) noexcept {
		this->index = other.index;
		this->proxy = other.proxy;
		other.index = 0;
		other.proxy = nullptr;
	}

	UniqueObject& UniqueObject::operator=(UniqueObject&& other) noexcept {
		if (this->proxy != nullptr) {
			this->proxy->remove(this->index);
		}

		this->index = other.index;
		this->proxy = other.proxy;

		return *this;
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

	bool WeakObject::has(SizeAlias i) {
		assert(this->isNotNull());
		return this->proxy->has(this->index, i);
	}

	WeakObject Everything::make() {
		if (!this->freeIndirections.empty()) {
			size_t i = this->freeIndirections.back();
			this->freeIndirections.pop_back();

			this->validIndices[i] = 1;

			return { i, this };
		}
		else {
			this->signatures.push_back(0);
			for (size_t type = 0; type < this->getTypeCount(); type++) {
				this->dataIndices[type].push_back(0);
			}

			this->qualifiers.push_back(this->getNextQualifier());
			this->validIndices.push_back(1);

			return { this->signatures.size() - 1, this };
		}
	}

	UniqueObject Everything::makeUnique() {
		return this->make();
	}

	std::optional<WeakObject> Everything::maybeGetFromIndex(SizeAlias index) {
		if (this->isValidIndex(index)) {
			return this->getFromIndex(index);
		}
		else {
			return std::nullopt;
		}
	}

	WeakObject Everything::getFromIndex(SizeAlias index) {
		assert(index > 0);
		assert(this->isValidIndex(index));
		return { index, this };
	}

	bool Everything::isValidIndex(SizeAlias index) {
		return (index > 0) && this->validIndices[index];
	}

	Qualifier Everything::getNextQualifier() {
		return this->qualifier++;
	}

	bool Everything::isQualified(SizeAlias i, Qualifier q) const {
		assert(i != 0);
		return this->qualifiers[i] == q;
	}

	Qualifier Everything::getQualifier(SizeAlias i) const {
		assert(i != 0);
		return this->qualifiers[i];
	}

	void QualifiedObject::set(WeakObject obj) {
		assert(obj.isNotNull());

		this->object = obj;
		this->qualifier = this->object.proxy->getQualifier(this->object.index);
	}

	bool QualifiedObject::isQualified() const {
		return this->object.isNotNull() && this->object.proxy->isQualified(this->object.index, this->qualifier);
	}

	WeakObject* QualifiedObject::operator->() {
		assert(this->isQualified());

		return &this->object;
	}
	QualifiedObject& QualifiedObject::operator=(WeakObject const& other) noexcept {
		this->set(other);

		return *this;
	}

	QualifiedObject::QualifiedObject(WeakObject const& other) noexcept {
		this->set(other);
	}
}
