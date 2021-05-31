#pragma once

#include <wglm/glm.hpp>

#include <array>
#include <cstdint>

constexpr auto WORLD_SIZE = 70;
using SizeAlias = size_t;

namespace game
{
	glm::ivec2 getDirectionFromIndex(uint32_t index);
	uint32_t getIndexFromDirection(glm::ivec2 vec);

	struct Directions
	{
		//uint32_t data;
		std::array<uint16_t, 4> data;

		glm::ivec2 getDirection(uint32_t neighbour) const;
		void setDirection(uint32_t neighbour, uint32_t direction);
		void setDirection(uint32_t neighbour, glm::ivec2 direction);
	};

	struct WorldGrid
	{
		std::array<std::array<SizeAlias, WORLD_SIZE>, WORLD_SIZE> grid;
		std::array<std::array<int32_t, WORLD_SIZE>, WORLD_SIZE> group;
		std::array<std::array<Directions, WORLD_SIZE>, WORLD_SIZE> directions;

		bool isGrouped(glm::ivec2 pos);
		void setGroup(glm::ivec2 pos, int32_t g);
		int32_t getGroup(glm::ivec2 pos) const;

		SizeAlias get(int32_t x, int32_t y);
		SizeAlias get(glm::ivec2 pos);

		void place(SizeAlias index, int32_t x, int32_t y);
		void place(SizeAlias index, glm::ivec2 pos);

		void remove(int32_t x, int32_t y);
		void remove(glm::ivec2 pos);

		bool occupied(int32_t x, int32_t y);
		bool occupied(glm::ivec2 pos);
		bool empty(glm::ivec2 pos);
	};
}
