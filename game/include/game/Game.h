#pragma once

#include <vector>
#include <cstddef>
#include <bitset>
#include <array>

#include <templates/Everything.h>

using SizeAlias = size_t;

namespace game
{
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
		static const size_t SIZE = 100;

		struct counter
		{
			static inline std::atomic<size_t> t = 0;
		};

		template<class T>
		struct component_index : counter
		{
			static inline size_t val = t++;
		};

		struct indirection
		{
			std::bitset<SIZE> signature{ 0 };
			std::array<size_t, SIZE> index;
		};

		std::array<RawData, SIZE> data;
		std::vector<indirection> indirectionMap{};

		WeakObject make();

		template<class T>
		void add(SizeAlias i);

		template<class T>
		T& get(SizeAlias i);

		template<class T>
		bool has(SizeAlias i);
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
		return this->data[component_index<T>::val].get<T>(this->indirectionMap[i].index[component_index<T>::val]);
	}
	template<class T>
	inline bool EverythingS::has(SizeAlias i) {
		return this->indirectionMap[i].signature.test(component_index<T>::val);
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
}
