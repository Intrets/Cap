#include "GameState.h"

#include <format>
#include <cstdlib>

#include <render/infos/RenderInfo.h>
#include <render/infos/DebugRenderInfo.h>

#include <mem/Locator.h>
#include <render/textures/BlockIDTextures.h>

#include <misc/Timer.h>
#include <misc/Log.h>

#include "Grapher.h"
#include "Merger.h"

#include <fstream>

namespace game
{
	void GameState::addRenderInfo(render::RenderInfo& renderInfo) {
		this->everything.match([&](GamePosition const& pos, GraphicsTile const& tile) {
			renderInfo.tileRenderInfo.addBlitInfo(
				glm::vec4(pos.pos, 1, 1),
				0,
				tile.blockID
			);
			});

		auto& debugRender = Locator<render::DebugRenderInfo>::ref();

		if (misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal()) {
			for (size_t i = 0; i < WORLD_SIZE; i++) {
				for (size_t j = 0; j < WORLD_SIZE; j++) {
					if (this->world->isGrouped({ i,j })) {
						float size = 0.1f;
						glm::vec2 min{ i + size, j + size };
						glm::vec2 max{ i + 1 - size, j + 1 - size };

						auto c1 = colors::uniqueColor(this->world->getGroup({ i,j }));
						//auto c2 = colors::uniqueColor(183874 - this->world->getGroup({ i,j }));

						debugRender.world.addBox(min, max, c1);
						//size = 0.2f;
						//min = { i + size, j + size };
						//max = { i + 1 - size, j + 1 - size };
						//debugRender.world.addBox(min, max, c2);
					}
					//if (this->world->occupied(i, j)) {
					//	Locator<render::DebugRenderInfo>::ref().world.addPoint(i + 0.5f, j + 0.5f);
					//	Locator<render::DebugRenderInfo>::ref().world.addBox(i, j, i + 1.0f, j + 1.0f);
					//}
				}
			}

			this->everything.run([&](Match<Grapher>& e) {
				e.get<Grapher>().debugRender();
				});

			this->everything.match([](Merger& merger) {
				merger.debugRender();
				});

			this->everything.match([&](RandomWalker& walker, GamePosition& pos) {
				auto& merger = this->everything.gets<Merger>().get<Merger>(1);
				auto p = merger.groups[walker.groupTarget].approximation;

				debugRender.world.addLine(
					p + 0.5f,
					glm::vec2(pos.pos) + 0.5f
				);
				});

		}
	}

	void GameState::runTick() {
		if (this->tick == 30) {
			this->everything.match([&](RandomWalker& walker, GamePosition& pos) {
				auto currentGroup = this->world->getGroup(pos.pos);
				walker.groupTarget = currentGroup;
				walker.indexTarget = 0;
				});
		}
		if (this->tick > 30 && this->tick % 10 == 0) {
			this->everything.match([&](RandomWalker& walker, GamePosition& pos) {
				auto& merger = this->everything.gets<Merger>().get<Merger>(1);
				auto currentGroup = this->world->getGroup(pos.pos);

				if (currentGroup == walker.groupTarget) {
					auto targets = merger.groups[currentGroup].neighbours.size();
					int32_t target = rand() % targets;

					walker.groupTarget = merger.groups[currentGroup].neighbours[target].group;
					walker.indexTarget = target;
				}

				auto d = this->world->getDirection(pos.pos, walker.indexTarget);

				pos.pos += d;
				});
		}

		this->everything.run([&](Match<Locomotion, GamePosition, Target>& e) {
			if (e.get<Locomotion>().cooldown != 0) {
				e.get<Locomotion>().cooldown--;
			}
			else {
				auto& target = e.get<Target>();

				if (target.path.empty()) {
					e.remove<Target>();
				}
				else {
					e.get<Locomotion>().cooldown = e.get<Locomotion>().fitness;

					auto newPos = e.get<Target>().path.front();
					e.get<Target>().path.pop_front();

					if (this->empty(newPos)) {
						this->moveInWorld(e.get<GamePosition>().pos, newPos);
						e.get<GamePosition>().pos = newPos;
					}
				}
			}
			});

		if (this->tick % 1 == 0) {
			this->everything.match([&](Grapher& grapher) {
				while (!grapher.step(*this->world));
				});
		}

		this->everything.match([&](Merger& merger) {
			auto& grapher = this->everything.gets<Grapher>().get<Grapher>(1);
			Locator<misc::Timer>::ref().newTiming("Merge");
			if (grapher.finished) {
				if (!grapher.groups.empty()) {
					merger.initialize(grapher.groups, *this->world);
					auto& log = Locator<misc::Log>::ref();
					log.putLine(std::format("number of regions: {}", merger.groups.size()));
					int32_t smallCount = 0;
					for (auto& group : merger.groups) {
						if (group.neighbours.size() < 5) {
							smallCount++;
						}
						else {
							log.putLine(std::format("group {}, neighbour count {}", group.group, group.neighbours.size()));
						}
					}
					if (smallCount == merger.groups.size()) {
						log.putLine("all groups have less than 5 neighbours");
					}
					else {
						log.putLine(std::format("... and {} groups with less than 5 neighbours", smallCount));
					}
					grapher.groups.clear();
				}
				else {
					//if (this->tick % 60 == 0) {
					//	merger.mergeStep(*this->world);
					//}
				}
			}
			Locator<misc::Timer>::ref().endTiming("Merge");
			});

		//this->everything.run([&](Match<Spawner, GamePosition>& e) {
		//	auto p = this->everything.make();
		//	p.add<GamePosition>();
		//	p.add<GraphicsTile>();
		//	p.add<Locomotion>();

		//	p.get<GamePosition>().pos = e.get<GamePosition>().pos + glm::ivec2(1, 0);
		//	p.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
		//	});


		this->everything.collectRemoved();

		this->tick++;
	}

	bool GameState::empty(glm::ivec2 p) {
		return this->world->empty(p);
	}

	void GameState::removeFromWorld(glm::ivec2 pos) {
		this->world->remove(pos);
	}

	void GameState::moveInWorld(glm::ivec2 from, glm::ivec2 to) {
		assert(this->world->grid[to.x][to.y] == 0);

		this->world->grid[to.x][to.y] = this->world->grid[from.x][from.y];
		this->world->grid[from.x][from.y] = 0;
	}

	void GameState::placeInWorld(SizeAlias index, glm::ivec2 pos) {
		assert(this->world->grid[pos.x][pos.y] == 0);
		this->everything.add<GamePosition>(index, pos);
		this->world->place(index, pos);
	}

	void GameState::placeInWorld(WeakObject& obj, glm::ivec2 pos) {
		this->placeInWorld(obj.index, pos);
	}

	void GameState::placeInWorld(UniqueObject& obj, glm::ivec2 pos) {
		this->placeInWorld(obj.index, pos);
	}

	void GameState::init() {
		{
			auto p = this->everything.make();
			p.add<Grapher>();
		}
		{
			auto const place = [&](int i, int j) {
				if (this->world->occupied(i, j)) {
					return;
				}
				auto p2 = this->everything.make();
				this->placeInWorld(p2, { i, j });
				p2.add<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			};


			auto const placeWall = [&](glm::ivec2 from, glm::ivec2 to) {
				glm::vec2 diff = to - from;

				if (diff.x < 0 || diff.y < 0) {
					std::swap(from, to);
					diff = to - from;
				}

				float D = glm::max(glm::abs(diff.x), glm::abs(diff.y));

				glm::vec2 step = diff / D;

				for (size_t i = 0; i <= D; i++) {
					glm::ivec2 pos = glm::vec2(from) + static_cast<float>(i) * step;

					place(pos.x, pos.y);
				}
			};

			//placeWall({ 2, 10 }, { 10, 10 });
			//placeWall({ 10, 3 }, { 10, 10 });

			//placeWall({ 2, 6 }, { 2, WORLD_SIZE - 2 });

			//placeWall({ 1, 1 }, { 10, 1 });

			//placeWall({ 30, 33 }, { 30, 40 });
			//placeWall({ 30, 40 }, { WORLD_SIZE - 2, 40 });
			//placeWall({ 40, 30 }, { 40, 40 });
			//placeWall({ 40, 30 }, { 33, 30 });

			//placeWall({ 1, 20 }, { WORLD_SIZE - 4, 20 });

			//placeWall({ 1, 35 }, { 10, 35 });
			//placeWall({ 12, 35 }, { 35, 35 });

			//placeWall({ 20, 35 }, { 20, 23 });

			//place(41, 30);

			//for (int32_t i = 0; i < 000; i++) {
			//	int x = 1 + rand() % (WORLD_SIZE - 5);
			//	int y = 1 + rand() % (WORLD_SIZE - 5);
			//	if (x < 30 && y < 30) {
			//		continue;
			//	}
			//	place(x, y);
			//}

			//for (int32_t i = 5; i < 20; i++) {
			//	place(10 + i, 20 - i);

			//}
		}
		{
			auto p = this->everything.make();
			p.add<Merger>();
		}
		for (size_t i = 0; i < 1; i++) {
			auto p = this->everything.make();
			p.add<GraphicsTile>();
			p.add<GamePosition>(glm::ivec2(2, 2));
			p.add<RandomWalker>();

			p.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");
		}

		for (size_t i = 1; i < WORLD_SIZE - 1; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				size_t j = 0;
				auto p2 = this->everything.make();
				p2.add<GraphicsTile>();
				this->placeInWorld(p2, { i, j });

				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}

		for (size_t i = 1; i < WORLD_SIZE - 1; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				int j = WORLD_SIZE - 1;
				auto p2 = this->everything.make();
				this->placeInWorld(p2, { i, j });
				p2.add<GraphicsTile>();

				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			size_t i = 0;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto p2 = this->everything.make();
				this->placeInWorld(p2, { i, j });
				p2.add<GraphicsTile>();

				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			int i = WORLD_SIZE - 1;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto p2 = this->everything.make();
				this->placeInWorld(p2, { i, j });
				p2.add<GraphicsTile>();

				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}

	}

	//float Concept::value(SignatureAlias const& signature) {
	//	float result = 0.0f;

	//	for (auto const& essence : this->essences) {
	//		if (signature.contains(essence.signature)) {

	//			result += essence.value;
	//		}
	//	}

}
