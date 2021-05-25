#include "GameObject.h"

#include "GameState.h"

#include <wglm/gtc/matrix_integer.hpp>
#include <misc/Misc.h>
#include <mem/Locator.h>
#include <misc/Log.h>

static auto next(size_t i) {
	return (i + 1) % 8;
}

static auto prev(size_t i) {
	return (i + 7) % 8;
}

static auto flip(size_t i) {
	return (i + 4) % 8;
}

bool PathFinding::step(game::WorldGrid& grid) {
	if (this->tick++ % 5 != 0) {
		return false;
	}

	auto D = target - current;
	//auto m = glm::max(D.x, D.y);
	auto m = glm::abs(D.x) > glm::abs(D.y) ? D.x : D.y;

	if (m == 0) {
		return true;
	}

	D /= m;

	glm::ivec2 p = this->current + D;

	int32_t match = 16 * (D.x + 1) + (D.y + 1);

	size_t dir;
	switch (match) {
		case 0x22:
			dir = 0;
			break;
		case 0x21:
			dir = 1;
			break;
		case 0x20:
			dir = 2;
			break;
		case 0x10:
			dir = 3;
			break;
		case 0x00:
			dir = 4;
			break;
		case 0x01:
			dir = 5;
			break;
		case 0x02:
			dir = 6;
			break;
		case 0x12:
			dir = 7;
			break;
		default:
			break;
	}


	if (!this->collided) {

		if (!this->visited.count(*reinterpret_cast<uint64_t*>(&p)) && !grid.occupied(p.x, p.y)) {
			this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), this->path.size() });
			this->path.push_back(p);
			this->current = p;
		}
		else {
			this->direction = dir;
			this->collided = true;
		}
		return false;
	}

	if (this->winding < 2) {
		if (!this->visited.count(*reinterpret_cast<uint64_t*>(&p)) && !grid.occupied(p.x, p.y)) {
			this->waypoints.push_back(this->current);

			this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), this->path.size() });
			this->current = p;

			this->direction = dir;
			this->path.push_back(p);
			this->collided = false;

			return false;
		}
	}

	static const std::vector<glm::ivec2> order = {
		{1,1},
		{1,0},
		{1, -1},
		{0,-1},
		{-1,-1},
		{-1,0},
		{-1,1},
		{0,1},
	};

	this->searched.clear();

	auto dec = previousClockWise ? next : prev;
	auto inc = previousClockWise ? prev : next;
	size_t index = inc(inc(inc(flip(direction))));
	for (size_t i = 0; i < 8; i++) {
		p = this->current + order[index];
		this->searched.push_back(p);

		if (!this->visited.count(*reinterpret_cast<uint64_t*>(&p)) && !grid.occupied(p.x, p.y)) {
			this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), this->path.size() });

			this->path.push_back(p);
			this->current = p;

			this->winding += i - 1;
			//this->winding += (4 - i);
			//if (this->previousClockWise) {
			//}
			//else {
			//	this->winding -= this->direction - index;
			//}


			Locator<misc::Log>::ref().putStreamLine(std::stringstream() << this->winding);


			this->direction = index;
			return false;
		}
		index = inc(index);
	}


	//for (auto v : vec) {
	//	p = this->current + (rot * v);

	//	if (!this->visited.count(*reinterpret_cast<uint64_t*>(&p)) && !grid.occupied(p.x, p.y)) {
	//		this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), this->path.size() });

	//		this->current = p;
	//		this->path.push_back(p);

	//		this->previousDirection = rot * v;
	//		return false;
	//	}
	//}

	//this->path.pop_back();
	//this->current = this->path.back();

	return false;
}
