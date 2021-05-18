#include "GameState.h"

#include <render/infos/RenderInfo.h>

#include <mem/Locator.h>
#include <render/textures/BlockIDTextures.h>

#include <cstdlib>

namespace game
{
	void GameState::addRenderInfo(render::RenderInfo& renderInfo) {
		Signature renderSignature;
		renderSignature.set(COMPONENT::GAME_POSITION);
		renderSignature.set(COMPONENT::GRAPHIS_TILE);

		for (auto& [h, obj] : this->refMan.data) {
			if (obj.get()->signature.contains(renderSignature)) {
				renderInfo.tileRenderInfo.addBlitInfo(
					glm::vec4(obj.get()->gamePosition().pos, 1, 1),
					0,
					obj.get()->graphicsTile().blockID
				);
			}
		}
	}

	void GameState::runTick() {
		Signature conciousSignature;
		conciousSignature.set(COMPONENT::BRAIN);
		conciousSignature.set(COMPONENT::POSSESSION);
		conciousSignature.set(COMPONENT::VICINITY);

		Signature foodSignature;
		foodSignature.set(COMPONENT::NUTRITION);

		Signature friendlySignature;
		friendlySignature.set(COMPONENT::BRAIN);

		Signature brainSignature;
		brainSignature.set(COMPONENT::BRAIN);

		Signature locomotionSignature;
		locomotionSignature.set(COMPONENT::LOCOMOTION);
		locomotionSignature.set(COMPONENT::GAME_POSITION);

		Concept foodConcept;

		foodConcept.essences.push_back({ 10.0f, foodSignature });
		foodConcept.essences.push_back({ -10.0f, friendlySignature });

		for (auto& [h, obj] : this->refMan.data) {
			// Advanced test
			if (obj.get()->signature.contains(locomotionSignature)) {
				auto& loc = obj.get()->locomotion();
				auto& pos = obj.get()->gamePosition();

				if (loc.cooldown != 0) {
					loc.cooldown--;
				}
				else if (loc.target.has_value()) {
					auto d = loc.target.value() - pos.pos;
					std::cout << "d:" << d.x << " " << d.y << "\n";
					if (d.x == 0 && d.y == 0) {
						loc.target = std::nullopt;
					}
					else {
						int32_t maxD = glm::max(glm::abs(d.x), glm::abs(d.y));

						auto mov = glm::sign(d) * (glm::abs(d) / maxD);
						std::cout << mov.x << " " << mov.y << "\n";

						pos.pos += mov;
						loc.cooldown = 120;
					}
				}
			}

			if (obj.get()->signature.contains(brainSignature)) {
				if (obj.get()->brain().energy > 0) {
					obj.get()->brain().energy--;
				}
			}

			if (obj.get()->signature.contains(conciousSignature)) {
				auto& brain = obj.get()->brain();
				if (!brain.currentAction.has_value()) {
					if (brain.energy == 0) {
						std::cout << "energy low, seeking food\n";

						//for (auto& mem : obj.get()->brain().memory) {
						//	for (auto& res : mem.results) {
						//		if (foodConcept.value(res) > 1.0f) {
						//			brain.currentAction = mem;
						//		}
						//	}
						//}

						glm::ivec2 target{ std::rand() % 20, std::rand() % 20 };


						Action testAction;
						testAction.runFunction = [target = target](Object* obj) -> ActionResult
						{
							ActionResult result{};

							auto d = target - obj->gamePosition().pos;

							if (d.x == 0 && d.y == 0) {
								obj->brain().energy = 200;
								result.success = true;
							}
							else {
								int32_t maxD = glm::max(glm::abs(d.x), glm::abs(d.y));

								auto mov = glm::sign(d) * (glm::abs(d) / maxD);
								std::cout << mov.x << " " << mov.y << "\n";

								obj->gamePosition().pos += mov;
							}
							return result;
						};

						obj.get()->brain().currentAction = testAction;
					}
				}
				else {
					if (brain.currentAction.value().run(obj.get()).success) {
						brain.currentAction = std::nullopt;
					}
				}
			}

		}

		this->tick++;
	}

	GameState::GameState() {
		{
			//for (size_t j = 1; j < 100; j++) {
			//	for (size_t i = 1; i < WORLD_SIZE - 2; i++) {
			//		auto ref = this->refMan.makeRef<Object>();
			//		auto ptr = ref.get();

			//		glm::ivec2 pos = { j, i };

			//		ptr->signature.set(COMPONENT::BRAIN);
			//		ptr->signature.set(COMPONENT::LOCOMOTION);
			//		ptr->locomotion().fitness = 0;

			//		ptr->signature.set(COMPONENT::GAME_POSITION);
			//		ptr->gamePosition().pos = pos;

			//		ptr->signature.set(COMPONENT::GRAPHIS_TILE);
			//		ptr->graphicsTile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");
			//	}
			//}
		}

		{
			auto ref = this->refMan.makeRef<Object>();
			auto ptr = ref.get();

			ptr->signature.set(
				{
				COMPONENT::BRAIN,
				COMPONENT::POSSESSION,
				COMPONENT::VICINITY,
				});

			ptr->signature.set(COMPONENT::GAME_POSITION);
			ptr->gamePosition().pos = { 10,10 };
			//this->world[10][10] = ref;

			ptr->signature.set(COMPONENT::GRAPHIS_TILE);
			ptr->graphicsTile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("gnome.dds");

			ptr->signature.set(COMPONENT::LOCOMOTION);
			ptr->locomotion().fitness = 10;

			Action recallFood;
			recallFood.requirements = {};
			Signature foodSignature;
			foodSignature.set(COMPONENT::NUTRITION).set(COMPONENT::GAME_POSITION);
			recallFood.results = { foodSignature };
			recallFood.runFunction = [](Object* obj) -> ActionResult
			{
				ActionResult result{};

				return result;
			};

			ptr->brain().memory.push_back(recallFood);

		}


		{
			auto ref = this->refMan.makeRef<Object>();
			auto ptr = ref.get();

			glm::ivec2 pos = { 14, 14 };

			ptr->signature.set(COMPONENT::GAME_POSITION);
			ptr->gamePosition().pos = pos;
			//this->world[14][14] = ref;

			ptr->signature.set(COMPONENT::GRAPHIS_TILE);
			ptr->graphicsTile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("food.dds");
		}


		for (size_t i = 0; i < WORLD_SIZE; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				size_t j = 0;
				auto ref = this->refMan.makeRef<Object>();
				auto ptr = ref.get();
				ptr->signature.set(COMPONENT::GAME_POSITION);
				ptr->gamePosition().pos = glm::ivec2(i, j);

				ptr->signature.set(COMPONENT::GRAPHIS_TILE);
				ptr->graphicsTile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");

				//this->world[i][j] = ref;
			}
		}

		for (size_t i = 0; i < WORLD_SIZE; i++) {
			//for (size_t j = 0; j < 30; j++) {
			{
				int j = WORLD_SIZE - 1;
				auto ref = this->refMan.makeRef<Object>();
				auto ptr = ref.get();
				ptr->signature.set(COMPONENT::GAME_POSITION);
				ptr->gamePosition().pos = glm::ivec2(i, j);
				ptr->signature.set(COMPONENT::GRAPHIS_TILE);
				ptr->graphicsTile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");

				//this->world[i][j] = ref;
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			size_t i = 0;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto ref = this->refMan.makeRef<Object>();
				auto ptr = ref.get();
				ptr->signature.set(COMPONENT::GAME_POSITION);
				ptr->gamePosition().pos = glm::ivec2(i, j);
				ptr->signature.set(COMPONENT::GRAPHIS_TILE);
				ptr->graphicsTile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");

				//this->world[i][j] = ref;
			}
		}

		//for (size_t i = 0; i < 30; i++) {
		{
			int i = WORLD_SIZE - 1;
			for (size_t j = 0; j < WORLD_SIZE; j++) {
				auto ref = this->refMan.makeRef<Object>();
				auto ptr = ref.get();
				ptr->signature.set(COMPONENT::GAME_POSITION);
				ptr->gamePosition().pos = glm::ivec2(i, j);
				ptr->signature.set(COMPONENT::GRAPHIS_TILE);
				ptr->graphicsTile().blockID = Locator<render::BlockIDTextures>::ref().getBlockTextureID("weird_ground.dds");

				//this->world[i][j] = ref;
			}
		}
	}

	ActionResult Action::run(Object* obj) {
		return this->runFunction(obj);
	}

	float Concept::value(Signature const& signature) {
		float result = 0.0f;

		for (auto const& essence : this->essences) {
			if (signature.contains(essence.signature)) {

				result += essence.value;
			}
		}

		return result;
	}

	Signature& Signature::set(COMPONENT component) {
		this->data.set(component);
		return *this;
	}

	void Signature::set(std::initializer_list<COMPONENT> components) {
		for (auto comp : components) {
			this->data.set(comp);
		}
	}

	bool Signature::test(COMPONENT component) {
		return this->data.test(component);
	}

	bool Signature::contains(Signature const& other) const {
		return (this->data & other.data) == other.data;
	}

	Action const& Brain::findAction(std::vector<Signature> const& requirements) {
		for (auto const& action : this->memory) {
			for (auto const& otherReq : requirements) {
				for (auto const& memoryReq : action.requirements) {
					if (!memoryReq.contains(otherReq)) {

					}
				}
			}
		}
		return Action();
	}

	void Brain::merge(std::vector<Action>& other) {
		this->memory.insert(
			this->memory.end(),
			std::make_move_iterator(other.begin()),
			std::make_move_iterator(other.end())
		);
	}
}
