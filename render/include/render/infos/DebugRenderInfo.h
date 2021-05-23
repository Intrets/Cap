#pragma once

#include <vector>
#include <wglm/glm.hpp>

namespace render
{
	struct DebugRenderInfo
	{
		struct Data
		{
			struct Points
			{
				std::vector<glm::vec2> points;
			} points;

			struct Lines
			{
				std::vector<glm::vec2> lines;
			} lines;

			void addPoint(glm::vec2 p);
			void addPoint(float x, float y);

			void addLine(glm::vec2 p1, glm::vec2 p2);
			void addLine(float x1, float y1, float x2, float y2);
			void addLine(glm::vec2 p1, float x2, float y2);
			void addLine(float x1, float y1, glm::vec2 p2);

			void addBox(glm::vec2 u, glm::vec2 d);
			void addBox(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4);
		};

		Data world;
		Data screen;

		void clear();
	};
}
