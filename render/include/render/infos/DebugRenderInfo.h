#pragma once

#include <vector>
#include <wglm/glm.hpp>

#include "../Colors.h"

namespace render
{
	constexpr colors::Color defaultColor = colors::magenta;

	struct DebugRenderInfo
	{
		struct Data
		{
			struct Point
			{
				glm::vec2 pos;
				colors::Color color;
			};

			struct Points
			{
				std::vector<Point> points;
			} points;

			struct Lines
			{
				std::vector<Point> lines;
			} lines;


			void addPoint(glm::vec2 p, colors::Color c = defaultColor);
			void addPoint(float x, float y, colors::Color c = defaultColor);

			void addLine(glm::vec2 p1, glm::vec2 p2, colors::Color c = defaultColor);
			void addLine(float x1, float y1, float x2, float y2, colors::Color c = defaultColor);
			void addLine(glm::vec2 p1, float x2, float y2, colors::Color c = defaultColor);
			void addLine(float x1, float y1, glm::vec2 p2, colors::Color c = defaultColor);

			void addBox(glm::vec2 u, glm::vec2 d, colors::Color c = defaultColor);
			void addBox(float x1, float y1, float x2, float y2, colors::Color c = defaultColor);
			void addBox(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, colors::Color c = defaultColor);
		};

		Data world;
		Data screen;

		void clear();
	};
}
