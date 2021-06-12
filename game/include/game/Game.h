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
#include <unordered_map>

#include <tepp/tepp.h>

#include <misc/Misc.h>

#include <mem/Index.h>

#include <serial/Serializer.h>

struct Component;

template<class T>
struct Identifier;

struct StructInformation
{
	std::string name{};
	int32_t index{};
	size_t width{};

	void(*objectDestructor)(void*) = nullptr;
	bool(*objectReader)(serial::Serializer& serializer, void*) = nullptr;
	bool(*objectWriter)(serial::Serializer& serializer, void*) = nullptr;
	bool(*objectPrinter)(serial::Serializer& serializer, void*) = nullptr;
};

struct StoredStructInformations
{
	inline static std::unordered_map<std::string, StructInformation> infos{};
};

template<>
struct serial::Serializable<StructInformation>
{
	static constexpr std::string_view typeName = "StructInformation";

	static bool run(Read, Serializer& serializer, StructInformation&& val) {
		std::string name;
		READ(name);
		if (name != "") {
			assert(StoredStructInformations::infos.contains(name));
			val = StoredStructInformations::infos[name];
		}
		return true;
	};

	static bool run(Write, Serializer& serializer, StructInformation&& val) {
		return serializer.write(val.name);
	}

	static bool run(Print, Serializer& serializer, StructInformation&& obj) {
		return serializer.runAll<Print>(
			ALL(name),
			ALL(index),
			ALL(width)
			);
	}
};

namespace game
{
	constexpr size_t SIZE = 64;
	using SignatureType = std::bitset<SIZE>;
	using Qualifier = size_t;

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

		StructInformation structInformation;

		size_t reservedObjects = 0;
		Index<RawData> index{ 0 };
		size_t objectSize = 0;
		std::vector<std::byte> data{};
		std::vector<Index<Everything>> indices{};

		std::vector<Index<RawData>> deletions{};

		struct DeletedInfo
		{
			Index<RawData> i;
			Index<Everything> changed;
		};

		std::vector<DeletedInfo> packDeletions();

		RawData() = default;
		~RawData();

		DEFAULTCOPYMOVE(RawData);

		template<class T>
		inline void remove(Index<RawData> i);

		inline void removeUntyped(Index<RawData> i);

		template<class T>
		inline T& get(Index<RawData> i);

		inline void* getUntyped(Index<RawData> i);

		inline Index<Everything> getIndex(Index<RawData> i) const;

		bool print(serial::Serializer& serializer, Index<RawData> i);

		template<class T, class... Args>
		inline std::pair<Index<RawData>, T*> add(Index<Everything> i, Args&&... args);

		inline Index<RawData> cloneUntyped(Index<RawData> i, Index<Everything> j);

		void increaseSize();
	};
}

template<>
struct serial::Serializable<game::RawData::DeletedInfo>
{
	inline const static std::string_view type_name = "RawData";

	ALL_DEF(game::RawData::DeletedInfo) {
		return serializer.runAll<Selector>(
			ALL(i),
			ALL(changed)
			);
	};
};

template<>
struct serial::Serializable<game::RawData>
{
	inline static const auto typeName = "RawData";

	static bool run(serial::Read, serial::Serializer& serializer, game::RawData&& rawData) {
		if (!serializer.readAll(
			rawData.structInformation,
			rawData.reservedObjects,
			rawData.index,
			rawData.objectSize,
			rawData.indices,
			rawData.deletions
		)) return false;

		rawData.data.resize(rawData.structInformation.width * rawData.index);

		for (Index<game::RawData> i{ 1 }; i < rawData.index; i++) {
			if (!rawData.structInformation.objectReader(serializer, rawData.getUntyped(i))) return false;
		}

		return true;
	};

	static bool run(Write, Serializer& serializer, game::RawData&& rawData) {
		if (!serializer.writeAll(
			rawData.structInformation,
			rawData.reservedObjects,
			rawData.index,
			rawData.objectSize,
			rawData.indices,
			rawData.deletions
		)) return false;

		for (Index<game::RawData> i{ 1 }; i < rawData.index; i++) {
			if (!rawData.structInformation.objectWriter(serializer, rawData.getUntyped(i))) return false;
		}

		return true;
	};

	PRINT_DEF(game::RawData) {
		return serializer.runAll<Print>(
			ALL(structInformation),
			ALL(reservedObjects),
			ALL(objectSize),
			ALL(data),
			ALL(indices),
			ALL(deletions)
			);
	}
};

namespace game
{
	struct WeakObject
	{
		Index<Everything> index{ 0 };
		Everything* proxy{ nullptr };

		void deleteObject();

		bool isNull() const;
		bool isNotNull() const;

		template<class T>
		inline void remove();

		template<class T>
		inline std::optional<T*> getMaybe();

		template<class T>
		inline T& get();

		template<class T, class... Args>
		inline T& add(Args&&... args);

		template<class T>
		inline bool has() const;

		bool has(Index<Component> i);
	};

	struct UniqueObject : WeakObject
	{
		UniqueObject(WeakObject&& other);

		UniqueObject& operator=(UniqueObject&& other) noexcept;
		UniqueObject(UniqueObject&& other) noexcept;

		UniqueObject() = default;
		~UniqueObject();

		NOCOPY(UniqueObject);
	};

	struct QualifiedObject
	{
		WeakObject object;

		Qualifier qualifier = 0;

		void set(WeakObject obj);

		bool isQualified() const;

		WeakObject* operator->();

		QualifiedObject& operator=(WeakObject const& other) noexcept;
		QualifiedObject(WeakObject const& other) noexcept;

		QualifiedObject() = default;
		~QualifiedObject() = default;

		DEFAULTCOPYMOVE(QualifiedObject);
	};

	struct Everything
	{
		struct component_counter
		{
			static inline int32_t t = 0;
		};

		template<class T>
		struct component_index : component_counter
		{
			static inline int32_t getVal() {
				assert(t < SIZE);
				return t++;
			};

			static inline int32_t val = getVal();
		};

		template<class T>
		static inline const int32_t component_index_v = component_index<T>::val;

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


		std::vector<RawData> data{ SIZE };
		std::vector<size_t> freeIndirections{};

		std::vector<Qualifier> qualifiers{ 0 };
		std::vector<int32_t> validIndices{};
		Qualifier qualifier = 1;

		std::vector<SignatureType> signatures{ 0 };
		std::array<std::vector<Index<RawData>>, SIZE> dataIndices;

		std::vector<Index<Everything>> removed{};

		WeakObject make();
		UniqueObject makeUnique();

		std::optional<WeakObject> maybeGetFromIndex(Index<Everything> index);
		WeakObject getFromIndex(Index<Everything> index);
		bool isValidIndex(Index<Everything> index);

		Qualifier getNextQualifier();
		bool isQualified(Index<Everything> i, Qualifier q) const;
		Qualifier getQualifier(Index<Everything> i) const;

		inline void remove(Index<Everything> i);

		void collectRemoved();

		template<class T>
		inline void removeComponent(Index<Everything> i);

		inline void removeComponent(Index<Everything> i, size_t type);

		template<class T, class... Args>
		inline T& add(Index<Everything> i, Args&&... args);

		template<class T>
		inline T& get(Index<Everything> i);

		template<class T>
		inline RawData& gets();

		inline RawData& gets(size_t type);

		bool print(serial::Serializer& serializer, Index<Everything> index, size_t type);

		template<class... Ts>
		inline bool has(Index<Everything> i) const;

		inline bool has(Index<Everything> i, Index<Component> type);

		template<class F>
		inline void run(F f);

		template<class F>
		inline void match(F f);

		template<class... Ms>
		size_t selectPivot();

		size_t getTypeCount();

		Everything();
		~Everything() = default;

		NOCOPY(Everything);
		DEFAULTMOVE(Everything);
	};
}

template<>
struct serial::Serializable<game::Everything>
{
	inline static const auto typeName = "Everything";

	ALL_DEF(game::Everything) {
		return serializer.runAll<Selector>(
			ALL(data),
			ALL(freeIndirections),
			ALL(qualifiers),
			ALL(qualifier),
			ALL(signatures),
			ALL(dataIndices),
			ALL(removed),
			ALL(validIndices)
			);
	};
};

template<>
struct serial::Serializable<game::WeakObject>
{
	inline static const auto typeName = "WeakObject";

	PRINT_DEF(game::WeakObject) {
		if (obj.isNull()) {
			return serializer.printString("Empty object");
		}
		else {
			auto end = obj.proxy->getTypeCount();

			for (Index<Component> i{ 0 }; i < end; i++) {
				if (obj.has(i)) {
					serializer.printIndentedString(obj.proxy->data[i].structInformation.name + " ");
					obj.proxy->print(serializer, obj.index, i);
				}
			}

			return true;
		}
	}
};

template<>
struct serial::Serializable<game::QualifiedObject>
{
	inline static const auto typeName = "QualifiedObject";

	PRINT_DEF(game::QualifiedObject) {
		if (!obj.isQualified()) {
			return serializer.printString("Unqualified object");
		}
		else {
			return serializer.print(obj.object);
		}
	}
};



namespace game
{
	template<class T>
	struct RegisterStruct
	{
		static bool reg() {
			StructInformation info;
			static_assert(serial::has_type_name<T>);
			info.name = serial::Serializable<T>::typeName;
			info.index = game::Everything::component_index_v<T>;
			info.width = RawData::aligned_sizeof<T>::get();
			info.objectDestructor = [](void* obj) {
				reinterpret_cast<T*>(obj)->~T();
			};

			info.objectReader = [](serial::Serializer& serializer, void* obj) {
				return serializer.read<T>(std::forward<T>(*reinterpret_cast<T*>(obj)));
			};

			info.objectWriter = [](serial::Serializer& serializer, void* obj) {
				return serializer.write<T>(std::forward<T>(*reinterpret_cast<T*>(obj)));
			};

			info.objectPrinter = [](serial::Serializer& serializer, void* obj) {
				return serializer.print<T>(std::forward<T>(*reinterpret_cast<T*>(obj)));
			};

			StoredStructInformations::infos[info.name] = info;

			return true;
		};
		inline static bool initialized = reg();
	};


	struct Loop
	{
		template<class E, class F>
		static inline void run(E& e, F f) {
			using A = te::reverse_t<te::arguments_list_t<F>>;
			Loop::run<E, F, A>(e, f);
		}

		template<class E, class F, class L, class... Args>
		static inline void run(E& e, F f, Args... args) {
			if constexpr (L::is_empty) {
				f(args...);
			}
			else {
				using head_stripped_ref = std::remove_reference_t<typename L::head>;
				head_stripped_ref::template run<F, typename L::tail, Args...>(e, f, args...);
			}
		}
	};

	template<class M, class... Ms>
	struct Match
	{
		WeakObject obj;

		template<class T>
		inline T& get() {
			static_assert(te::contains_v<te::list<M, Ms...>, T>);
			return this->obj.template get<T>();
		};

		template<class T>
		inline void remove() {
			static_assert(te::contains_v<te::list<M, Ms...>, T>);
			this->obj.template remove<T>();
		};

		template<class F, class L, class... Args>
		static inline void run(Everything& e, F f, Args... args) {
			size_t pivot = e.selectPivot<M, Ms...>();

			auto& g = e.gets(pivot);
			const size_t end = g.index;

			if constexpr (sizeof...(Ms) == 0) {
				for (Index<game::RawData> i{ 1 }; i < end; i++) {
					auto index = g.getIndex(i);
					Loop::run<Everything, F, L, Match<M, Ms...>, Args...>(e, f, Match<M, Ms...>{ { index, & e } }, args...);
				}
			}
			else {
				for (Index<game::RawData> i{ 1 }; i < end; i++) {
					auto index = g.getIndex(i);

					if (e.has<Ms...>(index)) {
						Loop::run<Everything, F, L, Match<M, Ms...>, Args...>(e, f, Match<M, Ms...>{ { index, & e } }, args...);
					}
				}
			}
		};
	};

	template<class L>
	struct MatchExpanded;

	template<class M, class... Ms>
	struct MatchExpanded<te::list<M, Ms... >>
	{
		template<class F>
		static inline void run(Everything& e, F f) {
			size_t pivot = e.selectPivot<M, Ms...>();

			auto& g = e.gets(pivot);
			const size_t end = g.index;

			if constexpr (sizeof...(Ms) == 0) {
				for (Index<game::RawData> i{ 1 }; i < end; i++) {
					f(g.get<M>(i));
				}
			}
			else {
				for (Index<game::RawData> i{ 1 }; i < end; i++) {
					auto index = g.getIndex(i);

					if (e.has<Ms...>(index)) {
						f(e.get<M>(index), e.get<Ms>(index)...);
					}
				}
			}
		};
	};

	template<class T>
	inline void RawData::remove(Index<RawData> i) {
		assert(i != 0);
		assert(i < this->index);
		assert(this->objectSize != 0);
		assert(this->objectSize == aligned_sizeof<T>::get());
		this->removeUntyped(i);
	}

	inline std::vector<RawData::DeletedInfo> RawData::packDeletions() {
		std::vector<RawData::DeletedInfo> res;

		for (auto i : this->deletions) {
			size_t targetOffset = i * this->objectSize;
			size_t sourceOffset = --this->index * this->objectSize;

			if (targetOffset == sourceOffset) {
				this->indices.pop_back();
				continue;
			}
			else {
				std::memcpy(&this->data[targetOffset], &this->data[sourceOffset], this->objectSize);
				auto changed = this->indices.back();
				this->indices.pop_back();

				this->indices[i] = changed;

				res.push_back({ i, changed });
			}
		}

		this->deletions.clear();

		return res;
	}

	inline RawData::~RawData() {
		for (Index<RawData> i{ 1 }; i < this->index; i++) {
			this->structInformation.objectDestructor(this->getUntyped(i));
		}
	}

	inline void RawData::removeUntyped(Index<RawData> i) {
		assert(i != 0);
		assert(i < this->index);

		size_t targetOffset = i * this->objectSize;

		this->structInformation.objectDestructor(&this->data[targetOffset]);

		this->deletions.push_back(i);
	}

	template<class T>
	inline T& RawData::get(Index<RawData> i) {
		assert(i != 0);
		assert(i < this->reservedObjects);
		return *reinterpret_cast<T*>(&this->data[aligned_sizeof<T>::get() * i]);
	}

	inline void* RawData::getUntyped(Index<RawData> i) {
		assert(i != 0);
		assert(i < this->reservedObjects);
		return static_cast<void*>(&this->data[this->objectSize * i]);
	}

	inline Index<Everything> RawData::getIndex(Index<RawData> i) const {
		assert(i != 0);
		assert(i < this->index);
		return this->indices[i];
	}

	inline bool RawData::print(serial::Serializer& serializer, Index<RawData> i) {
		return this->structInformation.objectPrinter(serializer, this->getUntyped(i));
	}

	inline Index<RawData> RawData::cloneUntyped(Index<RawData> i, Index<Everything> j) {
		assert(this->index > 1);
		assert(i > 0 && i <= this->index);
		assert(this->structInformation.clone != nullptr);
		assert(this->structInformation.width != 0);

		if (this->index >= this->reservedObjects) {
			this->increaseSize();
		}

		this->indices.push_back(j);
		this->structInformation.clone(this->getUntyped(i), this->getUntyped(this->index));

		return this->index++;
	}

	inline void RawData::increaseSize() {
		this->reservedObjects *= 2;
		this->data.resize(this->reservedObjects * this->structInformation.width);
	}

	template<class T, class... Args>
	inline std::pair<Index<RawData>, T*> RawData::add(Index<Everything> i, Args&&... args) {
		this->objectSize = aligned_sizeof<T>::get();

		if (this->reservedObjects == 0) {
			this->reservedObjects = 16;
			this->index.set(1);
			this->indices.push_back(Index<Everything>{ 0 });
			this->data.resize(this->reservedObjects * aligned_sizeof<T>::get());

			this->structInformation = StoredStructInformations::infos[std::string(serial::Serializable<T>::typeName)];

		}
		else if (this->index >= this->reservedObjects) {
			this->increaseSize();
		}

		assert(this->objectSize == aligned_sizeof<T>::get());
		assert(this->objectSize != 0);

		this->indices.push_back(i);

		auto& obj = this->get<T>(this->index);

		new (&obj) T{ std::forward<Args>(args)... };

		return { Index<RawData>{ this->index++ }, &obj };
	}

	inline void Everything::remove(Index<Everything> i) {
		if (i == 0) {
			return;
		}

		for (Index<Component> type{ 0 }; type < this->getTypeCount(); type++) {
			if (this->has(i, type)) {
				this->data[type].removeUntyped(this->dataIndices[type][i]);
			}
		}

		this->signatures[i].reset();

		this->qualifiers[i] = this->getNextQualifier();

		this->removed.push_back(i);
		this->validIndices[i] = 0;
	}

	inline void Everything::collectRemoved() {
		for (size_t type = 0; type < this->getTypeCount(); type++) {
			for (auto const& d : this->data[type].packDeletions()) {
				this->dataIndices[type][d.changed] = this->dataIndices[type][d.i];
				this->dataIndices[type][d.i].set(0);
			}
		}

		for (auto i : this->removed) {
			assert(this->signatures[i].none());
			for (size_t type = 0; type < this->getTypeCount(); type++) {
				assert(this->dataIndices[type][i] == 0);
			}
			this->freeIndirections.push_back(i);
		}

		this->removed.clear();
	}

	inline void Everything::removeComponent(Index<Everything> i, size_t type) {
		assert(this->signatures[i].test(type));
		this->data[type].removeUntyped(this->dataIndices[type][i]);
		this->signatures[i].reset(type);
	}

	inline RawData& Everything::gets(size_t type) {
		return this->data[type];
	}

	inline bool Everything::print(serial::Serializer& serializer, Index<Everything> index, size_t type) {
		return this->data[type].print(serializer, this->dataIndices[type][index]);
	}

	inline bool Everything::has(Index<Everything> i, Index<Component> type) {
		return this->signatures[i].test(type);
	}

	inline size_t Everything::getTypeCount() {
		return component_counter::t;
	}

	inline Everything::Everything() {
		for (size_t type = 0; type < SIZE; type++) {
			this->dataIndices[type].push_back(Index<RawData>{ 0 });
		}
	}

	template<class T>
	inline void Everything::removeComponent(Index<Everything> i) {
		this->removeComponent(i, component_index_v<T>);
	}

	template<class T, class... Args>
	inline T& Everything::add(Index<Everything> i, Args&&... args) {
		[[maybe_unused]]
		auto b = RegisterStruct<T>::initialized;
		assert(!this->has<T>(i));
		auto [index, ptr] = this->data[component_index_v<T>].template add<T>(i, std::forward<Args>(args)...);
		this->dataIndices[component_index_v<T>][i] = index;
		this->signatures[i].set(component_index_v<T>);
		return *ptr;
	}

	template<class T>
	inline T& Everything::get(Index<Everything> i) {
		return this->data[component_index_v<T>].template get<T>(this->dataIndices[component_index_v<T>][i]);
	}

	template<class T>
	inline RawData& Everything::gets() {
		return this->gets(component_index_v<T>);
	}

	template<class... Ts>
	inline bool Everything::has(Index<Everything> i) const {
		auto const sig = group_signature_v<Ts...>;
		return (this->signatures[i] & sig) == sig;
	}

	template<class F>
	inline void Everything::run(F f) {
		Loop::run(*this, f);
	}

	template<class F>
	inline void Everything::match(F f) {
		using arguments_list = typename te::remove_ref_and_const_t<te::arguments_list_t<F>>;
		MatchExpanded<arguments_list>::run(*this, f);
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
	inline void WeakObject::remove() {
		assert(this->isNotNull());
		this->proxy->removeComponent<T>(this->index);
	}

	template<class T>
	inline std::optional<T*> WeakObject::getMaybe() {
		if (this->has<T>()) {
			return &this->get<T>();
		}
		else {
			return std::nullopt;
		}
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
	inline bool WeakObject::has() const {
		assert(this->isNotNull());
		return this->proxy->has<T>(this->index);
	}
}
