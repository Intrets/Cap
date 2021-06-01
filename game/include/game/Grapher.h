#pragma once

#include <utility>

#include <wglm/glm.hpp>
#include <misc/Rectangle.h>

#include "Game.h"
#include "WorldGrid.h"

struct Grapher
{
	struct Region
	{
		glm::ivec2 bot;
		glm::ivec2 top;

		bool contains(glm::ivec2 p);
	} region;

	struct Front
	{
		glm::ivec2 min;
		glm::ivec2 max;

		int32_t direction;
		int32_t size;

		void tryExpand(glm::ivec2 p);
	};

	bool finished = false;

	int32_t currentDepth = 0;

	//int32_t blockSize = std::numeric_limits<int32_t>::max();
	int32_t blockSize = 13;
	glm::ivec2 currentBlock;

	glm::ivec2 lastPos;
	int32_t lastGroup = 0;

	std::vector<Front> fronts;

	std::vector<glm::ivec2> groups;

	bool incrementLastPos();

	void debugRender();

	bool step(game::WorldGrid& grid);
};

struct RandomWalker
{
	int32_t indexTarget;
	int32_t groupTarget;
};
