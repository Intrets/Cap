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
		static int cycle = 5;
		cycle = (cycle + 1) % 6;
		SignatureAlias renderSignature;
		renderSignature.set(GAMEOBJECT_COMPONENT::GAMEPOSITION);
		renderSignature.set(GAMEOBJECT_COMPONENT::GRAPHICSTILE);

		switch (cycle) {
		case 4:
		{
			size_t count = 0;
			Locator<misc::Timer>::ref().newTiming("old1.1");
			size_t end = this->everything->getlast<GamePosition>();
			for (size_t i = 0; i < end; i++) {
				if (this->everything->signatures[this->everything->gamepositions[i].index].contains(renderSignature)) {
					count++;
				}
			}
			Locator<misc::Timer>::ref().endTiming("old1.1");
			std::cout << "old1.1 count " << count << '\n';
			break;
		}
		case 0:
		{
			size_t count = 0;
			Locator<misc::Timer>::ref().newTiming("old");
			this->everything->run([&](Match<GamePosition, GraphicsTile> e) {
				count++;
				renderInfo.tileRenderInfo.addBlitInfo(
					glm::vec4(e.get<GamePosition>().pos, 1, 1),
					0,
					e.get<GraphicsTile>().blockID
				);
				});
			Locator<misc::Timer>::ref().endTiming("old");
			std::cout << "old count " << count << '\n';

			break;
		}
		case 1:
		{
			Locator<misc::Timer>::ref().newTiming("new");
			size_t count = 0;
			SizeAlias end = this->everything2.data[EverythingS::component_index<GamePosition>::val].index;

			for (SizeAlias i = 0; i < end; i++) {
				auto& e = this->everything2.data[EverythingS::component_index<GamePosition>::val].get<GamePosition>(i);

				if (this->everything2.has<GraphicsTile>(e.index)) {
					count++;
					renderInfo.tileRenderInfo.addBlitInfo(
						glm::vec4(e.pos, 1, 1),
						0,
						this->everything2.get<GraphicsTile>(e.index).blockID
					);
				}
			}
			Locator<misc::Timer>::ref().endTiming("new");
			std::cout << "new count " << count << '\n';
			break;
		}
		case 2:
		{
			Locator<misc::Timer>::ref().newTiming("new1.1");
			SizeAlias end = this->everything2.data[EverythingS::component_index<GamePosition>::val].index;

			size_t count = 0;

			for (SizeAlias i = 0; i < end; i++) {
				auto& e = this->everything2.data[EverythingS::component_index<GamePosition>::val].get<GamePosition>(i);

				if (this->everything2.indirectionMap[e.index].contains(EverythingS::group_signature<GamePosition, GraphicsTile>::val)) {
					count++;
					renderInfo.tileRenderInfo.addBlitInfo(
						glm::vec4(e.pos, 1, 1),
						0,
						this->everything2.get<GraphicsTile>(e.index).blockID
					);
				}
			}
			Locator<misc::Timer>::ref().endTiming("new1.1");
			std::cout << "new1.1 count " << count << '\n';
			break;
		}
		case 3:
		{

			size_t count = 0;
			//std::function<void(game::MatchS<GamePosition, GraphicsTile>)> f = [&](game::MatchS<GamePosition, GraphicsTile> e) {
			//	count++;
			//	renderInfo.tileRenderInfo.addBlitInfo(
			//		//glm::vec4(e.get<GamePosition>().pos, 1, 1),
			//		glm::vec4(0),
			//		0,
			//		0
			//		//e.get<GraphicsTile>().blockID
			//	);
			//};

			Locator<misc::Timer>::ref().newTiming("new2");
			this->everything2.run([&](game::MatchS<GamePosition, GraphicsTile>& e) {
				count++;
				renderInfo.tileRenderInfo.addBlitInfo(
					glm::vec4(e.get<GamePosition>().pos, 1, 1),
					0,
					e.get<GraphicsTile>().blockID
				);
				});

			Locator<misc::Timer>::ref().endTiming("new2");
			std::cout << "new2 count " << count << '\n';

			break;
		}
		case 5:
		{
			Locator<misc::Timer>::ref().newTiming("something new");
			size_t count = 0;
			void(*f)(game::MatchS<GamePosition, GraphicsTile>&, render::RenderInfo&, size_t&) = [](game::MatchS<GamePosition, GraphicsTile>& e, render::RenderInfo& renderInfo, size_t& count) {
				count++;

				renderInfo.tileRenderInfo.addBlitInfo(
					glm::vec4(e.get<GamePosition>().pos, 1, 1),
					0,
					e.get<GraphicsTile>().blockID
				);
			};

			te::Loop::run<
				EverythingS,
				decltype(f),
				te::list<game::MatchS<GamePosition, GraphicsTile>>,

				render::RenderInfo&,
				size_t&>
				(this->everything2, f, renderInfo, count);

			Locator<misc::Timer>::ref().endTiming("something new");
			std::cout << "something new count: " << count << '\n';
			break;
		}
		default:
			break;
		}

	}

	void GameState::runTick() {
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

		SizeAlias end = this->everything2.data[EverythingS::component_index<GamePosition>::val].index;

		for (SizeAlias i = 0; i < end; i++) {
			auto& e = this->everything2.data[EverythingS::component_index<GamePosition>::val].get<GamePosition>(i);

			if (this->everything2.has<Locomotion>(e.index)) {
				auto& loco = this->everything2.get<Locomotion>(e.index);
				if (loco.cooldown != 0) {
					loco.cooldown--;
				}
				else {
					loco.cooldown = loco.fitness;
					e.pos += glm::ivec2((rand() % 3) - 1, (rand() % 3) - 1);
				}
			}
		}
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

	struct Void {};

	GameState::GameState() {
		this->everything = std::make_unique<Everything>();

		rand();

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

			{
				//auto p = this->everything->makeWeak();
				//p.addbrain();
				//p.addpossession();
				//p.addvicinity();
				//p.addgraphicstile();
				//p.addlocomotion();
				//p.addgameposition();

				//p.gameposition().pos = { 5 , 5 };

				//p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");

				//p.locomotion().fitness = 10;
			}
			{
				auto p = this->everything2.make();
				p.add<Brain>();
				p.add<Vicinity>();
				p.add<GraphicsTile>();
				p.add<Locomotion>();
				p.add<GamePosition>();

				p.get<GamePosition>().pos = { 5, 5 };

				p.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");

				p.get<Locomotion>().fitness = 10;
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

				auto p2 = this->everything2.make();
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
				auto p = this->everything->makeWeak();
				p.addgameposition();
				p.gameposition().pos = glm::ivec2(i, j);

				p.addgraphicstile();
				p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");

				auto p2 = this->everything2.make();
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
				auto p = this->everything->makeWeak();
				p.addgameposition();
				p.gameposition().pos = glm::ivec2(i, j);

				p.addgraphicstile();
				p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");

				auto p2 = this->everything2.make();
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
				auto p = this->everything->makeWeak();
				p.addgameposition();
				p.gameposition().pos = glm::ivec2(i, j);

				p.addgraphicstile();
				p.graphicstile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");

				auto p2 = this->everything2.make();
				p2.add<GamePosition>();
				p2.add<GraphicsTile>();

				p2.get<GamePosition>().pos = glm::ivec2(i, j);
				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
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
