#pragma once

#include <unordered_map>
#include <vector>
#include <cstdint>

#include <wglm/glm.hpp>

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

	bool hasNeighbour(int32_t group, int32_t neighbour);
	int32_t getNeighbourCount(int32_t group);

	void initialize(std::vector<glm::ivec2>& seedPoints, game::WorldGrid& grid);
	void mergeStep(game::WorldGrid& grid);
	void mergeGroups(game::WorldGrid& grid, int32_t group, int32_t into);
	void debugRender();
};