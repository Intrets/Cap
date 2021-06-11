#pragma once

#include <unordered_map>
#include <vector>
#include <cstdint>

#include <wglm/glm.hpp>

#include <serial/Serializer.h>

#include "Game.h"

namespace game
{
	struct WorldGrid;
}

struct Merger
{
	struct Neighbour
	{
		int32_t group{ 0 };
		std::vector<glm::ivec2> front{};
	};

	struct Group
	{
		int32_t size{ 0 };
		int32_t group{ 0 };
		std::vector<Neighbour> neighbours{};
		glm::ivec2 seed;
		glm::vec2 approximation;
	};

	std::vector<Group> groups;

	std::vector<int32_t> nonEmptyGroups;

	void fillPaths(game::WorldGrid& grid, int32_t group);

	bool hasNeighbour(int32_t group, int32_t neighbour);
	int32_t getNeighbourCount(int32_t group);

	void initialize(std::vector<glm::ivec2>& seedPoints, game::WorldGrid& grid);
	void mergeStep(game::WorldGrid& grid);
	void mergeGroups(game::WorldGrid& grid, int32_t group, int32_t into);
	void debugRender();
};

template<>
struct serial::Serializable<Merger::Neighbour>
{
	ALL_DEF(Merger::Neighbour) {
		return serializer.runAll<Selector>(
			ALL(group),
			ALL(front)
			);
	}
};

template<>
struct serial::Serializable<Merger::Group>
{
	ALL_DEF(Merger::Group) {
		return serializer.runAll<Selector>(
			ALL(size),
			ALL(group),
			ALL(neighbours),
			ALL(seed),
			ALL(approximation)
			);
	}
};

template<>
struct serial::Serializable<Merger>
{
	static constexpr std::string_view typeName = "Merger";

	ALL_DEF(Merger) {
		return serializer.runAll<Selector>(
			ALL(groups),
			ALL(nonEmptyGroups)
			);
	}
};
