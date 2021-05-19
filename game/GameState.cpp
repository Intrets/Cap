#include "GameState.h"

#include <render/infos/RenderInfo.h>

#include <mem/Locator.h>
#include <render/textures/BlockIDTextures.h>

#include <cstdlib>

#include <misc/Timer.h>

namespace game
{
	void GameState::addRenderInfo(render::RenderInfo& renderInfo) {
		SignatureAlias renderSignature;
		renderSignature.set(GAMEOBJECT_COMPONENT::GAMEPOSITION);
		renderSignature.set(GAMEOBJECT_COMPONENT::GRAPHICSTILE);

		this->everything->run([&](Match<GamePosition, GraphicsTile> e) {
			renderInfo.tileRenderInfo.addBlitInfo(
				glm::vec4(e.get<GamePosition>().pos, 1, 1),
				0,
				e.get<GraphicsTile>().blockID
			);
			});
	}

	void GameState::runTick() {
		std::cout << '\n';

		SignatureAlias conciousSignature;
		conciousSignature.set(GAMEOBJECT_COMPONENT::BRAIN);
		conciousSignature.set(GAMEOBJECT_COMPONENT::POSSESSION);
		conciousSignature.set(GAMEOBJECT_COMPONENT::VICINITY);

		SignatureAlias foodSignature;
		foodSignature.set(GAMEOBJECT_COMPONENT::NUTRITION);

		SignatureAlias friendlySignature;
		friendlySignature.set(GAMEOBJECT_COMPONENT::BRAIN);

		SignatureAlias brainSignature;
		brainSignature.set(GAMEOBJECT_COMPONENT::BRAIN);

		SignatureAlias locomotionSignature;
		locomotionSignature.set(GAMEOBJECT_COMPONENT::LOCOMOTION);
		locomotionSignature.set(GAMEOBJECT_COMPONENT::GAMEPOSITION);

		Concept foodConcept;

		foodConcept.essences.push_back({ 10.0f, foodSignature });
		foodConcept.essences.push_back({ -10.0f, friendlySignature });

		this->everything->run([](Match<GamePosition, Locomotion>& e) {
			if (e.get<Locomotion>().cooldown != 0) {
				e.get<Locomotion>().cooldown--;
			}
			else {
				e.get<Locomotion>().cooldown = e.get<Locomotion>().fitness;
				e.get<GamePosition>().pos += glm::ivec2((rand() % 3) - 1, (rand() % 3) - 1);
			}
			});

		//for (auto& [h, obj] : this->refMan.data) {
		//	// Advanced test
		//	if (obj.get()->signature.contains(locomotionSignature)) {
		//		auto& loc = obj.get()->locomotion();
		//		auto& pos = obj.get()->gamePosition();

		//		if (loc.cooldown != 0) {
		//			loc.cooldown--;
		//		}
		//		else if (loc.target.has_value()) {
		//			auto d = loc.target.value() - pos.pos;
		//			std::cout << "d:" << d.x << " " << d.y << "\n";
		//			if (d.x == 0 && d.y == 0) {
		//				loc.target = std::nullopt;
		//			}
		//			else {
		//				int32_t maxD = glm::max(glm::abs(d.x), glm::abs(d.y));

		//				auto mov = glm::sign(d) * (glm::abs(d) / maxD);
		//				std::cout << mov.x << " " << mov.y << "\n";

		//				pos.pos += mov;
		//				loc.cooldown = 120;
		//			}
		//		}
		//	}

		//	if (obj.get()->signature.contains(brainSignature)) {
		//		if (obj.get()->brain().energy > 0) {
		//			obj.get()->brain().energy--;
		//		}
		//	}

		//	if (obj.get()->signature.contains(conciousSignature)) {
		//		auto& brain = obj.get()->brain();
		//		if (!brain.currentAction.has_value()) {
		//			if (brain.energy == 0) {
		//				std::cout << "energy low, seeking food\n";

		//				//for (auto& mem : obj.get()->brain().memory) {
		//				//	for (auto& res : mem.results) {
		//				//		if (foodConcept.value(res) > 1.0f) {
		//				//			brain.currentAction = mem;
		//				//		}
		//				//	}
		//				//}

		//				glm::ivec2 target{ std::rand() % 20, std::rand() % 20 };


		//				Action testAction;
		//				//testAction.runFunction = [target = target](GameObject* obj) -> ActionResult
		//				//{
		//				//	ActionResult result{};

		//				//	auto d = target - obj->gamePosition().pos;

		//				//	if (d.x == 0 && d.y == 0) {
		//				//		obj->brain().energy = 200;
		//				//		result.success = true;
		//				//	}
		//				//	else {
		//				//		int32_t maxD = glm::max(glm::abs(d.x), glm::abs(d.y));

		//				//		auto mov = glm::sign(d) * (glm::abs(d) / maxD);
		//				//		std::cout << mov.x << " " << mov.y << "\n";

		//				//		obj->gamePosition().pos += mov;
		//				//	}
		//				//	return result;
		//				//};

		//				obj.get()->brain().currentAction = testAction;
		//			}
		//		}
		//		else {
		//			//if (brain.currentAction.value().run(obj.get()).success) {
		//			//	brain.currentAction = std::nullopt;
		//			//}
		//		}
		//	}

		//}

		this->tick++;
	}

	GameState::GameState() {
		this->everything = std::make_unique<Everything>();


		{
			//for (size_t j = 1; j < 100; j++) {
			//	for (size_t i = 1; i < WORLD_SIZE - 2; i++) {
			//		auto p = this->everything->makeWeak();

			//		p.addgameposition();
			//		p.gameposition().pos = { j, i };

			//		p.addbrain();
			//		p.addlocomotion();
			//		p.locomotion().fitness = 0;

			//		p.addgraphicstile();
			//		p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");
			//	}
			//}
		}

		{

			auto p = this->everything->makeWeak();
			p.addbrain();
			p.addpossession();
			p.addvicinity();
			p.addgraphicstile();
			p.addlocomotion();
			p.addgameposition();

			p.gameposition().pos = { 5 , 5 };

			p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");

			p.locomotion().fitness = 10;


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


		{
			//auto ref = this->refMan.makeRef<Object>();
			//auto ptr = ref.get();

			//glm::ivec2 pos = { 14, 14 };

			//ptr->signature.set(GAMEOBJECT_COMPONENT::GAMEPOSITION);
			//ptr->gamePosition().pos = pos;
			//this->world[14][14] = ref;

			//ptr->signature.set(GAMEOBJECT_COMPONENT::GRAPHICSTILE);
			//ptr->graphicsTile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("food.dds");
		}


		for (size_t i = 0; i < WORLD_SIZE; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				size_t j = 0;
				auto p = this->everything->makeWeak();
				p.addgameposition();
				p.gameposition().pos = glm::ivec2(i, j);

				p.addgraphicstile();
				p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");

				//this->world[i][j] = ref;
			}
		}

		for (size_t i = 0; i < WORLD_SIZE; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				int j = WORLD_SIZE - 1;
				auto p = this->everything->makeWeak();
				p.addgameposition();
				p.gameposition().pos = glm::ivec2(i, j);

				p.addgraphicstile();
				p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			size_t i = 0;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto p = this->everything->makeWeak();
				p.addgameposition();
				p.gameposition().pos = glm::ivec2(i, j);

				p.addgraphicstile();
				p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			int i = WORLD_SIZE - 1;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto p = this->everything->makeWeak();
				p.addgameposition();
				p.gameposition().pos = glm::ivec2(i, j);

				p.addgraphicstile();
				p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
			}
		}
	}

	float Concept::value(SignatureAlias const& signature) {
		float result = 0.0f;

		for (auto const& essence : this->essences) {
			if (signature.contains(essence.signature)) {

				result += essence.value;
			}
		}

		return result;
	}
}
