#pragma once

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
		int32_t group;
		std::vector<glm::ivec2> front;
	};

	struct Group
	{
		int32_t group;
		std::vector<Neighbour> neighbours;
	};

	std::vector<Group> groups;

	void initialize(std::vector<glm::ivec2>& seedPoints, game::WorldGrid& grid);
	void debugRender();
};