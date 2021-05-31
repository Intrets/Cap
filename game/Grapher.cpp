#include "Grapher.h"

#include <render/infos/DebugRenderInfo.h>
#include <vector>

#include <mem/Locator.h>

bool Grapher::incrementLastPos() {
	//this->lastPos.x++;
	//if (this->lastPos.x >= this->region.top.x) {
	//	this->lastPos.x = 0;
	//	this->lastPos.y++;

	//	if (this->lastPos.y >= this->region.top.y) {
	//		return false;
	//	}
	//}
	//return true;
	static int32_t count = 0;
	count++;
	this->lastPos = { rand() % WORLD_SIZE, rand() % WORLD_SIZE };
	return count < WORLD_SIZE* WORLD_SIZE * 10;
}

void Grapher::debugRender() {
	auto& debugRender = Locator<render::DebugRenderInfo>::ref();

	//debugRender.world.addBox(this->region.bot, this->region.top + glm::ivec2(1, 1));
}

bool Grapher::step(game::WorldGrid& grid) {
	const auto isValid = [&](glm::ivec2 pos) {
		return grid.empty(pos) && !grid.isGrouped(pos);
	};

	if (this->finished) {
		return true;
	}

	if (this->fronts.empty()) {
		this->fronts.clear();
		this->currentDepth = 0;
		while (true) {
			if (!this->incrementLastPos()) {
				this->finished = true;
				return true;
			}

			if (isValid(this->lastPos)) {
				grid.setGroup(this->lastPos, ++this->lastGroup);
				this->groups.push_back(this->lastPos);
				for (size_t i = 0; i < 4; i++) {
					Front f;
					f.max = lastPos;
					f.min = lastPos;
					f.size = 1;
					f.direction = (i * 2) + 1;
					this->fronts.push_back(f);
				}

				this->currentBlock = floordiv(this->lastPos, this->blockSize);
				break;
			}
		}
	}
	else {
		bool progress = false;
		for (auto& front : this->fronts) {
			int32_t i = 0;
			int32_t end = front.size;
			glm::ivec2 d = game::getDirectionFromIndex((front.direction + 2) % 8);
			glm::ivec2 forward = game::getDirectionFromIndex(front.direction);
			glm::ivec2 pos = front.min + forward;

			if (floordiv(front.min + forward, this->blockSize) != this->currentBlock || floordiv(front.max + forward, this->blockSize) != this->currentBlock) {
				front.size = 0;
				continue;
			}

			while (i < end && !isValid(pos)) {
				pos += d;
				i++;
			}

			// completely blocked
			if (i == end) {
				//front.size = 0;
				continue;
			}

			auto starti = i;

			while (i < end && isValid(pos)) {
				pos += d;
				i++;
			}

			auto endi = i;

			while (i < end && !isValid(pos)) {
				pos += d;
				i++;
			}

			//if (starti != 0 || endi != end) {
			//	front.size = 0;
			//	continue;
			//}
			if (endi - starti < 5 && endi - starti < currentDepth / 2) {
				//front.size = 0;
				continue;
			}
			if (endi - starti < 2 && currentDepth > 5) {
				//front.size = 0;
				continue;
			}
			if (endi - starti < 2 && currentDepth > 2) {
				//front.size = 0;
				continue;
			}
			//if (endi - starti < 2 && currentDepth > 2) {
			//	//front.size = 0;
			//	continue;
			//}
			//if (endi - starti < 3 && currentDepth > 3) {
			//	//front.size = 0;
			//	continue;
			//}
			if (i != end) {
				front.size = 0;
				continue;
			}
			else {
				this->currentDepth++;
				progress = true;
				auto oldSize = front.size;

				front.size = endi - starti;
				front.max = front.min + (endi - 1) * d + forward;
				front.min = front.min + starti * d + forward;
				for (auto j = 0; j < front.size; j++) {
					grid.setGroup(front.min + j * d, this->lastGroup);
				}
				for (auto& f : this->fronts) {
					f.tryExpand(front.min);
					f.tryExpand(front.max);
				}

				//if (oldSize != front.size) {
				//	this->fronts.clear();
				//	break;
				//}
			}
		}

		this->fronts.erase(
			std::remove_if(
				this->fronts.begin(),
				this->fronts.end(),
				[](auto const& front) { return front.size == 0; }),
			this->fronts.end()
		);

		if (!progress) {
			this->fronts.clear();
		}
	}
	return false;
}

void Grapher::Front::tryExpand(glm::ivec2 p) {
	if (this->size == 0) {
		return;
	}
	glm::ivec2 nextMin = this->min + game::getDirectionFromIndex((direction + 6) % 8);

	if (p == nextMin) {
		min = nextMin;
		size++;
		return;
	}

	glm::ivec2 nextMax = this->max + game::getDirectionFromIndex((direction + 2) % 8);

	if (p == nextMax) {
		max = nextMax;
		size++;
		return;
	}
}

bool Grapher::Region::contains(glm::ivec2 p) {
	return bot.x <= p.x && p.x <= top.x && bot.y <= p.y && p.y <= top.y;
}
