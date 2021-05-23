#include "infos/DebugRenderInfo.h"

void render::DebugRenderInfo::Data::addPoint(glm::vec2 p, colors::Color color) {
	this->points.points.push_back({ p, color });
}

void render::DebugRenderInfo::Data::addPoint(float x, float y, colors::Color color) {
	this->points.points.push_back({ { x, y }, color });
}

void render::DebugRenderInfo::Data::addLine(glm::vec2 p1, glm::vec2 p2, colors::Color color) {
	this->lines.lines.push_back({ p1, color });
	this->lines.lines.push_back({ p2, color });
}

void render::DebugRenderInfo::Data::addLine(float x1, float y1, float x2, float y2, colors::Color color) {
	this->addLine({ x1, y1 }, { x2, y2 }, color);
}

void render::DebugRenderInfo::Data::addLine(glm::vec2 p1, float x2, float y2, colors::Color color) {
	this->addLine(p1, { x2, y2 }, color);
}

void render::DebugRenderInfo::Data::addLine(float x1, float y1, glm::vec2 p2, colors::Color color) {
	this->addLine({ x1, y1 }, p2, color);
}

void render::DebugRenderInfo::Data::addBox(glm::vec2 u, glm::vec2 d, colors::Color color) {
	this->addLine(u, u.x, d.y, color);
	this->addLine(u, d.x, u.y, color);
	this->addLine(d, u.x, d.y, color);
	this->addLine(d, d.x, u.y, color);
}

void render::DebugRenderInfo::Data::addBox(float x1, float y1, float x2, float y2, colors::Color color) {
	this->addBox({ x1,y1 }, { x2,y2 }, color);
}

void render::DebugRenderInfo::Data::addBox(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, colors::Color color) {
	this->addLine(p1, p2, color);
	this->addLine(p2, p3, color);
	this->addLine(p3, p4, color);
	this->addLine(p4, p1, color);
}

void render::DebugRenderInfo::clear() {
	this->world = {};
	this->screen = {};
}
