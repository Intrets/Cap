#pragma once

#include <vector>
#include <cstddef>
#include <bitset>
#include <array>
#include <utility>
#include <stack>
#include <iostream>
#include <numeric>
#include <optional>

#include <tepp/tepp.h>
#include <misc/Misc.h>

using SizeAlias = size_t;

namespace game
{
	static const size_t SIZE = 64;
	using SignatureType = std::bitset<SIZE>;

	struct Everything;

	struct RawData
	{
		template<class T>
		struct aligned_sizeof
		{
			static constexpr size_t get() {
				const auto width = 8;
				constexpr size_t size = sizeof(T);

				if constexpr (sizeof(T) == 0) {
					return 0;
				}
				else {
					return size + (width - 1) - (size - 1) % width;
				}
			}
		};

		Everything& parent;
		SizeAlias reservedObjects = 0;
		SizeAlias index = 0;
		SizeAlias objectSize = 0;
		std::vector<std::byte> data{};
		std::vector<SizeAlias> indices{};

		RawData() = delete;
		RawData(Everything& p) : parent(p) {};

		DEFAULTCOPYMOVE(RawData);

		template<class T>
		inline void remove(SizeAlias i);

		// returns the index of the component owning the object that was moved
		// or optionally nothing if the last element was removed (no moving needed)
		inline std::optional<SizeAlias> remove_untyped(SizeAlias i);

		template<class T>
		inline T& get(SizeAlias i);

		inline SizeAlias getIndex(SizeAlias i);

		template<class T, class... Args>
		inline std::pair<SizeAlias, T*> add(SizeAlias i, Args&&... args);
	};

	struct WeakObject
	{
		SizeAlias index{ 0 };
		Everything* proxy{ nullptr };

		template<class T>
		inline T& get();

		template<class T, class... Args>
		inline T& add(Args&&... args);

		template<class T>
		inline bool has();
	};

	struct Everything
	{
		struct counter
		{
			static inline size_t t = 0;
		};

		template<class T>
		struct component_index : counter
		{
			static inline size_t getVal() {
				assert(t < SIZE);
				return t++;
			};

			static inline size_t val = getVal();
		};

		template<class T>
		static inline const size_t component_index_v = component_index<T>::val;

		template<class M, class... Ms>
		struct group_signature
		{
			static const inline SignatureType fillSignature() {
				SignatureType res{};
				if constexpr (sizeof...(Ms) == 0) {
					res.set(Everything::component_index<M>::val);
				}
				else {
					for (auto s : { Everything::component_index<M>::val, Everything::component_index<Ms>::val... }) {
						res.set(s);
					}
				}
				return res;
			};

			static const inline SignatureType value = fillSignature();
		};

		template<class... Ms>
		static inline const SignatureType group_signature_v = group_signature<Ms...>::value;

		std::vector<RawData> data{ SIZE, { *this } };
		std::vector<size_t> freeIndirections{};

		std::vector<SignatureType> signatures{};
		std::array<std::vector<SizeAlias>, SIZE> dataIndices;

		inline WeakObject make();

		inline void remove(SizeAlias i);

		template<class T, class... Args>
		inline T& add(SizeAlias i, Args&&... args);

		template<class T>
		inline T& get(SizeAlias i);

		template<class T>
		inline RawData& gets();

		inline RawData& gets(size_t type);

		template<class... Ts>
		inline bool has(SizeAlias i);

		inline bool has(SizeAlias i, size_t type);

		template<class F>
		inline void run(F f);

		template<class... Ms>
		size_t selectPivot();

		Everything() = default;
		~Everything() = default;

		NOCOPYMOVE(Everything);
	};

	template<class M, class... Ms>
	struct Match
	{
		WeakObject obj;

		template<class T>
		inline T& get() {
			static_assert(te::contains_v<te::list<M, Ms...>, T>);
			return this->obj.get<T>();
		};

		template<class F, class L, class... Args>
		static inline void run(Everything& e, F f, Args... args) {
			size_t pivot = e.selectPivot<M, Ms...>();

			auto& g = e.gets(pivot);
			const size_t end = g.index;

			if constexpr (sizeof...(Ms) == 0) {
				for (SizeAlias i = 0; i < end; i++) {
					auto index = g.getIndex(i);
					te::Loop::run<Everything, F, L, Match<M, Ms...>, Args...>(e, f, Match<M, Ms...>{ { index, & e } }, args...);
				}
			}
			else {
				for (SizeAlias i = 0; i < end; i++) {
					auto index = g.getIndex(i);

					if (e.has<Ms...>(index)) {
						te::Loop::run<Everything, F, L, Match<M, Ms...>, Args...>(e, f, Match<M, Ms...>{ { index, & e } }, args...);
					}
				}
			}
		};
	};


	template<class T>
	inline void RawData::remove(SizeAlias i) {
		assert(this->objectSize == 0 || (this->objectSize == aligned_sizeof<T>::get()));
		this->remove_untyped(i);
	}

	inline std::optional<SizeAlias> RawData::remove_untyped(SizeAlias i) {
		assert(i < this->index);

		size_t targetOffset = i * this->objectSize;
		size_t sourceOffset = --this->index * this->objectSize;

		if (targetOffset == sourceOffset) {
			this->indices.pop_back();
			return std::nullopt;
		}
		else {
			std::memcpy(&this->data[targetOffset], &this->data[sourceOffset], this->objectSize);
			auto changed = this->indices.back();
			this->indices.pop_back();

			this->indices[i] = changed;
			return changed;
		}
	}

	template<class T>
	inline T& RawData::get(SizeAlias i) {
		assert(this->objectSize == 0 || (this->objectSize == aligned_sizeof<T>::get()));
		return *reinterpret_cast<T*>(&this->data[aligned_sizeof<T>::get() * i]);
	}

	inline SizeAlias RawData::getIndex(SizeAlias i) {
		return this->indices[i];
	}

	template<class T, class... Args>
	inline std::pair<SizeAlias, T*> RawData::add(SizeAlias i, Args&&... args) {
		assert(this->objectSize == 0 || (this->objectSize == aligned_sizeof<T>::get()));
		this->objectSize = aligned_sizeof<T>::get();

		if (this->reservedObjects == 0) {
			this->reservedObjects = 16;
			this->index = 1;
			this->indices.push_back(0);
			this->data.resize(this->reservedObjects * aligned_sizeof<T>::get());
		}
		else if (this->index >= this->reservedObjects) {
			this->reservedObjects *= 2;
			this->data.resize(this->reservedObjects * aligned_sizeof<T>::get());
		}
		this->indices.push_back(i);

		auto& obj = this->get<T>(this->index);

		obj = T{ std::forward<Args>(args)... };

		return { this->index++, &obj };
	}

	inline void Everything::remove(SizeAlias i) {
		for (size_t type = 0; type < SIZE; type++) {
			if (this->has(i, type)) {
				auto maybeChanged = this->data[type].remove_untyped(this->dataIndices[type][i]);
				if (maybeChanged.has_value()) {
					auto changed = maybeChanged.value();
					this->dataIndices[type][changed] = this->dataIndices[type][i];
					this->dataIndices[type][i] = 0;
				}
			}
		}

		this->signatures[i].reset();

		this->freeIndirections.push_back(i);
	}

	inline RawData& Everything::gets(size_t type) {
		return this->data[type];
	}

	inline bool Everything::has(SizeAlias i, size_t type) {
		return this->dataIndices[type][i] != 0;
	}

	template<class T, class... Args>
	inline T& Everything::add(SizeAlias i, Args&&... args) {
		auto [index, ptr] = this->data[component_index_v<T>].add<T>(i, std::forward<Args>(args)...);
		this->dataIndices[component_index_v<T>][i] = index;
		this->signatures[i].set(component_index_v<T>);
		return *ptr;
	}

	template<class T>
	inline T& Everything::get(SizeAlias i) {
		return this->data[component_index_v<T>].get<T>(this->dataIndices[component_index_v<T>][i]);
	}

	template<class T>
	inline RawData& Everything::gets() {
		return this->gets(component_index_v<T>);
	}

	template<class... Ts>
	inline bool Everything::has(SizeAlias i) {
		if constexpr (sizeof...(Ts) == 1) {
			return this->has(i, component_index_v<Ts...>);
		}
		else {
			auto const sig = group_signature_v<Ts...>;
			return (this->signatures[i] & sig) == sig;
		}
	}

	template<class F>
	inline void Everything::run(F f) {
		te::Loop::run(*this, f);
	}

	template<class... Ms>
	inline size_t Everything::selectPivot() {
		size_t pivot = 0;
		size_t smallest = std::numeric_limits<size_t>::max();
		for (auto s : { Everything::component_index<Ms>::val... }) {
			size_t typeSize = this->data[s].index;

			if (typeSize < smallest) {
				smallest = typeSize;
				pivot = s;
			}
		}
		return pivot;
	}

	template<class T>
	inline T& WeakObject::get() {
		return this->proxy->get<T>(this->index);
	}

	template<class T, class... Args>
	inline T& WeakObject::add(Args&&... args) {
		return this->proxy->add<T>(this->index, std::forward<Args>(args)...);
	}

	template<class T>
	inline bool WeakObject::has() {
		return this->proxy->has<T>(this->index);
	}

	inline WeakObject Everything::make() {
		if (!this->freeIndirections.empty()) {
			size_t i = this->freeIndirections.back();
			this->freeIndirections.pop_back();

			return { i, this };
		}
		else {
			this->signatures.push_back(0);
			for (size_t type = 0; type < SIZE; type++) {
				this->dataIndices[type].push_back(0);
			}
			return { this->signatures.size() - 1, this };
		}
	}
}
