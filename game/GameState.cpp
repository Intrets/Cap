#include "GameState.h"

#include <render/infos/RenderInfo.h>

#include <mem/Locator.h>
#include <render/textures/BlockIDTextures.h>

#include <cstdlib>

#include <misc/Timer.h>

namespace game
{
	void GameState::addRenderInfo(render::RenderInfo& renderInfo) {
		static int alternate = 0;
		alternate++;
		alternate %= 16;
		switch (alternate) {
		case 0:
		{
			size_t counter = 0;
			Locator<misc::Timer>::ref().newTiming("new2");
			this->everything.run([&](game::Match<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>, Test<4>, Test<5>, Test<6>, Test<7>, Test<8>, Test<9>, Test<10>>& e) {
				//e.get<GamePosition>().pos += glm::ivec2(1, 0);
				//e.get<GraphicsTile>().blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(e.get<GamePosition>().pos, 1, 1),
				//	0,
				//	e.get<GraphicsTile>().blockID
				//);
				counter++;
				});

			Locator<misc::Timer>::ref().endTiming("new2");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 1:
		{
			Locator<misc::Timer>::ref().newTiming("entt10");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>, Test<4>, Test<5>, Test<6>, Test<7>, Test<8>, Test<9>, Test<10>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto, auto, auto, auto, auto, auto, auto, auto, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt10");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 2:
		{
			Locator<misc::Timer>::ref().newTiming("entt2");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>, Test<4>, Test<5>, Test<6>, Test<7>, Test<8>, Test<9>, Test<10>>();
			size_t counter = 0;

			for (auto entity : view) {
				//view.get<GamePosition>(entity).pos += glm::ivec2(1, 0);
				//view.get<GraphicsTile>(entity).blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(view.get<GamePosition>(entity).pos, 1, 1),
				//	0,
				//	view.get<GraphicsTile>(entity).blockID
				//);
				counter++;
			}

			Locator<misc::Timer>::ref().endTiming("entt2");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 3:
		{
			Locator<misc::Timer>::ref().newTiming("a");
			auto& g = this->everything.data[Everything::component_index_v<GamePosition>];
			size_t counter = 0;

			for (size_t i = 0; i < g.index; i++) {
				//auto& pos = g.get<GamePosition>(i);
				auto& pos = *reinterpret_cast<GamePosition*>(&g.data[i]);
				//if (this->everything.has<GraphicsTile>(pos.index)) {
				//if (this->everything.indirectionMap[0].index[1] != 0) {
				//if (pos.index != 0){
				if (this->everything.indirectionMap[pos.index].index[1] != 0) {

					counter++;
				}
				//auto& tile = this->everything.get<GraphicsTile>(pos.index);

				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
			}
			Locator<misc::Timer>::ref().endTiming("a");
			std::cout << "counter " << counter << '\n';
			break;
		}

		case 4:
		{
			Locator<misc::Timer>::ref().newTiming("entt9");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>, Test<4>, Test<5>, Test<6>, Test<7>, Test<8>, Test<9>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto, auto, auto, auto, auto, auto, auto, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt9");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 5:
		{
			Locator<misc::Timer>::ref().newTiming("entt8");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>, Test<4>, Test<5>, Test<6>, Test<7>, Test<8>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto, auto, auto, auto, auto, auto, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt8");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 6:
		{
			Locator<misc::Timer>::ref().newTiming("entt7");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>, Test<4>, Test<5>, Test<6>, Test<7>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto, auto, auto, auto, auto, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt7");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 7:
		{
			Locator<misc::Timer>::ref().newTiming("entt6");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>, Test<4>, Test<5>, Test<6>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto, auto, auto, auto, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt6");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 8:
		{
			Locator<misc::Timer>::ref().newTiming("entt5");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>, Test<4>, Test<5>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto, auto, auto, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt5");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 9:
		{
			Locator<misc::Timer>::ref().newTiming("entt4");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>, Test<4>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto, auto, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt4");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 10:
		{
			Locator<misc::Timer>::ref().newTiming("entt3");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>, Test<3>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt3");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 11:
		{
			Locator<misc::Timer>::ref().newTiming("entt2");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>, Test<2>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt2");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 12:
		{
			Locator<misc::Timer>::ref().newTiming("entt1");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>, Test<1>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt1");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 14:
		{
			Locator<misc::Timer>::ref().newTiming("entt0");
			auto view = this->registry.view<GamePosition, GraphicsTile, Test<0>>();

			size_t counter = 0;
			view.each([&](auto pos, auto tile, auto) {
				//pos.pos += glm::ivec2(1, 0);
				//tile.blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(pos.pos, 1, 1),
				//	0,
				//	tile.blockID
				//);
				counter++;
				});
			Locator<misc::Timer>::ref().endTiming("entt0");
			std::cout << "counter " << counter << '\n';
			break;
		}
		case 15:
		{
			size_t counter = 0;
			Locator<misc::Timer>::ref().newTiming("double");
			this->everything.run([&](game::Match<GamePosition, GraphicsTile>& e) {
				//e.get<GamePosition>().pos += glm::ivec2(1, 0);
				//e.get<GraphicsTile>().blockID++;
				//renderInfo.tileRenderInfo.addBlitInfo(
				//	glm::vec4(e.get<GamePosition>().pos, 1, 1),
				//	0,
				//	e.get<GraphicsTile>().blockID
				//);
				counter++;
				});

			Locator<misc::Timer>::ref().endTiming("double");
			std::cout << "counter " << counter << '\n';
			break;
		}


		default:
			break;
		}

	}

	void GameState::runTick() {
		//this->everything.run([](Match<Locomotion, GamePosition>& e) {
		//	if (e.get<Locomotion>().cooldown != 0) {
		//		e.get<Locomotion>().cooldown--;
		//	}
		//	else {
		//		e.get<Locomotion>().cooldown = e.get<Locomotion>().fitness;
		//		e.get<GamePosition>().pos += glm::ivec2((rand() % 3) - 1, (rand() % 3) - 1);
		//	}
		//	});

		//this->everything.run([&](Match<Spawner, GamePosition>& e) {
		//	//auto p = this->everything.make();
		//	//p.add<GamePosition>();
		//	//p.add<GraphicsTile>();

		//	//p.get<GamePosition>().pos = e.get<GamePosition>().pos + glm::ivec2(1, 0);
		//	//p.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
		//	});


		this->tick++;
	}

	GameState::GameState() {
		{
			const auto entity = this->registry.create();
			this->registry.emplace<GamePosition>(entity);

			this->registry.emplace<Test<0>>(entity);
			this->registry.emplace<Test<1>>(entity);
			this->registry.emplace<Test<2>>(entity);
			this->registry.emplace<Test<3>>(entity);
			this->registry.emplace<Test<4>>(entity);
			this->registry.emplace<Test<5>>(entity);
			this->registry.emplace<Test<6>>(entity);
			this->registry.emplace<Test<7>>(entity);
			this->registry.emplace<Test<8>>(entity);
			this->registry.emplace<Test<9>>(entity);
			this->registry.emplace<Test<10>>(entity);
			this->registry.emplace<Test<11>>(entity);
			this->registry.emplace<Test<12>>(entity);
			this->registry.emplace<Test<13>>(entity);
			this->registry.emplace<Test<14>>(entity);
			this->registry.emplace<Test<15>>(entity);
			this->registry.emplace<Test<16>>(entity);
			this->registry.emplace<Test<17>>(entity);
			this->registry.emplace<Test<18>>(entity);
			this->registry.emplace<Test<19>>(entity);
			this->registry.emplace<Test<20>>(entity);
			this->registry.emplace<Test<21>>(entity);
			this->registry.emplace<Test<22>>(entity);
			this->registry.emplace<Test<23>>(entity);
			this->registry.emplace<Test<24>>(entity);
			this->registry.emplace<Test<25>>(entity);
			this->registry.emplace<Test<26>>(entity);
			this->registry.emplace<Test<27>>(entity);
			this->registry.emplace<Test<28>>(entity);
			this->registry.emplace<Test<29>>(entity);
			this->registry.emplace<Test<30>>(entity);
			this->registry.emplace<Test<31>>(entity);
			this->registry.emplace<Test<32>>(entity);
			this->registry.emplace<Test<33>>(entity);
			this->registry.emplace<Test<34>>(entity);
			this->registry.emplace<Test<35>>(entity);
			this->registry.emplace<Test<36>>(entity);
			this->registry.emplace<Test<37>>(entity);
			this->registry.emplace<Test<38>>(entity);
			this->registry.emplace<Test<39>>(entity);
			this->registry.emplace<Test<40>>(entity);
			this->registry.emplace<Test<41>>(entity);
			this->registry.emplace<Test<42>>(entity);
			this->registry.emplace<Test<43>>(entity);
			this->registry.emplace<Test<44>>(entity);
			this->registry.emplace<Test<45>>(entity);
			this->registry.emplace<Test<46>>(entity);
			this->registry.emplace<Test<47>>(entity);
			this->registry.emplace<Test<48>>(entity);
			this->registry.emplace<Test<49>>(entity);
			this->registry.emplace<Test<50>>(entity);
			this->registry.emplace<Test<51>>(entity);
			this->registry.emplace<Test<52>>(entity);
			this->registry.emplace<Test<53>>(entity);
			this->registry.emplace<Test<54>>(entity);
			this->registry.emplace<Test<55>>(entity);
			this->registry.emplace<Test<56>>(entity);
			this->registry.emplace<Test<57>>(entity);
			this->registry.emplace<Test<58>>(entity);
			this->registry.emplace<Test<59>>(entity);
			this->registry.emplace<Test<60>>(entity);
			this->registry.emplace<Test<61>>(entity);
			this->registry.emplace<Test<62>>(entity);
		}



		{
			{
				const auto entity = this->registry.create();
				this->registry.emplace<GamePosition>(entity, 0u, glm::ivec2(5, 5));
				this->registry.emplace<GraphicsTile>(entity, 0u, Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds"));
				this->registry.emplace<Test<0>>(entity);
				this->registry.emplace<Test<1>>(entity);
				this->registry.emplace<Test<2>>(entity);
				this->registry.emplace<Test<3>>(entity);
				this->registry.emplace<Test<4>>(entity);
				this->registry.emplace<Test<5>>(entity);
				this->registry.emplace<Test<6>>(entity);
				this->registry.emplace<Test<7>>(entity);
				this->registry.emplace<Test<8>>(entity);
				this->registry.emplace<Test<9>>(entity);
				this->registry.emplace<Test<10>>(entity);


				auto p2 = this->everything.make();
				p2.add<GamePosition>(glm::ivec2(5, 5));
				p2.add<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
				p2.add<Test<0>>();
				p2.add<Test<1>>();
				p2.add<Test<2>>();
				p2.add<Test<3>>();
				p2.add<Test<4>>();
				p2.add<Test<5>>();
				p2.add<Test<6>>();
				p2.add<Test<7>>();
				p2.add<Test<8>>();
				p2.add<Test<9>>();
			}

			{
				//const auto entity = this->registry.create();
				//this->registry.emplace<GamePosition>(entity, 0u, glm::ivec2(5, 6));
				//this->registry.emplace<GraphicsTile>(entity, 0u, Locator<render::BlockIDTextures>::ref().getBlockTextureID("s_block.dds"));
				//this->registry.emplace<Spawner>(entity);
				//auto p = this->everything.make();

				//p.add<GamePosition>();
				//p.add<GraphicsTile>();
				//p.add<Spawner>();

				//p.get<GamePosition>().pos = glm::ivec2(5, 6);
				//p.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("s_block.dds");
			}

			//{
			//	auto p = this->everything.make();
			//	//p.add<Brain>();
			//	//p.add<Vicinity>();
			//	p.add<GraphicsTile>();
			//	//p.add<Locomotion>();
			//	p.add<GamePosition>();

			//	p.get<GamePosition>().pos = { 5, 5 };

			//	p.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");

			//	//p.get<Locomotion>().fitness = 10;
			//}

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
				p2.add<Test<0>>();
				p2.add<Test<1>>();
				p2.add<Test<2>>();
				p2.add<Test<3>>();
				p2.add<Test<4>>();
				p2.add<Test<5>>();
				p2.add<Test<6>>();
				p2.add<Test<7>>();
				p2.add<Test<8>>();
				p2.add<Test<9>>();

				const auto entity = this->registry.create();
				this->registry.emplace<GamePosition>(entity, 0u, glm::ivec2(i, j));
				this->registry.emplace<GraphicsTile>(entity, 0u, Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds"));
				this->registry.emplace<Test<0>>(entity);
				this->registry.emplace<Test<1>>(entity);
				this->registry.emplace<Test<2>>(entity);
				this->registry.emplace<Test<3>>(entity);
				this->registry.emplace<Test<4>>(entity);
				this->registry.emplace<Test<5>>(entity);
				this->registry.emplace<Test<6>>(entity);
				this->registry.emplace<Test<7>>(entity);
				this->registry.emplace<Test<8>>(entity);
				this->registry.emplace<Test<9>>(entity);
				this->registry.emplace<Test<10>>(entity);

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
				p2.add<Test<0>>();
				p2.add<Test<1>>();
				p2.add<Test<2>>();
				p2.add<Test<3>>();
				p2.add<Test<4>>();
				p2.add<Test<5>>();
				p2.add<Test<6>>();
				p2.add<Test<7>>();
				p2.add<Test<8>>();
				p2.add<Test<9>>();

				p2.get<GamePosition>().pos = glm::ivec2(i, j);
				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
				const auto entity = this->registry.create();
				this->registry.emplace<GamePosition>(entity, 0u, glm::ivec2(i, j));
				this->registry.emplace<GraphicsTile>(entity, 0u, Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds"));
				this->registry.emplace<Test<0>>(entity);
				this->registry.emplace<Test<1>>(entity);
				this->registry.emplace<Test<2>>(entity);
				this->registry.emplace<Test<3>>(entity);
				this->registry.emplace<Test<4>>(entity);
				this->registry.emplace<Test<5>>(entity);
				this->registry.emplace<Test<6>>(entity);
				this->registry.emplace<Test<7>>(entity);
				this->registry.emplace<Test<8>>(entity);
				this->registry.emplace<Test<9>>(entity);
				this->registry.emplace<Test<10>>(entity);
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			size_t i = 0;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto p2 = this->everything.make();
				p2.add<GamePosition>();
				p2.add<GraphicsTile>();
				p2.add<Test<0>>();
				p2.add<Test<1>>();
				p2.add<Test<2>>();
				p2.add<Test<3>>();
				p2.add<Test<4>>();
				p2.add<Test<5>>();
				p2.add<Test<6>>();
				p2.add<Test<7>>();
				p2.add<Test<8>>();
				p2.add<Test<9>>();

				p2.get<GamePosition>().pos = glm::ivec2(i, j);
				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
				const auto entity = this->registry.create();
				this->registry.emplace<GamePosition>(entity, 0u, glm::ivec2(i, j));
				this->registry.emplace<GraphicsTile>(entity, 0u, Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds"));
				this->registry.emplace<Test<0>>(entity);
				this->registry.emplace<Test<1>>(entity);
				this->registry.emplace<Test<2>>(entity);
				this->registry.emplace<Test<3>>(entity);
				this->registry.emplace<Test<4>>(entity);
				this->registry.emplace<Test<5>>(entity);
				this->registry.emplace<Test<6>>(entity);
				this->registry.emplace<Test<7>>(entity);
				this->registry.emplace<Test<8>>(entity);
				this->registry.emplace<Test<9>>(entity);
				this->registry.emplace<Test<10>>(entity);
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			int i = WORLD_SIZE - 1;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto p2 = this->everything.make();
				p2.add<GamePosition>();
				p2.add<GraphicsTile>();
				p2.add<Test<0>>();
				p2.add<Test<1>>();
				p2.add<Test<2>>();
				p2.add<Test<3>>();
				p2.add<Test<4>>();
				p2.add<Test<5>>();
				p2.add<Test<6>>();
				p2.add<Test<7>>();
				p2.add<Test<8>>();
				p2.add<Test<9>>();

				p2.get<GamePosition>().pos = glm::ivec2(i, j);
				p2.get<GraphicsTile>().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");
				const auto entity = this->registry.create();
				this->registry.emplace<GamePosition>(entity, 0u, glm::ivec2(i, j));
				this->registry.emplace<GraphicsTile>(entity, 0u, Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds"));
				this->registry.emplace<Test<0>>(entity);
				this->registry.emplace<Test<1>>(entity);
				this->registry.emplace<Test<2>>(entity);
				this->registry.emplace<Test<3>>(entity);
				this->registry.emplace<Test<4>>(entity);
				this->registry.emplace<Test<5>>(entity);
				this->registry.emplace<Test<6>>(entity);
				this->registry.emplace<Test<7>>(entity);
				this->registry.emplace<Test<8>>(entity);
				this->registry.emplace<Test<9>>(entity);
				this->registry.emplace<Test<10>>(entity);
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
