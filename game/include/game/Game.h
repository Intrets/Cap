#pragma once

#include <vector>
#include <cstddef>
#include <bitset>
#include <array>

#include <templates/Everything.h>
#include <misc/Misc.h>

using SizeAlias = size_t;

namespace game
{
	static const size_t SIZE = 100;
	using SignatureType = std::bitset<SIZE>;

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

		SizeAlias size = 0;
		SizeAlias index = 0;
		std::vector<std::byte> data{};

		template<class T>
		T& get(SizeAlias i);

		template<class T>
		SizeAlias add();
	};

	struct EverythingS;

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
			static inline size_t getVal() {
				return val;
			};
		};

		template<class M, class... Ms>
		struct group_signature
		{
			static inline SignatureType fillSignature() {
				SignatureType res{};
				//if constexpr (sizeof...(Ms) == 0) {
				//	res.set(EverythingS::component_index<M>.getVal());
				//}
				//else {
				//	for (auto s : { EverythingS::component_index<M>.getVal(), EverythingS::component_index<Ms>.getVal()... }) {
				//		res.set(s);
				//	}
				//}
				return res;
			};

			static inline SignatureType val = fillSignature();
		};

		struct indirection
		{
			EverythingS* proxy;
			SignatureType signature{ 0 };
			std::array<size_t, SIZE> index{};

			template<class T>
			T& get();

			bool contains(SignatureType& sig);

			indirection() = default;
			indirection(EverythingS* p) : proxy(p) {};
			~indirection() = default;
			DEFAULTCOPYMOVE(indirection);
		};

		std::array<RawData, SIZE> data;
		std::vector<indirection> indirectionMap{};

		WeakObject make();

		template<class T>
		void add(SizeAlias i);

		template<class T>
		T& get(SizeAlias i);

		template<class T>
		RawData& gets();

		template<class T>
		bool has(SizeAlias i);

		template<class F>
		void run(F f);
	};

	template<class M, class... Ms>
	struct MatchS
	{
		EverythingS::indirection proxy;

		template<class T>
		inline T& get() {
			static_assert(te::Contains<T, te::List<M, Ms...>>::val());
			return proxy.get<T>();
		};

		template<class F, class L, class... Args>
		static inline void run(EverythingS& e, F f, Args... args) {
			size_t end = e.gets<M>().index;
			for (size_t i = 0; i < end; i++) {
				auto h = e.indirectionMap[e.gets<M>().get<M>(i).index];
				auto sig = EverythingS::group_signature<M, Ms...>::val;
				if (h.contains(sig)) {
					te::Loop::run<EverythingS, F, L, MatchS<M, Ms...>, Args...>(e, f, MatchS<M, Ms...>{h}, args...);
				}
			}
		};
	};


	template<class T>
	inline T& RawData::get(SizeAlias i) {
		return *reinterpret_cast<T*>(&this->data[aligned_sizeof<T>::get() * i]);
	}

	template<class T>
	inline SizeAlias RawData::add() {
		if (this->size == 0) {
			this->size = 16;
			this->data.resize(this->size * aligned_sizeof<T>::get());
		}
		else if (this->index >= this->size) {
			this->size *= 2;
			this->data.resize(this->size * aligned_sizeof<T>::get());
		}

		auto& obj = this->get<T>(this->index);

		obj = T{};
		obj.index = this->index;

		return this->index++;
	}
	template<class T>
	inline void EverythingS::add(SizeAlias i) {
		this->indirectionMap[i].index[component_index<T>::val] = this->data[component_index<T>::val].add<T>();
		this->indirectionMap[i].signature.set(component_index<T>::val);
		this->get<T>(i).index = i;
	}
	template<class T>
	inline T& EverythingS::get(SizeAlias i) {
		return this->indirectionMap[i].get<T>();
		//return this->data[component_index<T>::val].get<T>(this->indirectionMap[i].index[component_index<T>::val]);
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
		te::Loop::run(*this, te::wrap_in_std_fun(f));
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
		return this->proxy->gets<T>().get<T>(this->index[component_index<T>::val]);
	}
}
