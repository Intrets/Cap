#pragma once

#include <vector>
#include <cstddef>
#include <bitset>
#include <array>
#include <utility>

#include <tepp/tepp.h>
#include <misc/Misc.h>

using SizeAlias = size_t;

#define POINTER_INDIRECTION

namespace game
{
	static const size_t SIZE = 64;
	using SignatureType = std::bitset<SIZE>;

	struct EverythingS;

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

		EverythingS& parent;
		SizeAlias size = 0;
		SizeAlias index = 0;
		std::vector<std::byte> data{};

		RawData() = delete;
		RawData(EverythingS& p) : parent(p) {};

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
		EverythingS* proxy{ nullptr };

		template<class T>
		inline T& get();

		template<class T>
		inline void add();

		template<class T>
		inline bool has();
	};

	struct EverythingS
	{
		struct counter
		{
			static inline size_t t = 0;
		};

		template<class T>
		struct component_index : counter
		{
			static inline size_t val = t++;
		};

		template<class T>
		static inline const size_t component_index_v = component_index<T>::val;

		template<class M, class... Ms>
		struct group_signature
		{
			static const inline SignatureType fillSignature() {
				SignatureType res{};
				if constexpr (sizeof...(Ms) == 0) {
					res.set(EverythingS::component_index<M>.getVal());
				}
				else {
					for (auto s : { EverythingS::component_index<M>::val, EverythingS::component_index<Ms>::val... }) {
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
			EverythingS* proxy;
			SignatureType signature{ 0 };
			std::array<size_t, SIZE> index{};

#ifdef POINTER_INDIRECTION
			std::array<void*, SIZE> ptrs{};
#endif

			template<class T>
			T& get();

			inline bool contains(SignatureType const& sig) const {
				return (this->signature & sig) == sig;
			};

			indirection() = default;
			indirection(EverythingS* p) : proxy(p) {};
			~indirection() = default;
			DEFAULTCOPYMOVE(indirection);
		};

		std::vector<RawData> data{ SIZE, {*this} };
		std::vector<indirection> indirectionMap{};

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
	struct MatchS
	{
		EverythingS::indirection& proxy;

		template<class T>
		inline T& get() {
			static_assert(te::contains_v<te::list<M, Ms...>, T>);
			return proxy.get<T>();
		};

		template<class F, class L, class... Args>
		static inline void run(EverythingS& e, F f, Args... args) {
			size_t end = e.gets<M>().index;
			auto& g = e.data[EverythingS::component_index<M>::val];
			for (SizeAlias i = 0; i < end; i++) {
				auto& el = g.get<M>(i);

				if (e.indirectionMap[el.index].contains(EverythingS::group_signature_v<M, Ms...>)) {
					te::Loop::run<EverythingS, F, L, MatchS<M, Ms...>, Args...>(e, f, MatchS<M, Ms...>{ e.indirectionMap[el.index] }, args...);
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
			this->parent.indirectionMap[p.index].ptrs[EverythingS::component_index_v<T>] = &p;
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
	inline void EverythingS::add(SizeAlias i) {
		auto [index, ptr] = this->data[component_index_v<T>].add<T>(i);
		this->indirectionMap[i].index[component_index_v<T>] = index;
		this->indirectionMap[i].signature.set(component_index_v<T>);
#ifdef POINTER_INDIRECTION
		this->indirectionMap[i].ptrs[component_index_v<T>] = ptr;
#endif // POINTER_INDIRECTION
	}
	template<class T>
	inline T& EverythingS::get(SizeAlias i) {
		return this->indirectionMap[i].get<T>();
	}
	template<class T>
	inline RawData& EverythingS::gets() {
		return this->data[component_index<T>::val];
	}
	template<class T>
	inline bool EverythingS::has(SizeAlias i) {
		return this->indirectionMap[i].signature.test(component_index<T>::val);
	}
	template<class F>
	inline void EverythingS::run(F f) {
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
	inline T& EverythingS::indirection::get() {
#ifdef POINTER_INDIRECTION
		auto& r = *reinterpret_cast<T*>(this->ptrs[component_index_v<T>]);
		return r;
#else
		return this->proxy->gets<T>().get<T>(this->index[component_index_v<T>]);
#endif // POINTER_INDIRECTION
	}
	inline WeakObject EverythingS::make() {
		WeakObject res{ this->indirectionMap.size(), this };
		this->indirectionMap.push_back({ this });
		return res;
	}
}
