#include "GameObject.h"

#include "GameState.h"

#include <wglm/gtc/matrix_integer.hpp>
#include <misc/Misc.h>

bool PathFinding::step(game::WorldGrid& grid) {
	if (this->i++ % 20 != 0) {
		return false;
	}
	auto D = target - current;
	auto m = glm::max(D.x, D.y);

	if (m == 0) {
		return true;
	}

	D /= m;

	int32_t match = 16 * (D.x + 1) + (D.y + 1);

	static std::vector<glm::ivec2> diag = {
		{1,1},
		{1,0},
		{0,1},
		{1, -1},
		{-1, 1},
		{-1,0},
		{0,-1},
		{-1,-1}
	};

	static std::vector<glm::ivec2> straight = {
		{1,0},
		{1,1},
		{1,-1},
		{0,1},
		{0,-1},
		{-1,1},
		{-1,-1},
		{-1,0}
	};

	glm::highp_imat2 rot;

	if (idot(this->previousDirection, D) < 0) {
		rot = glm::highp_imat2(0, -1, -1, 0);
	}

	glm::highp_imat2 rot2;
	bool s = true;

	switch (match) {
		case 0x22:
			s = false;
			[[fallthrough]];
		case 0x21:
			rot2 = {
				{1,0},
				{0,1} };
			break;
		case 0x20:
			s = false;
			[[fallthrough]];
		case 0x10:
			rot2 = {
				{0,-1},
				{1,0} };
			break;
		case 0x00:
			s = false;
			[[fallthrough]];
		case 0x01:
			rot2 = {
				{-1,0},
				{0,-1} };
			break;
		case 0x02:
			s = false;
			[[fallthrough]];
		case 0x12:
			rot2 = {
				{0,1},
				{-1,0} };
			break;
		default:
			break;
	}

	rot *= rot2;

	auto p = this->current + D;

	//if (!this->visited.count(*reinterpret_cast<uint64_t*>(&p)) && !grid.occupied(p.x, p.y)) {
	//	this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), 1 });

	//	this->path.push_back(p);
	//	this->current = p;
	//	this->previousDirection = D;

	//	return false;
	//}

	//p = this->current + prejviousDirection;

	//if (!this->visited.count(*reinterpret_cast<uint64_t*>(&p)) && !grid.occupied(p.x, p.y)) {
	//	this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), 1 });

	//	this->path.push_back(p);
	//	this->current = p;

	//	return false;
	//}


	auto& vec = s ? straight : diag;

	for (auto v : vec) {
		p = this->current + (rot * v);

		if (!this->visited.count(*reinterpret_cast<uint64_t*>(&p)) && !grid.occupied(p.x, p.y)) {
			this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), this->path.size() });

			this->current = p;
			this->path.push_back(p);

			this->previousDirection = rot * v;
			return false;
		}


		//if (!this->visited.count(*reinterpret_cast<uint64_t*>(&p))) {
		//	if (!grid.occupied(p.x, p.y)) {
		//		this->visited.insert({ *reinterpret_cast<uint64_t*>(&p), this->path.size() });

		//		this->path.push_back(p);
		//		this->current = p;
		//		this->previousDirection = rot * v;

		//		return false;
		//	}
		//}
		//else {
		//	auto it = this->visited.find(*reinterpret_cast<uint64_t*>(&p));

		//	if (it->second > 0 && this->path.size() > 20 + it->second) {
		//		this->path.resize(it->second);
		//		this->current = this->path.back();

		//		return false;
		//	}
		//}

	}

	//this->path.pop_back();
	//this->current = this->path.back();

	return false;
}
