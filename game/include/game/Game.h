#pragma once

#include <vector>
#include <cstddef>
#include <bitset>
#include <array>
#include <utility>
#include <stack>
#include <iostream>
#include <numeric>

#include <tepp/tepp.h>
#include <misc/Misc.h>

using SizeAlias = size_t;

//#define POINTER_INDIRECTION
//#define GAMEOBJECT_POINTER_CACHE

#ifdef GAMEOBJECT_POINTER_CACHE
#define GAMEOBJECT SizeAlias index = 0; game::Everything::indirection* indirectionCache;
#else
#define GAMEOBJECT SizeAlias index = 0;
#endif // GAMEOBJECT_POINTER_CACHE

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
		SizeAlias size = 0;
		SizeAlias index = 0;
		SizeAlias objectSize = 0;
		std::vector<std::byte> data{};

		RawData() = delete;
		RawData(Everything& p) : parent(p) {};

		DEFAULTCOPYMOVE(RawData);

		template<class T>
		inline void remove(SizeAlias i);

		// pair.first: the first sizeof(SizeAlias) bytes of the object stored at i.
		// index (for indirectionMap) of the object to update
		inline void* remove_untyped(SizeAlias i);

		template<class T>
		inline T& get(SizeAlias i);

		template<class T, class... Args>
		inline std::pair<SizeAlias, T*> add(SizeAlias i, Args&&... args);

#ifdef POINTER_INDIRECTION
		template<class T>
		void refreshPointers();
#endif // POINTER_INDIRECTION
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
					res.set(Everything::component_index<M>.getVal());
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

		struct indirection
		{
			Everything* proxy;
			SignatureType signature{ 0 };
			std::array<size_t, SIZE> index{};

#ifdef POINTER_INDIRECTION
			std::array<void*, SIZE> ptrs{};
#endif // POINTER_INDIRECTION

			template<class T>
			T& get();

			inline bool contains(SignatureType const& sig) const {
				return (this->signature & sig) == sig;
			};

			template<class T>
			inline bool has() {
				return this->signature.test(component_index_v<T>);
			};

			indirection() = default;
			indirection(Everything* p) : proxy(p) {};
			~indirection() = default;
			DEFAULTCOPYMOVE(indirection);
		};

		std::vector<RawData> data{ SIZE, { *this } };
		std::vector<indirection> indirectionMap{};
		std::vector<size_t> freeIndirections{};

		inline WeakObject make();

		inline void remove(SizeAlias i);

		template<class T, class... Args>
		inline T& add(SizeAlias i, Args&&... args);

		template<class T>
		inline T& get(SizeAlias i);

		template<class T>
		inline RawData& gets();

		template<class T>
		inline bool has(SizeAlias i);

		template<class F>
		inline void run(F f);

		Everything();
		~Everything() = default;

		NOCOPYMOVE(Everything);
	};

	template<class M, class... Ms>
	struct Match
	{
		Everything::indirection& proxy;

		template<class T>
		inline T& get() {
			static_assert(te::contains_v<te::list<M, Ms...>, T>);
			return proxy.get<T>();
		};

		template<class F, class L, class... Args>
		static inline void run(Everything& e, F f, Args... args) {
			size_t end = e.gets<M>().index;
			auto& g = e.data[Everything::component_index<M>::val];

			if constexpr (sizeof...(Ms) == 0) {
				for (SizeAlias i = 0; i < end; i++) {
					auto& indirection = e.indirectionMap[g.get<M>(i).index];
					te::Loop::run<Everything, F, L, Match<M, Ms...>, Args...>(e, f, Match<M, Ms...>{ indirection }, args...);
				}
			}
			else if constexpr (sizeof...(Ms) == 1) {
				for (SizeAlias i = 0; i < end; i++) {
					auto& indirection = e.indirectionMap[g.get<M>(i).index];

					if (indirection.has<Ms...>()) {
						te::Loop::run<Everything, F, L, Match<M, Ms...>, Args...>(e, f, Match<M, Ms...>{ indirection }, args...);
					}
				}
			}
			else {
				for (SizeAlias i = 0; i < end; i++) {
					auto& indirection = e.indirectionMap[g.get<M>(i).index];

					if (indirection.contains(Everything::group_signature_v<M, Ms...>)) {
						te::Loop::run<Everything, F, L, Match<M, Ms...>, Args...>(e, f, Match<M, Ms...>{ indirection }, args...);
					}
				}
			}
		};
	};


	template<class T>
	inline void RawData::remove(SizeAlias i) {
		assert(this->objectSize == 0 || (this->objectSize == aligned_sizeof<T>::get()));
		this->get<T>(i) = this->get<T>(--this->index);
	}

	inline void* RawData::remove_untyped(SizeAlias i) {
		size_t targetOffset = i * this->objectSize;
		size_t sourceOffset = --this->index * this->objectSize;
		std::memcpy(&this->data[targetOffset], &this->data[sourceOffset], this->objectSize);

		return &this->data[targetOffset];
	}

	template<class T>
	inline T& RawData::get(SizeAlias i) {
		assert(this->objectSize == 0 || (this->objectSize == aligned_sizeof<T>::get()));
		return *reinterpret_cast<T*>(&this->data[aligned_sizeof<T>::get() * i]);
	}

#ifdef POINTER_INDIRECTION
	template<class T>
	inline void RawData::refreshPointers() {
		for (size_t i = 0; i < this->index; i++) {
			auto& p = this->get<T>(i);
			this->parent.indirectionMap[p.index].ptrs[Everything::component_index_v<T>] = &p;
		}
	}
#endif // POINTER_INDIRECTION


	template<class T, class... Args>
	inline std::pair<SizeAlias, T*> RawData::add(SizeAlias i, Args&&... args) {
		assert(this->objectSize == 0 || (this->objectSize == aligned_sizeof<T>::get()));
		this->objectSize = aligned_sizeof<T>::get();

		if (this->size == 0) {
			this->size = 16;
			this->data.resize(this->size * aligned_sizeof<T>::get());
#ifdef POINTER_INDIRECTION
			this->refreshPointers<T>();
#endif // POINTER_INDIRECTION
		}
		else if (this->index >= this->size) {
			this->size *= 2;
			this->data.resize(this->size * aligned_sizeof<T>::get());
#ifdef POINTER_INDIRECTION
			this->refreshPointers<T>();
#endif // POINTER_INDIRECTION
		}

		auto& obj = this->get<T>(this->index);

#ifdef GAMEOBJECT_POINTER_CACHE
		obj = T{ i, &this->parent.indirectionMap[i], std::forward<Args>(args)... };
#else
		obj = T{ i, std::forward<Args>(args)... };
#endif // GAMEOBJECT_POINTER_CACHE

		return { this->index++, &obj };
	}

	inline void Everything::remove(SizeAlias i) {
		auto& indirection = this->indirectionMap[i];
		for (size_t type = 0; type < counter::t; type++) {
			if (indirection.signature.test(type)) {
				auto ptr = this->data[type].remove_untyped(indirection.index[type]);
				auto index = *reinterpret_cast<SizeAlias*>(ptr);

				this->indirectionMap[index].index[type] = index;

#ifdef POINTER_INDIRECTION
				this->indirectionMap[index].ptrs[type] = ptr;
#endif // POINTER_INDIRECTION
			}
		}

		this->freeIndirections.push_back(i);
	}

	inline Everything::Everything() {
#ifdef GAMEOBJECT_POINTER_CACHE
		this->indirectionMap.resize(100);
		this->freeIndirections.resize(100);
		std::iota(this->freeIndirections.begin(), this->freeIndirections.end(), 0);
#endif // GAMEOBJECT_POINTER_CACHE
	}

	template<class T, class... Args>
	inline T& Everything::add(SizeAlias i, Args&&... args) {
		auto [index, ptr] = this->data[component_index_v<T>].add<T>(i, std::forward<Args>(args)...);
		this->indirectionMap[i].index[component_index_v<T>] = index;
		this->indirectionMap[i].signature.set(component_index_v<T>);
#ifdef POINTER_INDIRECTION
		this->indirectionMap[i].ptrs[component_index_v<T>] = ptr;
#endif // POINTER_INDIRECTION

#ifdef GAMEOBJECT_POINTER_CACHE
		assert(ptr->indirectionCache == &this->indirectionMap[i]);
		ptr->indirectionCache = &this->indirectionMap[i];
#endif // GAMEOBJECT_POINTER_CACHE
		return *ptr;
	}

	template<class T>
	inline T& Everything::get(SizeAlias i) {
		return this->indirectionMap[i].get<T>();
	}

	template<class T>
	inline RawData& Everything::gets() {
		return this->data[component_index<T>::val];
	}

	template<class T>
	inline bool Everything::has(SizeAlias i) {
		return this->indirectionMap[i].signature.test(component_index<T>::val);
	}

	template<class F>
	inline void Everything::run(F f) {
		te::Loop::run(*this, f);
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

	template<class T>
	inline T& Everything::indirection::get() {
#ifdef POINTER_INDIRECTION
		return *reinterpret_cast<T*>(this->ptrs[component_index_v<T>]);
#else
		return this->proxy->gets<T>().get<T>(this->index[component_index_v<T>]);
#endif // POINTER_INDIRECTION
	}

	inline WeakObject Everything::make() {
#ifdef GAMEOBJECT_POINTER_CACHE
		if (this->freeIndirections.empty()) {
			std::cerr << "fatal error: out of static limit of objects\n";
			exit(1);
		}
#endif // GAMEOBJECT_POINTER_CACHE


		if (!this->freeIndirections.empty()) {
			size_t i = this->freeIndirections.back();
			this->freeIndirections.pop_back();

			this->indirectionMap[i] = { this };

			return { i, this };
		}
		else {
			this->indirectionMap.push_back({ this });
			return { this->indirectionMap.size() - 1, this };
		}
	}
}
