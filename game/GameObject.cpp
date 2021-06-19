#include "GameObject.h"

#include <bit>

#include <wglm/gtc/matrix_integer.hpp>
#include <misc/Misc.h>
#include <mem/Global.h>
#include <misc/Log.h>

#include <render/infos/DebugRenderInfo.h>
#include <render/Colors.h>

#include "WorldGrid.h"
#include "GameState.h"


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

void PathFinding::debugRender() {
	switch (this->stage) {
		case 0:
		{
			for (auto [k, v] : this->visited) {
				auto kk = k;
				glm::vec2 p = std::bit_cast<glm::ivec2>(kk);

				Global<render::DebugRenderInfo>->world.addBox(
					p + 0.2f,
					p + 0.8f,
					colors::green
				);
			}

			float size = 0.9f;
			for (auto p : this->searched) {
				Global<render::DebugRenderInfo>->world.addBox(
					p + size,
					p + 1.0f - size,
					colors::yellow
				);
				size -= 0.05f;
			}

			size = 1.0f;
			for (glm::vec2 p : this->waypoints) {
				Global<render::DebugRenderInfo>->world.addBox(
					p + size,
					p + 1.0f - size,
					colors::blue
				);
			}
			break;
		}
		case 1:
		{
			for (glm::vec2 p : this->path) {

				Global<render::DebugRenderInfo>->world.addBox(
					p + 0.2f,
					p + 0.8f,
					colors::green
				);
			}

			for (glm::vec2 p : this->prunedPath) {
				Global<render::DebugRenderInfo>->world.addBox(
					p + 0.2f,
					p + 0.8f,
					colors::cyan
				);
			}

			float size = 1.0f;
			for (glm::vec2 p : this->waypoints) {
				Global<render::DebugRenderInfo>->world.addBox(
					p + size,
					p + 1.0f - size,
					colors::blue
				);
			}

			{
				if (!this->waypoints.empty()) {
					glm::vec2 p = this->waypoints.back();
					Global<render::DebugRenderInfo>->world.addBox(
						p + size,
						p + 1.0f - size,
						colors::yellow
					);
				}
			}



			size = 0.2f;
			for (glm::vec2 p : this->newWaypoints) {
				Global<render::DebugRenderInfo>->world.addBox(
					p + size,
					p + 1.0f - size,
					colors::red
				);
			}

			break;
		}
		case 2:
		{
			float size = 0.7f;
			for (glm::vec2 p : this->finalPath) {
				Global<render::DebugRenderInfo>->world.addBox(
					p + size,
					p + 1.0f - size,
					colors::red
				);
			}
			size = 0.2f;
			for (glm::vec2 p : this->newWaypoints) {
				Global<render::DebugRenderInfo>->world.addBox(
					p + size,
					p + 1.0f - size,
					colors::red
				);
			}


			if (this->F.get() != nullptr) {
				for (auto [k, v] : this->F->visited) {
					auto kk = k;
					glm::vec2 p = std::bit_cast<glm::ivec2>(kk);

					Global<render::DebugRenderInfo>->world.addBox(
						p + 0.2f,
						p + 0.8f,
						colors::green
					);
				}

				size = 0.9f;
				for (auto p : this->F->searched) {
					Global<render::DebugRenderInfo>->world.addBox(
						p + size,
						p + 1.0f - size,
						colors::yellow
					);
					size -= 0.05f;
				}

				size = 1.0f;
				for (glm::vec2 p : this->F->waypoints) {
					Global<render::DebugRenderInfo>->world.addBox(
						p + size,
						p + 1.0f - size,
						colors::blue
					);
				}

				glm::vec2 p1 = this->highlight1;

				Global<render::DebugRenderInfo>->world.addPoint(
					p1 + 0.5f,
					colors::green
				);

				glm::vec2 p2 = this->highlight2;

				Global<render::DebugRenderInfo>->world.addPoint(
					p2 + 0.5f,
					colors::green
				);

			}
			break;
		}
		default:
			break;
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

	auto f = front.front();
	front.pop();

	if (glm::abs(f.winding) > 10) {
		return false;
	}

	auto D = target - f.current;
	auto Dabs = glm::abs(D);
	auto m = glm::max(Dabs.x, Dabs.y);

	if (m == 0) {
		found = true;
		this->stage++;
		this->path = f.path;
		this->waypoints = f.waypoints;
		//this->newWaypoints.push_back(this->target);
		return true;
	}

	D /= m;

	glm::ivec2 p = f.current + D;

	int32_t match = 16 * (D.x + 1) + (D.y + 1);

	size_t dir = 8;
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
			assert(0);
			break;
	}


	if (!f.collided) {
		if (!grid.occupied(p.x, p.y)) {
			this->visited.insert({ std::bit_cast<uint64_t>(p), this->count });
			f.path.push_back(p);
			//this->current = p;
			f.current = p;
			this->front.push(f);
		}
		else {
			auto w = f.winding;
			//this->direction = dir;
			//this->collided = true;
			f.D = norm2(this->target - p);
			f.clockwise = true;
			f.collided = true;
			f.direction = prev(prev(dir));
			f.winding = w + 2;
			this->front.push(f);

			f.D = norm2(this->target - p);
			f.clockwise = false;
			f.winding = w + 2;
			f.direction = next(next(dir));
			this->front.push(f);
		}
		return false;
	}

	if (glm::abs(f.winding) < 2) {
		if (!grid.occupied(p.x, p.y) && !this->visited.count(std::bit_cast<uint64_t>(p))) {
			f.waypoints.push_back(p);

			//this->visited.insert({ std::bit_cast<uint64_t>(p), this->count });
			updateOrInsert<uint64_t, int32_t>(this->visited,
				std::bit_cast<uint64_t>(p),
				this->count,
				[](int32_t v) {return v; }
			);

			//this->direction = dir;
			//this->path.push_back(p);
			//this->collided = false;

			f.direction = dir;
			f.collided = false;
			f.path.push_back(p);
			f.current = p;
			f.D = norm2(this->target - p);

			this->front.push(f);

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

	auto inc = f.clockwise ? prev : next;
	size_t index = (inc(inc(flip(f.direction))));
	for (int32_t i = 0; i < 8; i++) {
		p = f.current + order[index];
		this->searched.push_back(p);

		if (!grid.occupied(p.x, p.y)) {
			this->visited.insert({ std::bit_cast<uint64_t>(p), this->count });

			//this->path.push_back(p);

			//this->winding += i - 1;
			//this->direction = index;

			f.direction = index;
			f.winding += i - 2;
			f.path.push_back(p);
			f.current = p;
			f.D = norm2(this->target - p);
			this->front.push(f);

			Global<misc::Log>->putStreamLine(std::stringstream() << "winding: " << f.winding << " i: " << i);


			return false;
		}
		index = inc(index);
	}


	//for (auto v : vec) {
	//	p = this->current + (rot * v);

	//	if (!this->visited.count(std::bit_cast<uint64_t>(p)) && !grid.occupied(p.x, p.y)) {
	//		this->visited.insert({ std::bit_cast<uint64_t>(p), this->path.size() });

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

		std::reverse(this->newWaypoints.begin(), this->newWaypoints.end());
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
		auto start_ = this->newWaypoints.back();
		this->highlight1 = start_;
		this->newWaypoints.pop_back();
		auto target_ = this->newWaypoints.back();
		this->highlight2 = target_;

		Front f{ start_ };
		f.waypoints.push_back(start_);
		F->front.push(f);
		F->target = target_;
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

glm::vec2 GamePosition::getInterpolatedPosition(int32_t tick) const {
	float scale = static_cast<float>(tick - this->startMovement) / static_cast<float>(this->pace);
	scale = glm::clamp(scale, 0.0f, 1.0f);
	return glm::vec2(this->previousPos) + scale * glm::vec2(this->pos - this->previousPos);
}
