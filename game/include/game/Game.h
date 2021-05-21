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
		inline T& get(SizeAlias i);

		template<class T>
		inline SizeAlias add();
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
		};

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

			static const inline SignatureType val = fillSignature();
		};

		struct indirection
		{
			EverythingS* proxy;
			SignatureType signature{ 0 };
			std::array<size_t, SIZE> index{};

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

		std::array<RawData, SIZE> data;
		std::vector<indirection> indirectionMap{};

		WeakObject make();

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
			//EverythingS::indirection r;

			//size_t end = e.gets<M>().index;
			//for (size_t i = 0; i < end; i++) {
			//	auto match = MatchS<M, Ms...>{r};
			//	//f(MatchS<M, Ms...>{r});
			//	te::Loop::run<EverythingS, F, L, MatchS<M, Ms...>&>(e, f, match);
			//}

			//auto sig = EverythingS::group_signature<M, Ms...>::val;
			//size_t end = e.gets<M>().index;
			//for (size_t i = 0; i < end; i++) {
			//	//auto obj = e.gets<M>().get<M>(i);
			//	//auto index = e.gets<M>().get<M>(i).index;
			//	auto h = e.indirectionMap[1];
			//	//if (h.contains(sig)) {
			//		te::Loop::run<EverythingS, F, L, MatchS<M, Ms...>, Args...>(e, f, MatchS<M, Ms...>{h}, args...);
			//	//}
			//}

			size_t end = e.gets<M>().index;
			auto& g = e.data[EverythingS::component_index<GamePosition>::val];
			for (SizeAlias i = 0; i < end; i++) {
				auto& el = g.get<GamePosition>(i);

				if (e.indirectionMap[el.index].contains(EverythingS::group_signature<GamePosition, GraphicsTile>::val)) {
					//f(MatchS<M, Ms...>{ e.indirectionMap[el.index]});
					te::Loop::run<EverythingS, F, L, MatchS<M, Ms...>, Args...>(e, f, MatchS<M, Ms...>{ e.indirectionMap[el.index] }, args...);
				}
			}


			//SizeAlias end = e.data[EverythingS::component_index<GamePosition>::val].index;
			//for (SizeAlias i = 0; i < end; i++) {
			//	auto& ee = e.data[EverythingS::component_index<GamePosition>::val].get<GamePosition>(i);

			//	if (e.indirectionMap[ee.index].contains(EverythingS::group_signature<GamePosition, GraphicsTile>::val)) {
			//		count++;
			//		//f(MatchS<GamePosition, GraphicsTile>{e.indirectionMap[ee.index]});
			//		//renderInfo.tileRenderInfo.addBlitInfo(
			//		//	glm::vec4(e.pos, 1, 1),
			//		//	0,
			//		//	this->everything2.get<GraphicsTile>(e.index).blockID
			//		//);
			//	}
			//}
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
		return this->proxy->gets<T>().get<T>(this->index[component_index<T>::val]);
	}
}
