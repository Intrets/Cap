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

template<class K, class V>
static void updateOrInsert(std::map<K, V>& map, K key, V value, std::function<V(V)> update) {
	auto it = map.find(key);
	if (it != map.end()) {
		map[key] = update(it->second);
	}
	else {
		map[key] = value;
	}
}

bool PathFinding::step(game::WorldGrid& grid) {
	switch (this->stage) {
		case 0:
			while (!this->stage1(grid)) {
				return false;
			}
			return false;
		case 1:
			while (!this->stage2(grid)) {
				return false;
			}
			return false;
		case 2:
			return this->stage3(grid);
		default:
			return false;
			break;
	}
}

bool PathFinding::stage1(game::WorldGrid& grid) {
	if (found) {
		return true;
	}
	if (this->tick++ % 1 != 0) {
		return false;
	}

	if (front.empty()) {
		return false;
	}

	auto F = front.front();
	//auto F = front.top();
	front.pop();

	if (glm::abs(F.winding) > 20) {
		return false;
	}

	auto D = target - F.current;
	auto Dabs = glm::abs(D);
	auto m = glm::max(Dabs.x, Dabs.y);

	if (m == 0) {
		found = true;
		this->stage++;
		this->path = F.path;
		this->waypoints = F.waypoints;
		//this->newWaypoints.push_back(this->target);
		return true;
	}

	D /= m;

	glm::ivec2 p = F.current + D;

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


	if (!F.collided) {
		if (!grid.occupied(p.x, p.y)) {
			this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), this->count });
			F.path.push_back(p);
			//this->current = p;
			F.current = p;
			this->front.push(F);
		}
		else {
			auto w = F.winding;
			//this->direction = dir;
			//this->collided = true;
			F.D = norm2(this->target - p);
			F.clockwise = true;
			F.collided = true;
			F.direction = prev(prev(dir));
			F.winding = w + 2;
			this->front.push(F);

			F.D = norm2(this->target - p);
			F.clockwise = false;
			F.winding = w + 2;
			F.direction = next(next(dir));
			this->front.push(F);
		}
		return false;
	}

	if (glm::abs(F.winding) < 2) {
		auto it = this->visited.find(1);
		if (!grid.occupied(p.x, p.y) && !this->visited.count(*reinterpret_cast<uint64_t*>(&p))) {
			F.waypoints.push_back(p);

			//this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), this->count });
			updateOrInsert<uint64_t, int32_t>(this->visited,
				*reinterpret_cast<uint64_t*>(&p),
				this->count,
				[](int32_t v) {return v; }
			);

			//this->direction = dir;
			//this->path.push_back(p);
			//this->collided = false;

			F.direction = dir;
			F.collided = false;
			F.path.push_back(p);
			F.current = p;
			F.D = norm2(this->target - p);

			this->front.push(F);

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

	auto dec = F.clockwise ? next : prev;
	auto inc = F.clockwise ? prev : next;
	size_t index = (inc(inc(flip(F.direction))));
	for (size_t i = 0; i < 8; i++) {
		p = F.current + order[index];
		this->searched.push_back(p);

		if (!grid.occupied(p.x, p.y)) {
			this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), this->count });

			//this->path.push_back(p);

			//this->winding += i - 1;
			//this->direction = index;

			F.direction = index;
			F.winding += i - 2;
			F.path.push_back(p);
			F.current = p;
			F.D = norm2(this->target - p);
			this->front.push(F);

			Locator<misc::Log>::ref().putStreamLine(std::stringstream() << "winding: " << F.winding << " i: " << i);


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

static int dist(glm::ivec2 p1, glm::ivec2 p2) {
	auto a = glm::abs(p1 - p2);

	return glm::max(a.x, a.y);
}

bool PathFinding::stage2(game::WorldGrid& grid) {
	if (this->tick++ % 1 != 0) {
		return false;
	}
	if (!this->path.empty()) {
		auto p = this->path.back();
		this->path.pop_back();

		auto D = dist(p, this->waypoints.back());

		if (D < this->lastD) {
			this->cutoff = false;
			lastD = D;
			this->prunedPath.push_back(p);
		}
		else {
			if (!this->cutoff) {
				this->newWaypoints.push_back(lastP);
			}
			this->cutoff = true;
		}

		if (p == this->waypoints.back()) {
			lastD = 999999;
			this->waypoints.pop_back();
			this->newWaypoints.push_back(p);
		}

		lastP = p;
	}
	else {
		this->stage++;

		this->newWaypoints.erase(std::remove_if(
			this->newWaypoints.begin(),
			this->newWaypoints.end(),
			[&](glm::ivec2 p) {
				size_t count = 0;
				for (size_t i = 0; i < 11; i++) {
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

					if (grid.occupied(p + order[i % 8])) {
						if (count++ > 2) {
							return true;
						}
					}
					else {
						count = 0;
					}

				}
				return false;
			}
		), this->newWaypoints.end());

		this->newWaypoints.push_back(this->start);
		this->newWaypoints.insert(this->newWaypoints.begin(), this->target);
		return true;
	}

	return false;
}

bool PathFinding::stage3(game::WorldGrid& grid) {
	if (this->tick++ % 1 != 0) {
		return false;
	}
	if (F.get() == nullptr) {
		if (this->newWaypoints.size() < 2) {
			this->waypoints.clear();
			return true;
		}
		F = std::make_unique<PathFinding>();
		auto start = this->newWaypoints.back();
		this->highlight1 = start;
		this->newWaypoints.pop_back();
		auto target = this->newWaypoints.back();
		this->highlight2 = target;

		Front f{ start };
		f.waypoints.push_back(start);
		F->front.push(f);
		F->target = target;
	}
	else {
		if (this->F->stage1(grid)) {
			finalPath.insert(finalPath.end(), F->path.begin(), F->path.end());
			this->F.reset();
		}
	}

	//for (size_t i = 0; i < this->newWaypoints.size() - 1; i++) {
	//	auto start = this->newWaypoints[i];
	//	auto target = this->newWaypoints[i + 1];
	//	F.target = target;

	//	Front f{ start };
	//	f.waypoints.push_back(start);

	//	F.front.push(f);

	//	while (!F.stage1(grid)) {}

	//	finalPath.insert(finalPath.end(), F.path.begin(), F.path.end());
	//}
	return false;
}














