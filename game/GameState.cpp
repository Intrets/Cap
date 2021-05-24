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

			this->everything.run([&](game::Match<GamePosition, Target>& e) {
				Locator<render::DebugRenderInfo>::ref().world.addLine(e.get<GamePosition>().pos, e.get<Target>().pos);
				});
		}

	}

	void GameState::runTick() {
		this->everything.run([&](Match<Locomotion, GamePosition, Target>& e) {
			if (e.get<Locomotion>().cooldown != 0) {
				e.get<Locomotion>().cooldown--;
			}
			else {
				e.get<Locomotion>().cooldown = e.get<Locomotion>().fitness;
				auto& pos = e.get<GamePosition>().pos;

				glm::vec2 diff = e.get<Target>().pos - pos;
				float D = glm::max(diff.x, diff.y);

				if (D == 0.0f) {
					this->everything.removeComponent<Target>(e.obj.index);
					return;
				}

				glm::ivec2 m = diff / D;

				auto newPos = pos + m;

				if (this->empty(newPos)) {
					this->removeFromWorld(e.get<GamePosition>().pos);
					e.get<GamePosition>().pos = newPos;
					this->placeInWorld(e.obj, newPos);
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


		this->tick++;
	}

	bool GameState::empty(glm::ivec2 p) {
		return this->world->empty(p);
	}

	void GameState::removeFromWorld(glm::ivec2 pos) {
		this->world->remove(pos);
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

	void GameState::placeInWorld(ManagedObject& obj, glm::ivec2 pos) {
		this->placeInWorld(obj.index, pos);
	}

	GameState::GameState() {
		{
			auto p = this->everything.make();
			p.add<GraphicsTile>();
			p.add<Locomotion>();
			p.add<GamePosition>();
			p.add<Brain>();
			p.add<Target>(glm::ivec2(50, 20));

			this->placeInWorld(p, { 2, 2 });

			p.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");
		}

		for (size_t i = 1; i < WORLD_SIZE - 1; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				size_t j = 0;
				auto p2 = this->everything.make();
				p2.add<GamePosition>();
				p2.add<GraphicsTile>();

				this->placeInWorld(p2, { i,j });
				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}

		for (size_t i = 1; i < WORLD_SIZE - 1; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				int j = WORLD_SIZE - 1;
				auto p2 = this->everything.make();
				p2.add<GamePosition>();
				p2.add<GraphicsTile>();

				this->placeInWorld(p2, { i,j });
				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			size_t i = 0;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto p2 = this->everything.make();
				p2.add<GamePosition>();
				p2.add<GraphicsTile>();

				this->placeInWorld(p2, { i,j });
				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			int i = WORLD_SIZE - 1;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto p2 = this->everything.make();
				p2.add<GamePosition>();
				p2.add<GraphicsTile>();

				this->placeInWorld(p2, { i,j });
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
