#include "WorldGrid.h"

#include <queue>
#include <cstdint>

#include <misc/Num.h>

namespace game
{
	bool Directions::hasDirection(int32_t index) {
		return this->data[index] & 0x10_u8;
	}

	bool WorldGrid::hasDirection(glm::ivec2 p, int32_t index) {
		return this->directions[p.x][p.y].hasDirection(index);
	}

	void WorldGrid::setDirection(glm::ivec2 p, int32_t index, uint8_t direction) {
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

	std::optional<WeakObject> WorldGrid::get(int32_t x, int32_t y) {
		if (0 <= x && x < WORLD_SIZE && 0 <= y && y < WORLD_SIZE) {
			if (this->grid[x][y].data.empty()) {
				return std::nullopt;
			}
			else {
				return this->grid[x][y].data.front();
			}
		}
		else {
			return std::nullopt;
		}
	}

	std::optional<WeakObject> WorldGrid::get(glm::ivec2 pos) {
		return this->get(pos.x, pos.y);
	}

	bool WorldGrid::occupied(int32_t x, int32_t y) {
		return this->occupied({ x, y });
	}

	bool WorldGrid::occupied(glm::ivec2 pos) {
		return !this->empty(pos);
	}

	bool WorldGrid::empty(glm::ivec2 pos) {
		return !this->get(pos).has_value();
	}

	glm::ivec2 Directions::getDirection(int32_t index) const {
		return getDirectionFromIndex(this->data[index] & 0x0F);
	}

	void Directions::setDirection(int32_t index, uint8_t direction) {
		this->data[index] = direction | 0x10_u8;
	}

	void Directions::setDirection(int32_t index, glm::ivec2 direction) {
		this->data[index] = getIndexFromDirection(direction) | 0x10_u8;
	}

	uint8_t getIndexFromDirection(glm::ivec2 vec) {
		assert(!(vec.x == 0 && vec.y == 0));
		assert(vec.x >= -1 && vec.y >= -1);
		assert(vec.x <= 1 && vec.y <= 1);

		const uint8_t i[3][3] = {
			{6,7,0},
			{5,0,1},
			{4,3,2}
		};

		return i[vec.y + 1][vec.x + 1];
	}

	UniqueObject WorldGrid::Trace::release(WeakObject const& obj) {
		assert(obj.isNotNull());

		auto it = std::find_if(this->data.begin(), this->data.end(), [i = obj.index](UniqueObject const& o) {
			return o.index == i;
		});
		assert(it != this->data.end());
		it->release();

		if (it != this->data.end() - 1) {
			*it = std::move(this->data.back());
		}
		this->data.pop_back();

		return obj;
	}

	void WorldGrid::Trace::add(WeakObject const& obj) {
		this->data.push_back(obj);
	}

	void WorldGrid::Trace::add(UniqueObject&& obj) {
		this->data.push_back(std::move(obj));
	}

	void WorldGrid::Trace::clear() {
		this->data.clear();
	}
}