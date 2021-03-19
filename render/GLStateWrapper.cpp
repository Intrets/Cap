#include "GLStateWrapper.h"

#include <cassert>

namespace ogs
{
	State::State() {
		this->setBlend(BLEND::DISABLED);
		this->setBlendFunc(BLEND_FUNC::SRC_ALPHA__ONE_MINUS_SRC_ALPHA);
		this->setDepthFunc(DEPTH_FUNC::LESS);
		this->setDepthTest(DEPTH_TEST::DISABLED);
		this->setPolygonMode(POLYGON_MODE::FILL);

		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &this->MAX_COLOR_ATTACHMENTS);
	}

	void State::setState(Configuration const& config) {
		this->setBlend(config.blend);
		if (config.blend == BLEND::ENABLED) {
			this->setBlendFunc(config.blendFunc);
		}

		this->setDepthTest(config.depthTest);
		if (config.depthTest == DEPTH_TEST::ENABLED) {
			this->setDepthFunc(config.depthFunc);
		}

		this->setPolygonMode(config.polygonMode);
	}

	void State::setBlend(BLEND b) {
		if (this->configuration.blend != b) {
			this->configuration.blend = b;
			switch (b) {
				case BLEND::ENABLED:
					glEnable(GL_BLEND);
					break;
				case BLEND::DISABLED:
					glDisable(GL_BLEND);
					break;
				default:
					assert(0);
					break;
			}

		}
	}

	void State::setBlendFunc(BLEND_FUNC func) {
		if (this->configuration.blendFunc != func) {
			this->configuration.blendFunc = func;
			switch (func) {
				case BLEND_FUNC::SRC_ALPHA__ONE_MINUS_SRC_ALPHA:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case BLEND_FUNC::SRC_ONE__ONE_MINUS_SRC_ALPHA:
					glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
					break;
				default:
					assert(0);
					break;
			}
		}
	}

	void State::setDepthTest(DEPTH_TEST test) {
		if (this->configuration.depthTest != test) {
			this->configuration.depthTest = test;
			switch (test) {
				case DEPTH_TEST::ENABLED:
					glEnable(GL_DEPTH_TEST);
					break;
				case DEPTH_TEST::DISABLED:
					glDisable(GL_DEPTH_TEST);
					break;
				default:
					assert(0);
					break;
			}
		}
	}

	void State::setDepthFunc(DEPTH_FUNC func) {
		if (this->configuration.depthFunc != func) {
			this->configuration.depthFunc = func;
			switch (func) {
				case DEPTH_FUNC::LESS:
					glDepthFunc(GL_LESS);
					break;
				default:
					assert(0);
					break;
			}
		}
	}

	void ogs::State::setPolygonMode(POLYGON_MODE mode) {
		if (this->configuration.polygonMode != mode) {
			this->configuration.polygonMode = mode;
			switch (mode) {
				case POLYGON_MODE::FILL:
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					break;
				case POLYGON_MODE::LINE:
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					break;
				case POLYGON_MODE::POINT:
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					break;
				default:
					assert(0);
					break;
			}
		}
	}

	Configuration TextConfiguration() {
		return {
			BLEND::ENABLED,
			BLEND_FUNC::SRC_ALPHA__ONE_MINUS_SRC_ALPHA,
			DEPTH_TEST::DISABLED,
			DEPTH_FUNC::LESS,
			POLYGON_MODE::FILL
		};
	}

	Configuration UIBackground() {
		return {
			BLEND::DISABLED,
			BLEND_FUNC::SRC_ONE__ONE_MINUS_SRC_ALPHA,
			DEPTH_TEST::ENABLED,
			DEPTH_FUNC::LESS,
			POLYGON_MODE::FILL
		};
	}
}
