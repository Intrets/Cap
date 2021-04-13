#pragma once

#include <wglm/glm.hpp>
#include <vector>

namespace render
{
	struct SingleBlitRenderInfo
	{
		glm::vec4 quad;
		glm::vec4 world;
		int32_t rotation;
	};

	struct BlitRenderInfo
	{
	private:
		std::vector<SingleBlitRenderInfo> data;

	public:
		std::vector<SingleBlitRenderInfo> const& getData() const;

		std::vector<SingleBlitRenderInfo>& getDataMut();

		void addBlitInfo(glm::vec4 q, glm::vec4 w, int32_t r);
		void addBlitInfo(SingleBlitRenderInfo const& info);
	};
}

namespace render
{
	struct SinglePixelRenderInfo
	{
		glm::vec4 quad;
		glm::vec4 world;
		glm::vec2 pixelWorld;
		glm::vec2 pixelTex;
		int32_t rotation;
	};

	struct PixelRenderInfo
	{
	private:
		std::vector<SinglePixelRenderInfo> data;

	public:
		std::vector<SinglePixelRenderInfo> const& getData() const;

		std::vector<SinglePixelRenderInfo>& getDataMut();

		void addBlitInfo(SinglePixelRenderInfo const& info);

		void addBlitInfo(glm::vec4 q, glm::vec4 w, glm::vec2 a, glm::vec2 b, int32_t r);
	};
}