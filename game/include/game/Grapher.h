#pragma once

#include <utility>

#include <wglm/glm.hpp>
#include <misc/Rectangle.h>

#include "Game.h"
#include "WorldGrid.h"

struct Grapher
{
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

template<>
struct serial::Serializable<Grapher::Front>
{
	static constexpr std::string_view typeName = "Front";

	ALL_DEF(Grapher::Front) {
		return serializer.runAll<Selector>(
			ALL(min),
			ALL(max),
			ALL(direction),
			ALL(size)
			);
	}
};

template<>
struct serial::Serializable<Grapher>
{
	static constexpr std::string_view typeName = "Grapher";

	ALL_DEF(Grapher) {
		return serializer.runAll<Selector>(
			ALL(finished),
			ALL(currentDepth),
			ALL(blockSize),
			ALL(currentBlock),
			ALL(lastPos),
			ALL(lastGroup),
			ALL(fronts),
			ALL(groups)
			);
	}
};

struct RandomWalker
{
	int32_t indexTarget;
	int32_t groupTarget;
};

template<>
struct serial::Serializable<RandomWalker>
{
	static constexpr std::string_view typeName = "RandomWalker";

	ALL_DEF(RandomWalker) {
		return serializer.runAll<Selector>(
			ALL(indexTarget),
			ALL(groupTarget)
			);
	}
};

