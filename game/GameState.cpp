#include "GameState.h"

#include <render/infos/RenderInfo.h>
#include <render/infos/DebugRenderInfo.h>

#include <mem/Locator.h>
#include <render/textures/BlockIDTextures.h>

#include <cstdlib>

#include <misc/Timer.h>

namespace game
{
	void GameState::addRenderInfo(render::RenderInfo& renderInfo) {
		this->everything.run([&](game::Match<GamePosition, GraphicsTile>& e) {
			renderInfo.tileRenderInfo.addBlitInfo(
				glm::vec4(e.get<GamePosition>().pos, 1, 1),
				0,
				e.get<GraphicsTile>().blockID
			);
			});

		if (misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal()) {
			//for (size_t i = 0; i < WORLD_SIZE; i++) {
			//	for (size_t j = 0; j < WORLD_SIZE; j++) {
			//		if (this->world->occupied(i, j)) {
			//			Locator<render::DebugRenderInfo>::ref().world.addPoint(i + 0.5f, j + 0.5f);
			//			Locator<render::DebugRenderInfo>::ref().world.addBox(i, j, i + 1.0f, j + 1.0f);
			//		}
			//	}
			//}

		}

	}

	void GameState::runTick() {
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

	GameState::GameState() {
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

			placeWall({ 2, 10 }, { 10, 10 });
			placeWall({ 10, 3 }, { 10, 10 });

			placeWall({ 2, 6 }, { 2, WORLD_SIZE - 2 });

			placeWall({ 1, 1 }, { 10, 1 });

			placeWall({ 30, 33 }, { 30, 40 });
			placeWall({ 30, 40 }, { WORLD_SIZE - 2, 40 });
			placeWall({ 40, 30 }, { 40, 40 });
			placeWall({ 40, 30 }, { 33, 30 });

			placeWall({ 1, 20 }, { WORLD_SIZE - 4, 20 });

			placeWall({ 1, 35 }, { 10, 35 });
			placeWall({ 12, 35 }, { 35, 35 });

			placeWall({ 20, 35 }, { 20, 23 });

			place(41, 30);

			//for (size_t i = 0; i < 200; i++) {
			//	int x = 1 + rand() % (WORLD_SIZE - 5);
			//	int y = 1 + rand() % (WORLD_SIZE - 5);

			//	place(x, y);

			//}



		}
		{
			auto p = this->everything.make();
			p.add<GraphicsTile>();
			p.add<Locomotion>();
			p.add<Brain>();
			p.add<PathFinding>();

			//glm::ivec2 target = { WORLD_SIZE - 3, WORLD_SIZE - 3 };
			glm::ivec2 target = { WORLD_SIZE - 3, WORLD_SIZE - 3 };
			glm::ivec2 start = { 2,2 };

			//p.get<PathFinding>().current = { 2, 2 };
			Front F{ start };
			F.waypoints.push_back(start);
			p.get<PathFinding>().front.push(F);
			p.get<PathFinding>().target = target;
			p.get<PathFinding>().start = start;

			//p.add<Target>(std::deque<glm::ivec2>{
			//	glm::ivec2(10, 2),
			//		glm::ivec2(10, 3),
			//		glm::ivec2(10, 4),
			//		glm::ivec2(10, 5),
			//		glm::ivec2(10, 6),
			//		glm::ivec2(11, 6),
			//		glm::ivec2(12, 5),
			//		glm::ivec2(11, 5),
			//		glm::ivec2(10, 6)
			//});

			//this->placeInWorld(p, { 2, 2 });

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
}
