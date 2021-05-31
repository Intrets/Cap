#include "Merger.h"

#include <set>
#include <queue>
#include <map>
#include <unordered_set>

#include "WorldGrid.h"

#include <mem/Locator.h>
#include <render/infos/DebugRenderInfo.h>

void Merger::initialize(std::vector<glm::ivec2>& seedPoints, game::WorldGrid& grid) {
	for (auto p : seedPoints) {
		Group group;
		group.group = grid.getGroup(p);

		const auto cmp = [](glm::ivec2 left, glm::ivec2 right) { return *reinterpret_cast<int64_t*>(&left) < *reinterpret_cast<int64_t*>(&right); };
		const auto hash = [](glm::ivec2 p) { return std::hash<int64_t>{}(*reinterpret_cast<int64_t*>(&p)); };
		const auto equal = [](glm::ivec2 left, glm::ivec2 right) { return left == right; };
		//std::set<glm::ivec2, decltype(cmp)> visited(cmp);
		std::unordered_set<glm::ivec2, decltype(hash), decltype(equal)> visited(8, hash, equal);
		std::queue<glm::ivec2> open;

		//std::multimap<int32_t, glm::ivec2> borders;
		std::vector<std::pair<int32_t, glm::ivec2>> borders;

		open.push(p);

		while (!open.empty()) {
			auto point = open.front();
			open.pop();

			const std::array<glm::ivec2, 4> D{
				glm::ivec2(1,0),
				glm::ivec2(0,1),
				glm::ivec2(-1,0),
				glm::ivec2(0,-1)
			};

			for (auto d : D) {
				auto const point2 = point + d;

				if (visited.count(point2) != 0) {
					continue;
				}

				auto point2Group = grid.getGroup(point2);
				if (point2Group != 0 && point2Group != group.group) {
					borders.push_back({ point2Group, point2 });
					continue;
				}

				if (point2Group != 0) {
					visited.insert(point2);
					open.push(point2);
				}
			}
		}

		std::sort(
			borders.begin(),
			borders.end(),
			[](auto const& left, auto const& right) {return left.first < right.first; }
		);

		if (!borders.empty()) {
			group.neighbours.emplace_back();
			group.neighbours.back().group = borders.back().first;

			for (auto const& [g, point] : borders) {
				if (borders.back().first != g) {
					group.neighbours.emplace_back();
					group.neighbours.back().group = g;
				}

				group.neighbours.back().front.push_back(point);
			}
		}

		this->groups.push_back(group);
	}
}

void Merger::debugRender() {
	auto& debugRender = Locator<render::DebugRenderInfo>::ref();

	for (auto& group : this->groups) {
		for (auto& neighbour : group.neighbours) {
			for (auto p : neighbour.front) {
				debugRender.world.addPoint(glm::vec2(p) + glm::vec2(0.5f), colors::uniqueColor(group.group));
			}
		}
	}
}
