#include "Rectangle.h"

#include <wglm/gtc/constants.hpp>

#include "NumHelpers.h"

Rect::Rect(glm::vec2 b, glm::vec2 t) : bot(b), top(t) {
}

Rect::Rect(float botx, float boty, float topx, float topy) : bot(botx, boty), top(topx, topy) {
}

void Rect::set(glm::vec2 p1, glm::vec2 p2) {
	this->bot = glm::min(p1, p2);
	this->top = glm::max(p1, p2);
}

void Rect::set(ScreenRectangle rec) {
	this->bot = rec.bot;
	this->top = rec.top;
}

void Rect::set(Rect rec) {
	this->bot = rec.bot;
	this->top = rec.top;
}

bool Rect::equals(Rect& other) const {
	return norm2(this->bot - other.bot) < glm::epsilon<float>() && norm2(this->top - other.top) < glm::epsilon<float>();
}

bool Rect::equals(ScreenRectangle& other) const {
	return norm2(this->bot - other.bot) < glm::epsilon<float>() && norm2(this->top - other.top) < glm::epsilon<float>();
}

bool Rect::contains(glm::vec2 p) const {
	return (this->bot.x < p.x) && (p.x < this->top.x) && (this->bot.y < p.y) && (p.y < this->top.y);
}

bool Rect::contains(Rect& rec) const {
	return this->contains(rec.bot) && this->contains(rec.top);
}

bool Rect::contains(ScreenRectangle& rec) const {
	return this->contains(rec.bot) && this->contains(rec.top);
}

float Rect::getLeft() const {
	return bot.x;
}

float Rect::getRight() const {
	return top.x;
}

float Rect::getTop() const {
	return top.y;
}

float Rect::getBot() const {
	return bot.y;
}

glm::vec2 Rect::getAbsSize() const {
	return glm::abs(this->bot - this->top);
}

glm::vec2 Rect::getTopLeft() const {
	return glm::vec2(this->bot.x, this->top.y);
}

glm::vec2 Rect::getBottomRight() const {
	return glm::vec2(this->top.x, this->bot.y);
}

glm::vec2 Rect::getTopRight() const {
	return this->top;
}

glm::vec2 Rect::getBottomLeft() const {
	return this->bot;
}

void Rect::setTop(float s) {
	this->top.y = s;
}

void Rect::setBot(float s) {
	this->bot.y = s;
}

void Rect::setRight(float s) {
	this->top.x = s;
}

void Rect::setLeft(float s) {
	this->bot.x = s;
}

void Rect::translate(glm::vec2 p) {
	this->bot += p;
	this->top += p;
}

void Rect::translateLeft(float s) {
	this->bot.x += s;
}

void Rect::translateRight(float s) {
	this->top.x += s;
}

void Rect::translateTop(float s) {
	this->top.y += s;
}

void Rect::translateBottom(float s) {
	this->bot.y += s;
}

void Rect::setHeight(float h) {
	this->bot.y = this->top.y - h;
}

void Rect::setWidth(float w) {
	this->top.x = this->bot.x + w;
}

void Rect::setSize(glm::vec2 size) {
	this->setWidth(size.x);
	this->setHeight(size.y);
}

float Rect::getWidth() const {
	return this->top.x - this->bot.x;
}

float Rect::getHeight() const {
	return this->top.y - this->bot.y;
}

void ScreenRectangle::setPixelSize(glm::ivec2 px) {
	this->screenPixels = px;
}

glm::ivec2 ScreenRectangle::getPixelSize() const {
	return this->screenPixels;
}

glm::ivec2 ScreenRectangle::getPixelPos() const {
	// TODO
	assert(false);
	return glm::ivec2(0);
}

void Rect::setTopLeft(glm::vec2 p) {
	this->bot.x = p.x;
	this->top.y = p.y;
}

void Rect::setBottomRight(glm::vec2 p) {
	this->bot.y = p.y;
	this->top.x = p.x;
}

void Rect::setTopRight(glm::vec2 p) {
	this->top = p;
}

void Rect::setBottomLeft(glm::vec2 p) {
	this->bot = p;
}
