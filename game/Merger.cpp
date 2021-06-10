#include "Merger.h"

#include <set>
#include <queue>
#include <map>
#include <unordered_set>
#include <array>

#include "WorldGrid.h"

#include <mem/Locator.h>
#include <render/infos/DebugRenderInfo.h>

void Merger::initialize(std::vector<glm::ivec2>& seedPoints, game::WorldGrid& grid) {
	this->groups.push_back({});
	for (auto p : seedPoints) {
		Group group;
		group.group = grid.getGroup(p);
		group.seed = p;

		const auto cmp = [](glm::ivec2 left, glm::ivec2 right) { return std::bit_cast<int64_t>(left) < std::bit_cast<int64_t>(right); };
		const auto hash = [](glm::ivec2 p) { return std::hash<int64_t>{}(std::bit_cast<int64_t>(p)); };
		const auto equal = [](glm::ivec2 left, glm::ivec2 right) { return left == right; };
		//std::set<glm::ivec2, decltype(cmp)> visited(cmp);
		std::unordered_set<glm::ivec2, decltype(hash), decltype(equal)> visited(8, hash, equal);
		std::queue<glm::ivec2> open;

		//std::multimap<int32_t, glm::ivec2> borders;
		std::vector<std::pair<int32_t, glm::ivec2>> borders;
		group.size = 1;

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
					group.size++;
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
			group.neighbours.back().group = borders.front().first;

			for (auto const& [g, point] : borders) {
				if (group.neighbours.back().group != g) {
					group.neighbours.emplace_back();
					group.neighbours.back().group = g;
				}

				group.neighbours.back().front.push_back(point);
			}
		}

		this->groups.push_back(group);
	}

	//this->mergeGroups(grid, 1, 2);
	this->mergeStep(grid);

	for (auto& group : this->groups) {
		if (!group.neighbours.empty()) {
			this->nonEmptyGroups.push_back(group.group);
		}
	}

	for (auto g : this->nonEmptyGroups) {
		std::vector<int32_t> neighbours;
		glm::vec2 total{ 0, 0 };
		int32_t count = 0;
		for (auto& neighbour : this->groups[g].neighbours) {
			neighbours.push_back(neighbour.group);
			for (auto p : neighbour.front) {
				total += p;
				count++;
			}
		}

		this->groups[g].approximation = total / static_cast<float>(count);
	}

	for (auto g : this->nonEmptyGroups) {
		this->fillPaths(grid, g);
	}
}

bool Merger::hasNeighbour(int32_t group, int32_t neighbour) {
	for (auto& n : this->groups[group].neighbours) {
		if (neighbour == n.group) {
			return true;
		}
	}
	return false;
}

void Merger::mergeStep(game::WorldGrid& grid) {
	auto const validSizes = [](int32_t size1, int32_t size2) {
		return size1 < 10 || size2 < 10 || (size1 + size2 < 13 * 13 + 10);
	};

	for (size_t i = 0; i < 10; i++) {
		for (auto& group : this->groups) {
			if (group.neighbours.size() == 1) {
				if (!validSizes(group.size, this->groups[group.neighbours.front().group].size)) {
					continue;
				}
				else {
					this->mergeGroups(grid, group.group, group.neighbours.front().group);
				}
			}
			else if (group.neighbours.size() == 2) {
				for (auto& neighbour : group.neighbours) {
					if (!validSizes(group.size, this->groups[neighbour.group].size)) {
						continue;
					}
					else {
						this->mergeGroups(grid, group.group, neighbour.group);
						break;
					}
				}
			}
			else if (group.neighbours.size() < 5) {
				for (auto& neighbour : group.neighbours) {
					if (!validSizes(group.size, this->groups[neighbour.group].size)) {
						continue;
					}
					else if (this->hasNeighbour(neighbour.group, group.group)) {

						this->mergeGroups(grid, group.group, neighbour.group);
						break;
					}
				}
			}
		}
	}
}

int32_t Merger::getNeighbourCount(int32_t group) {
	return static_cast<int32_t>(this->groups[group].neighbours.size());
}

void Merger::mergeGroups(game::WorldGrid& grid, int32_t group, int32_t into) {
	assert(group != 0 && into != 0);
	grid.replaceGroup(this->groups[group].seed, group, into);
	this->groups[into].size += this->groups[group].size;

	std::vector<Neighbour> newNeighbours;
	auto const insert = [&](Neighbour& newNeighbour) {
		auto f = [&](Neighbour& n) {return n.group == newNeighbour.group; };
		auto it = std::find_if(newNeighbours.begin(), newNeighbours.end(), f);

		if (it != newNeighbours.end()) {
			it->front.insert(it->front.end(), newNeighbour.front.begin(), newNeighbour.front.end());
		}
		else {
			newNeighbours.push_back(newNeighbour);
		}
	};

	for (auto& neighbour : this->groups[group].neighbours) {
		if (neighbour.group == group || neighbour.group == into) {
			continue;
		}
		else {
			newNeighbours.push_back(neighbour);

			auto& toChange = this->groups[neighbour.group];

			auto groupFound = std::find_if(
				toChange.neighbours.begin(),
				toChange.neighbours.end(),
				[&](auto& n) {
					return n.group == group;
				});

			if (groupFound != toChange.neighbours.end()) {
				auto intoFound = std::find_if(
					toChange.neighbours.begin(),
					toChange.neighbours.end(),
					[&](auto& n) {
						return n.group == into;
					});

				if (intoFound != toChange.neighbours.end()) {
					intoFound->front.insert(
						intoFound->front.begin(),
						groupFound->front.begin(),
						groupFound->front.end()
					);

					toChange.neighbours.erase(groupFound);
				}
				else {
					groupFound->group = into;
				}
			}
		}
	}

	for (auto& neighbour : this->groups[into].neighbours) {
		if (neighbour.group == group || neighbour.group == into) {
			continue;
		}
		else {
			insert(neighbour);
		}
	}

	this->groups[into].neighbours = newNeighbours;
	this->groups[group] = {};

}

void Merger::debugRender() {
	auto& debugRender = Locator<render::DebugRenderInfo>::ref();

	for (auto& group : this->groups) {
		//if (group.neighbours.size() < 10) {
		//	continue;
		//}
		for (auto& neighbour : group.neighbours) {
			for (auto p : neighbour.front) {
				debugRender.world.addPoint(glm::vec2(p) + glm::vec2(0.5f), colors::uniqueColor(group.group));
			}
		}
	}

	for (auto& group : this->nonEmptyGroups) {
		debugRender.world.addPoint(this->groups[group].approximation + 0.5f, colors::uniqueColor(group));
		for (auto& neighbour : this->groups[group].neighbours) {
			debugRender.world.addLine(
				this->groups[group].approximation + 0.5f,
				this->groups[neighbour.group].approximation + 0.5f,
				colors::white
			);
		}
	}
}

void Merger::fillPaths(game::WorldGrid& grid, int32_t g) {
	auto& group = this->groups[g];

	int32_t index = 0;

	for (auto& neighbour : group.neighbours) {
		std::queue<glm::ivec2> open;
		for (auto p : neighbour.front) {
			open.push(p);
		}

		while (!open.empty()) {
			auto point = open.front();
			open.pop();

			for (size_t i_ = 0; i_ < 4; i_++) {
				size_t i = i_ * 2 + 1;
				auto p = point + game::getDirectionFromIndex(i);

				if (grid.empty(p) && grid.getGroup(p) == g && !grid.hasDirection(p, index)) {
					grid.setDirection(p, index, (i + 4) % 8);
					open.push(p);
				}
			}
		}
		index++;
	}
}
