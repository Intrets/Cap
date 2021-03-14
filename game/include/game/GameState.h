#pragma once

#include <array>

#include <mem/ReferenceManager.h>

namespace render
{
	struct RenderInfo;
}

namespace game
{
	class GameState
	{
	private:

	public:
		int32_t tick = 0;

		std::array<std::array<bool, 100>, 100> world;

		void addRenderInfo(render::RenderInfo& renderInfo);

		GameState();
		~GameState() = default;
	};
}
