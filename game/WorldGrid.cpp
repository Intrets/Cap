#include "WorldGrid.h"

#include <queue>
#include <cstdint>

namespace game
{
	bool Directions::hasDirection(int32_t index) {
		return this->data[index] & 0x10;
	}

	bool WorldGrid::hasDirection(glm::ivec2 p, int32_t index) {
		return this->directions[p.x][p.y].hasDirection(index);
	}

	void WorldGrid::setDirection(glm::ivec2 p, int32_t index, int32_t direction) {
		this->directions[p.x][p.y].setDirection(index, direction);
	}

	void WorldGrid::setDirection(glm::ivec2 p, int32_t index, glm::ivec2 direction) {
		this->directions[p.x][p.y].setDirection(index, direction);
	}

	glm::ivec2 WorldGrid::getDirection(glm::ivec2 p, int32_t index) {
		return this->directions[p.x][p.y].getDirection(index);
	}

	void WorldGrid::replaceGroup(glm::ivec2 p, int32_t replace, int32_t with) {
		assert(this->getGroup(p) == replace);

		std::vector<glm::ivec2> open;
		this->setGroup(p, with);
		open.push_back(p);

		while (!open.empty()) {
			auto point = open.back();
			open.pop_back();

			const std::array<glm::ivec2, 4> D{
				glm::ivec2(1,0),
				glm::ivec2(0,1),
				glm::ivec2(-1,0),
				glm::ivec2(0,-1)
			};

			for (auto d : D) {
				auto point2 = point + d;

				if (this->getGroup(point2) == replace) {
					this->setGroup(point2, with);
					open.push_back(point2);
				}
			}
		}
	}

	bool WorldGrid::isGrouped(glm::ivec2 pos) {
		return this->group[pos.x][pos.y] != 0;
	}

	void WorldGrid::setGroup(glm::ivec2 pos, int32_t g) {
		this->group[pos.x][pos.y] = g;
	}

	int32_t WorldGrid::getGroup(glm::ivec2 pos) const {
		return this->group[pos.x][pos.y];
	}

	SizeAlias WorldGrid::get(int32_t x, int32_t y) {
		return this->grid[x][y];
	}

	SizeAlias WorldGrid::get(glm::ivec2 pos) {
		return this->grid[pos.x][pos.y];
	}

	void WorldGrid::place(SizeAlias index, int32_t x, int32_t y) {
		this->grid[x][y] = index;
	}

	void WorldGrid::place(SizeAlias index, glm::ivec2 pos) {
		this->grid[pos.x][pos.y] = index;
	}

	void WorldGrid::remove(int32_t x, int32_t y) {
		this->grid[x][y] = 0;
	}

	void WorldGrid::remove(glm::ivec2 pos) {
		this->grid[pos.x][pos.y] = 0;
	}

	bool WorldGrid::occupied(int32_t x, int32_t y) {
		return this->get(x, y) != 0;
	}

	bool WorldGrid::occupied(glm::ivec2 pos) {
		return this->get(pos) != 0;
	}

	bool WorldGrid::empty(glm::ivec2 pos) {
		return this->get(pos) == 0;
	}

	glm::ivec2 Directions::getDirection(int32_t index) const {
		return getDirectionFromIndex(this->data[index] & 0x0F);
	}

	void Directions::setDirection(int32_t index, int32_t direction) {
		this->data[index] = direction | 0x10;
	}

	void Directions::setDirection(int32_t index, glm::ivec2 direction) {
		this->data[index] = getIndexFromDirection(direction) | 0x10;
	}

	glm::ivec2 getDirectionFromIndex(uint32_t index) {
		switch (index) {
			case 0:
				return glm::ivec2(1, 1);
				break;
			case 1:
				return glm::ivec2(1, 0);
				break;
			case 2:
				return glm::ivec2(1, -1);
				break;
			case 3:
				return glm::ivec2(0, -1);
				break;
			case 4:
				return glm::ivec2(-1, -1);
				break;
			case 5:
				return glm::ivec2(-1, 0);
				break;
			case 6:
				return glm::ivec2(-1, 1);
				break;
			case 7:
				return glm::ivec2(0, 1);
				break;
			default:
				assert(0);
				return glm::ivec2(0, 0);
				break;
		}
	}

	uint32_t getIndexFromDirection(glm::ivec2 vec) {
		assert(!(vec.x == 0 && vec.y == 0));
		assert(vec.x >= -1 && vec.y >= -1);
		assert(vec.x <= 1 && vec.y <= 1);

		const uint32_t i[3][3] = {
			{6,7,0},
			{5,0,1},
			{4,3,2}
		};

		return i[vec.y + 1][vec.x + 1];
	}
}