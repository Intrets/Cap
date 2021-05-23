#include "GameState.h"

#include <render/infos/RenderInfo.h>

#include <mem/Locator.h>
#include <render/textures/BlockIDTextures.h>

#include <cstdlib>

#include <misc/Timer.h>

namespace game
{
	void GameState::addRenderInfo(render::RenderInfo& renderInfo) {
		renderInfo.tileRenderInfo.addBlitInfo(
			glm::vec4(0),
			0,
			0
		);

		this->everything.run([&](game::Match<GamePosition, GraphicsTile>& e) {
			//e.get<GamePosition>().pos += glm::ivec2(1, 0);
			//e.get<GraphicsTile>().blockID++;
			renderInfo.tileRenderInfo.addBlitInfo(
				glm::vec4(e.get<GamePosition>().pos, 1, 1),
				0,
				e.get<GraphicsTile>().blockID
			);
			});
	}

	void GameState::runTick() {
		this->everything.run([](Match<Locomotion, GamePosition>& e) {
			if (e.get<Locomotion>().cooldown != 0) {
				e.get<Locomotion>().cooldown--;
			}
			else {
				e.get<Locomotion>().cooldown = e.get<Locomotion>().fitness;
				e.get<GamePosition>().pos += glm::ivec2((rand() % 3) - 1, (rand() % 3) - 1);
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

	GameState::GameState() {
		{
			{
				auto p2 = this->everything.make();
				p2.add<GamePosition>(glm::ivec2(5, 5));
				p2.add<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}

			{
				auto p = this->everything.make();

				p.add<GamePosition>();
				p.add<GraphicsTile>();
				p.add<Spawner>();

				p.get<GamePosition>().pos = glm::ivec2(5, 6);
				p.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("s_block.dds");
			}

			{
				auto p = this->everything.make();
				//p.add<Brain>();
				//p.add<Vicinity>();
				p.add<GraphicsTile>();
				p.add<Locomotion>();
				p.add<GamePosition>();

				p.get<GamePosition>().pos = { 5, 5 };

				p.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");

				//p.get<Locomotion>().fitness = 10;
			}

			//Action recallFood;
			//recallFood.requirements = {};
			//SignatureAlias foodSignature;
			//foodSignature.set(GAMEOBJECT_COMPONENT::NUTRITION).set(GAMEOBJECT_COMPONENT::GAMEPOSITION);
			//recallFood.results = { foodSignature };
			////recallFood.runFunction = [](Object* obj) -> ActionResult
			////{
			////	ActionResult result{};

			////	return result;
			////};
		}

		for (size_t i = 0; i < WORLD_SIZE; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				size_t j = 0;
				auto p2 = this->everything.make();
				p2.add<GamePosition>();
				p2.add<GraphicsTile>();

				p2.get<GamePosition>().pos = glm::ivec2(i, j);
				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
				//this->world[i][j] = ref;
			}
		}

		for (size_t i = 0; i < WORLD_SIZE; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				int j = WORLD_SIZE - 1;
				auto p2 = this->everything.make();
				p2.add<GamePosition>();
				p2.add<GraphicsTile>();

				p2.get<GamePosition>().pos = glm::ivec2(i, j);
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

				p2.get<GamePosition>().pos = glm::ivec2(i, j);
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

				p2.get<GamePosition>().pos = glm::ivec2(i, j);
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

	//	return result;
	//}
}
