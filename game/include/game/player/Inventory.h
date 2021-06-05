#pragma once

#include <vector>

#include <serial/Serializer.h>

#include "Game.h"


namespace game
{
	struct Inventory
	{
		std::vector<UniqueObject> items;

		std::optional<UniqueObject> cursor;
	};
}

template<>
struct Identifier<game::Inventory>
{
	inline static std::string name = "Inventory";
};

template<>
struct Serializable<game::Inventory>
{
	template<class Selector, class T>
	static bool run(Serializer& serializer, T inventory) {
		return serializer.runAll<Selector>(
			inventory.items,
			inventory.cursor
			);
	}
};
