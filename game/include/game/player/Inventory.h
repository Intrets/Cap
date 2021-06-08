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
	ALL_DEF(game::Inventory) {
		return serializer.runAll<Selector>(
			ALL(items),
			ALL(cursor)
			);
	}
};
