#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace render
{
	struct CameraInfo
	{
		int32_t x;
		int32_t y;
		glm::vec2 camPos;
		glm::vec3 viewPort;
		glm::mat4 VP = glm::ortho(-viewPort.x, viewPort.x, -viewPort.y, viewPort.y, -viewPort.z, viewPort.z) * glm::translate(-glm::vec3(camPos, 0.0f));
	};
}