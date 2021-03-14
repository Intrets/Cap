#include "GameState.h"

#include <render/infos/RenderInfo.h>

#include <cstdlib>

namespace game
{
	void GameState::addRenderInfo(render::RenderInfo& renderInfo) {
		for (size_t i = 0; i < 100; i++) {
			for (size_t j = 0; j < 100; j++) {
				renderInfo.tileRenderInfo.data.push_back({
					glm::vec4(i, j, 1, 1),
					0,
					this->world[i][j] ? 0 : 1 }
				);
			}
		}
	}

	GameState::GameState() {
		for (size_t i = 0; i < 100; i++) {
			for (size_t j = 0; j < 100; j++) {
				this->world[i][j] = std::rand() % 2 == 0;
			}
		}
	}
}
