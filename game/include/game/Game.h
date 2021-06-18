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
#include <ranges>

#include <tepp/tepp.h>

#include <misc/Misc.h>

#include <mem/Index.h>
#include <mem/Global.h>
#include <mem/LazyGlobal.h>

#include <serial/Serializer.h>

struct Component;

template<class T>
struct Identifier;

struct StructInformation
{
	std::string name{};
	Index<Component> index{};
	size_t width{};

	void(*clone)(void* source, void* target) = nullptr;
	void(*objectDestructor)(void*) = nullptr;
	bool(*objectReader)(serial::Serializer& serializer, void*) = nullptr;
	bool(*objectWriter)(serial::Serializer& serializer, void*) = nullptr;
	bool(*objectPrinter)(serial::Serializer& serializer, void*) = nullptr;
};

struct StoredStructInformations
{
	std::unordered_map<std::string, StructInformation> infos{};
};

template<>
struct serial::Serializable<StructInformation>
{
	static constexpr std::string_view typeName = "StructInformation";

	static bool run(Read, Serializer& serializer, StructInformation&& val) {
		std::string name;
		READ(name);
		if (name != "") {
			assert(LazyGlobal<StoredStructInformations>->infos.contains(name));
			val = LazyGlobal<StoredStructInformations>->infos[name];
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

		NO_COPY(RawData);
		DEFAULT_MOVE(RawData);

		template<class T>
		inline void remove(Index<RawData> i);

		inline void removeUntyped(Index<RawData> i);

		template<class T>
		inline T& get(Index<RawData> i);

		inline void* getUntyped(Index<RawData> i);

		inline Index<Everything> getIndex(Index<RawData> i) const;

		bool print(serial::Serializer& serializer, Index<RawData> i);

		template<class T, class... Args>
		[[nodiscard]]
		inline std::pair<Index<RawData>, T*> add(Index<Everything> i, Args&&... args);

		[[nodiscard]]
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

	READ_DEF(game::RawData) {
		if (!serializer.readAll(
			obj.structInformation,
			obj.reservedObjects,
			obj.index,
			obj.objectSize,
			obj.indices,
			obj.deletions
		)) return false;

		obj.data.resize(obj.structInformation.width * obj.reservedObjects);

		for (Index<game::RawData> i{ 1 }; i < obj.index; i++) {
			if (!obj.structInformation.objectReader(serializer, obj.getUntyped(i))) return false;
		}

		return true;
	};

	WRITE_DEF(game::RawData) {
		if (!serializer.writeAll(
			obj.structInformation,
			obj.reservedObjects,
			obj.index,
			obj.objectSize,
			obj.indices,
			obj.deletions
		)) return false;

		for (Index<game::RawData> i{ 1 }; i < obj.index; i++) {
			if (!obj.structInformation.objectWriter(serializer, obj.getUntyped(i))) return false;
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

		Index<Everything> getIndex() const {
			return this->index;
		}

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

		bool has(Index<Component> i) const;
		Index<RawData> getComponentIndex(Index<Component> type) const;
	};

	struct UniqueObject : WeakObject
	{
		void release();

		UniqueObject(WeakObject const& other);

		UniqueObject& operator=(UniqueObject&& other) noexcept;
		UniqueObject(UniqueObject&& other) noexcept;

		UniqueObject() = default;
		~UniqueObject();

		NO_COPY(UniqueObject);
	};

	struct QualifiedObject
	{
		WeakObject object;

		Qualifier qualifier = 0;

		void set(WeakObject obj);
		WeakObject getObject() const {
			return this->object;
		}
		WeakObject getObject() {
			return this->object;
		}

		bool isQualified() const;

		WeakObject* operator->();

		QualifiedObject& operator=(WeakObject const& other) noexcept;
		QualifiedObject(WeakObject const& other) noexcept;

		QualifiedObject() = default;
		~QualifiedObject() = default;

		DEFAULT_COPY_MOVE(QualifiedObject);
	};

	struct NewEverything
	{
		Everything* ptr = nullptr;

		NewEverything(Everything* p) : ptr(p) {};
		NewEverything() = default;
		~NewEverything() = default;
	};

	struct Everything
	{
		struct ComponentCounter
		{
			size_t t = 0;

			Index<Component> increment() {
				return { t++ };
			}

			size_t size() {
				return t;
			}
		};

		template<class T>
		struct ComponentIndex
		{
			Index<Component> val = LazyGlobal<ComponentCounter>->increment();
		};

		template<class T>
		struct component_index
		{
			static inline Index<Component> getVal() {
				assert(LazyGlobal<ComponentCounter>->size() < SIZE);

				StructInformation info;
				static_assert(serial::has_type_name<T>);
				info.name = serial::Serializable<T>::typeName;
				info.index = LazyGlobal<ComponentIndex<T>>->val;
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

				info.clone = [](void* source, void* target) {
					new (target) T(*reinterpret_cast<T*>(source));
				};

				LazyGlobal<StoredStructInformations>->infos.insert({ info.name, info });

				return info.index;
			};

			static inline Index<Component> val = getVal();
		};

		template<class T>
		static inline Index<Component> component_index_v = component_index<T>::val;

		template<class M, class... Ms>
		struct group_signature
		{
			static const inline SignatureType fillSignature() {
				SignatureType res{};
				if constexpr (sizeof...(Ms) == 0) {
					res.set(LazyGlobal<ComponentIndex<M>>->val);
				}
				else {
					for (auto s : { LazyGlobal<ComponentIndex<M>>->val, LazyGlobal<ComponentIndex<Ms>>->val... }) {
						res.set(s);
					}
				}
				return res;
			};

			static const inline SignatureType value = fillSignature();
		};

		template<class... Ms>
		static inline const SignatureType group_signature_v = group_signature<Ms...>::value;


		std::vector<Index<Everything>> freeIndirections{};

		std::vector<Qualifier> qualifiers{ 0 };
		std::vector<int32_t> validIndices{ false };
		Qualifier qualifier = 1;

		std::vector<SignatureType> signatures{ 0 };
		std::array<std::vector<Index<RawData>>, SIZE> dataIndices;

		std::vector<Index<Everything>> removed{};

		std::vector<RawData> data{ SIZE };


		WeakObject make();
		UniqueObject makeUnique();

		UniqueObject cloneAll(WeakObject const& obj);

		template<class... Ms>
		UniqueObject clone(WeakObject const& obj);

		UniqueObject clone(std::vector<Index<Component>> components, WeakObject const& obj);

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

		inline void removeComponent(Index<Everything> i, Index<Component> type);

		template<class T, class... Args>
		inline T& add(Index<Everything> i, Args&&... args);

		template<class T>
		inline T& get(Index<Everything> i);

		template<class T>
		inline RawData& gets();

		inline RawData& gets(Index<Component> type);

		bool print(serial::Serializer& serializer, Index<Everything> index, Index<Component> type);

		template<class... Ts>
		inline bool has(Index<Everything> i) const;

		inline bool has(Index<Everything> i, Index<Component> type) const;

		Index<RawData> getComponentIndex(Index<Everything> i, Index<Component> type) const;

		template<class F>
		inline void run(F f);

		template<class F>
		inline void match(F f);

		template<class... Ms>
		Index<Component> selectPivot();

		size_t getTypeCount();

		Everything();
		~Everything() = default;

		NO_COPY(Everything);
		DEFAULT_MOVE(Everything);
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
	}
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

	WRITE_DEF(game::WeakObject) {
		return serializer.writeAll(
			ALL(index)
		);
	}

	READ_DEF(game::WeakObject) {
		auto b = serializer.readAll(
			ALL(index)
		);

		assert(Global<game::NewEverything>()->ptr != nullptr);
		obj.proxy = Global<game::NewEverything>()->ptr;

		return b;
	}
};

template<>
struct serial::Serializable<game::UniqueObject>
{
	inline static const auto typeName = "UniqueObject";

	PRINT_DEF(game::UniqueObject) {
		game::WeakObject objWeak = obj;
		return serializer.printAll(objWeak);
	}

	WRITE_DEF(game::UniqueObject) {
		game::WeakObject weakObj = obj;

		return serializer.writeAll(
			weakObj
		);
	}

	READ_DEF(game::UniqueObject) {
		game::WeakObject weakObj;
		auto b = serializer.readAll(
			weakObj
		);

		obj.index = weakObj.index;
		obj.proxy = weakObj.proxy;

		return b;
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
			return serializer.printAll(obj.object);
		}
	}

	ALL_DEF(game::QualifiedObject) {
		assert(0);
		return true;
	}
};

namespace game
{
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
			auto pivot = e.selectPivot<M, Ms...>();

			auto& g = e.gets(pivot);
			const auto end = g.index;

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
	struct MatchExpanded<te::list<WeakObject, M, Ms...>>
	{
		template<class F>
		static inline void run(Everything& e, F f) {
			auto pivot = e.selectPivot<M, Ms...>();

			auto& g = e.gets(pivot);
			const auto end = g.index;

			if constexpr (sizeof...(Ms) == 0) {
				for (Index<game::RawData> i{ 1 }; i < end; i++) {
					auto index = g.getIndex(i);
					f(WeakObject{ index, &e }, g.template get<M>(i));
				}
			}
			else {
				for (Index<game::RawData> i{ 1 }; i < end; i++) {
					auto index = g.getIndex(i);

					if (e.has<Ms...>(index)) {
						f(WeakObject{ index, &e }, e.template get<M>(index), e.get<Ms>(index)...);
					}
				}
			}
		};
	};

	template<class M, class... Ms>
	struct MatchExpanded<te::list<M, Ms...>>
	{
		template<class F>
		static inline void run(Everything& e, F f) {
			auto pivot = e.selectPivot<M, Ms...>();

			auto& g = e.gets(pivot);
			const auto end = g.index;

			if constexpr (sizeof...(Ms) == 0) {
				for (Index<game::RawData> i{ 1 }; i < end; i++) {
					f(g.template get<M>(i));
				}
			}
			else {
				for (Index<game::RawData> i{ 1 }; i < end; i++) {
					auto index = g.getIndex(i);

					if (e.has<Ms...>(index)) {
						f(e.template get<M>(index), e.get<Ms>(index)...);
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

		std::sort(this->deletions.begin(), this->deletions.end(), [](auto left, auto right) {return left.i > right.i; });

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
		assert(this->deletions.empty());
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

			this->structInformation = LazyGlobal<StoredStructInformations>->infos[std::string(serial::Serializable<T>::typeName)];
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
		this->validIndices[i] = false;
	}

	inline void Everything::collectRemoved() {
		for (size_t type = 0; type < this->getTypeCount(); type++) {
			for (auto const& d : this->data[type].packDeletions()) {
				this->dataIndices[type][d.changed] = d.i;
			}
		}

		for (auto i : this->removed) {
			assert(this->signatures[i].none());
			this->freeIndirections.push_back(i);
		}

		this->removed.clear();
	}

	inline void Everything::removeComponent(Index<Everything> i, Index<Component> type) {
		assert(this->signatures[i].test(type));
		this->data[type].removeUntyped(this->dataIndices[type][i]);
		this->signatures[i].reset(type);
	}

	inline RawData& Everything::gets(Index<Component> type) {
		return this->data[type];
	}

	inline bool Everything::print(serial::Serializer& serializer, Index<Everything> index, Index<Component> type) {
		return this->data[type].print(serializer, this->dataIndices[type][index]);
	}

	inline bool Everything::has(Index<Everything> i, Index<Component> type) const {
		return this->signatures[i].test(type);
	}

	inline Index<RawData> Everything::getComponentIndex(Index<Everything> i, Index<Component> type) const {
		assert(this->has(i, type));
		return this->dataIndices[type][i];
	}

	inline size_t Everything::getTypeCount() {
		return LazyGlobal<ComponentCounter>->size();
	}

	inline Everything::Everything() {
		for (Index<Component> type{ 0 }; type < SIZE; type++) {
			this->dataIndices[type].push_back(Index<RawData>{ 0 });
		}
	}

	template<class ...Ms>
	inline UniqueObject Everything::clone(WeakObject const& obj) {
		std::vector<Index<Component>> components;

		for (auto component : { Everything::component_index<Ms>::val... }) {
			components.push_back(component);
		}

		return this->clone(components, obj);
	}

	template<class T>
	inline void Everything::removeComponent(Index<Everything> i) {
		this->removeComponent(i, component_index_v<T>);
	}

	template<class T, class... Args>
	inline T& Everything::add(Index<Everything> i, Args&&... args) {
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
		using arguments_list = te::map_t<std::remove_cvref_t, te::arguments_list_t<F>>;
		MatchExpanded<arguments_list>::run(*this, f);
	}

	template<class... Ms>
	inline Index<Component> Everything::selectPivot() {
		Index<Component> pivot{ 0 };
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
