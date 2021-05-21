#pragma once

#include <vector>
#include <cstddef>
#include <bitset>
#include <array>
#include <utility>

#include <tepp/tepp.h>
#include <misc/Misc.h>

using SizeAlias = size_t;

//#define POINTER_INDIRECTION
//#define GAMEOBJECT_POINTER_CACHE

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
		std::vector<std::byte> data{};

		RawData() = delete;
		RawData(Everything& p) : parent(p) {};

		DEFAULTCOPYMOVE(RawData);

		template<class T>
		inline T& get(SizeAlias i);

		template<class T>
		inline std::pair<SizeAlias, T*> add(SizeAlias i);

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

		template<class T>
		inline void add();

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
				return this->index[component_index_v<T>] != 0;
			};

			indirection() = default;
			indirection(Everything* p) : proxy(p) {};
			~indirection() = default;
			DEFAULTCOPYMOVE(indirection);
		};

		std::vector<RawData> data{ SIZE, {*this} };

#ifdef GAMEOBJECT_POINTER_CACHE
		std::vector<indirection> indirectionMap{ 10'000'000 };
#else
		std::vector<indirection> indirectionMap{};
#endif // GAMEOBJECT_POINTER_CACHE


		inline WeakObject make();

		template<class T>
		inline void add(SizeAlias i);

		template<class T>
		inline T& get(SizeAlias i);

		template<class T>
		inline RawData& gets();

		template<class T>
		inline bool has(SizeAlias i);

		template<class F>
		inline void run(F f);
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
	inline T& RawData::get(SizeAlias i) {
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


	template<class T>
	inline std::pair<SizeAlias, T*> RawData::add(SizeAlias i) {
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

		obj = T{};
		obj.index = i;

		return { this->index++, &obj };
	}
	template<class T>
	inline void Everything::add(SizeAlias i) {
		auto [index, ptr] = this->data[component_index_v<T>].add<T>(i);
		this->indirectionMap[i].index[component_index_v<T>] = index;
		this->indirectionMap[i].signature.set(component_index_v<T>);
#ifdef POINTER_INDIRECTION
		this->indirectionMap[i].ptrs[component_index_v<T>] = ptr;
#endif // POINTER_INDIRECTION

#ifdef GAMEOBJECT_POINTER_CACHE
		ptr->indirectionCache = &this->indirectionMap[i];
#endif // GAMEOBJECT_POINTER_CACHE
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
	template<class T>
	inline void WeakObject::add() {
		this->proxy->add<T>(this->index);
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
		WeakObject res{ this->indirectionMap.size(), this };

#ifdef GAMEOBJECT_POINTER_CACHE
		assert(this->indirectionMap.capacity() > this->indirectionMap.size());
#endif // GAMEOBJECT_POINTER_CACHE

		this->indirectionMap.push_back({ this });
		return res;
	}
		}
