#include "infos/DebugRenderInfo.h"

void render::DebugRenderInfo::Data::addPoint(glm::vec2 p) {
	this->points.points.push_back(p);
}

void render::DebugRenderInfo::Data::addPoint(float x, float y) {
	this->points.points.push_back({ x,y });
}

void render::DebugRenderInfo::Data::addLine(glm::vec2 p1, glm::vec2 p2) {
	this->lines.lines.push_back(p1);
	this->lines.lines.push_back(p2);
}

void render::DebugRenderInfo::Data::addLine(float x1, float y1, float x2, float y2) {
	this->lines.lines.push_back({ x1, y1 });
	this->lines.lines.push_back({ x2, y2 });
}

void render::DebugRenderInfo::Data::addLine(glm::vec2 p1, float x2, float y2) {
	this->lines.lines.push_back(p1);
	this->lines.lines.push_back({ x2, y2 });
}

void render::DebugRenderInfo::Data::addLine(float x1, float y1, glm::vec2 p2) {
	this->lines.lines.push_back({ x1, y1 });
	this->lines.lines.push_back(p2);
}

void render::DebugRenderInfo::Data::addBox(glm::vec2 u, glm::vec2 d) {
	this->addLine(u, u.x, d.y);
	this->addLine(u, d.x, u.y);
	this->addLine(d, u.x, d.y);
	this->addLine(d, d.x, u.y);
}

void render::DebugRenderInfo::Data::addBox(float x1, float y1, float x2, float y2) {
	this->addBox({ x1,y1 }, { x2,y2 });
}

void render::DebugRenderInfo::Data::addBox(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4) {
	this->addLine(p1, p2);
	this->addLine(p2, p3);
	this->addLine(p3, p4);
	this->addLine(p4, p1);
}

void render::DebugRenderInfo::clear() {
	this->world = {};
	this->screen = {};
}
