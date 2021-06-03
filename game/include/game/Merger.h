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
struct Serializable<Merger::Neighbour>
{
	template<class Selector, class T>
	static bool run(Serializer& serializer, T neighbour) {
		return serializer.runAll<Selector>(
			neighbour.group,
			neighbour.front
			);
	}
};

template<>
struct Serializable<Merger::Group>
{
	template<class Selector, class T>
	static bool run(Serializer& serializer, T group) {
		return serializer.runAll<Selector>(
			group.size,
			group.group,
			group.neighbours,
			group.seed,
			group.approximation
			);
	}
};

template<>
struct Serializable<Merger>
{
	template<class Selector, class T>
	static bool run(Serializer& serializer, T merger) {
		return serializer.runAll<Selector>(
			merger.groups,
			merger.nonEmptyGroups
			);
	}
};

template<>
struct Identifier<Merger>
{
	inline static std::string name = "Merger";
};