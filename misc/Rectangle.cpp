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

void Rect::set(Rect rec) {
	this->bot = rec.bot;
	this->top = rec.top;
}

bool Rect::equals(Rect& other) const {
	return norm2(this->bot - other.bot) < glm::epsilon<float>() && norm2(this->top - other.top) < glm::epsilon<float>();
}

bool Rect::contains(glm::vec2 p) const {
	return (this->bot.x < p.x) && (p.x < this->top.x) && (this->bot.y < p.y) && (p.y < this->top.y);
}

bool Rect::contains(Rect& rec) const {
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

ScreenRectangle::ScreenRectangle(glm::ivec2 size) :
	bot(0, 0),
	top(size),
	screenPixels(size) {
}

glm::ivec2 ScreenRectangle::size() const {
	return this->top - this->bot;
}

bool ScreenRectangle::operator==(ScreenRectangle const& other) const {
	return this->bot == other.bot && this->top == other.top;
}

bool ScreenRectangle::operator!=(ScreenRectangle const& other) const {
	return !(*this == other);
}

int32_t ScreenRectangle::getLeft() const {
	return this->bot.x;
}

int32_t ScreenRectangle::getRight() const {
	return this->top.x;
}

int32_t ScreenRectangle::getTop() const {
	return this->top.y;
}

int32_t ScreenRectangle::getBot() const {
	return this->bot.y;
}

glm::ivec2 ScreenRectangle::getTopLeft() const {
	return glm::ivec2(this->getLeft(), this->getTop());
}

glm::ivec2 ScreenRectangle::getBottomRight() const {
	return glm::ivec2(this->getRight(), this->getBot());
}

glm::ivec2 ScreenRectangle::getTopRight() const {
	return this->top;
}

glm::ivec2 ScreenRectangle::getBottomLeft() const {
	return this->bot;
}

glm::ivec2 ScreenRectangle::getSize() const {
	return this->top - this->bot;
}

glm::vec2 ScreenRectangle::getTopLeftScreen() const {
	return glm::vec2(this->getTopLeft()) / glm::vec2(this->screenPixels);
}

glm::vec2 ScreenRectangle::getBottomRightScreen() const {
	return glm::vec2(this->getBottomRight()) / glm::vec2(this->screenPixels);
}

glm::vec2 ScreenRectangle::getTopRightScreen() const {
	return glm::vec2(this->getTopRight()) / glm::vec2(this->screenPixels);
}

glm::vec2 ScreenRectangle::getBottomLeftScreen() const {
	return glm::vec2(this->getBottomLeft()) / glm::vec2(this->screenPixels);
}

void ScreenRectangle::setLeft(int32_t i) {
	this->bot.x = i;
}

void ScreenRectangle::setRight(int32_t i) {
	this->top.x = i;
}

void ScreenRectangle::setBot(int32_t i) {
	this->bot.y = i;
}

void ScreenRectangle::setTop(int32_t i) {
	this->top.y = i;
}

void ScreenRectangle::setTopLeft(glm::ivec2 v) {
	this->setLeft(v.x);
	this->setTop(v.y);
}

void ScreenRectangle::setBottomRight(glm::ivec2 v) {
	this->setRight(v.x);
	this->setBot(v.y);
}

void ScreenRectangle::setTopRight(glm::ivec2 v) {
	this->top = v;
}

void ScreenRectangle::setBottomLeft(glm::ivec2 v) {
	this->bot = v;
}

int32_t ScreenRectangle::getHeight() const {
	return this->top.y - this->bot.y;
}

int32_t ScreenRectangle::getWidth() const {
	return this->top.x - this->bot.x;
}

void ScreenRectangle::setHeight(int32_t i) {
	this->bot.y = this->top.y - i;
}

void ScreenRectangle::setWidth(int32_t i) {
	this->top.x = this->bot.x + i;
}

void ScreenRectangle::setSize(ScreenRectangle rec) {
	this->bot = rec.bot;
	this->top = rec.top;
}

void ScreenRectangle::translate(glm::ivec2 v) {
	this->bot += v;
	this->top += v;
}

void ScreenRectangle::translateTop(int32_t i) {
	this->top.y += i;
}

void ScreenRectangle::translateBot(int32_t i) {
	this->bot.y += i;
}

void ScreenRectangle::translateLeft(int32_t i) {
	this->bot.x += i;
}

void ScreenRectangle::translateRight(int32_t i) {
	this->top.x += i;
}

bool ScreenRectangle::contains(glm::ivec2 p) const {
	return (this->bot.x <= p.x) && (p.x <= this->top.x) && (this->bot.y <= p.y) && (p.y <= this->top.y);
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

glm::vec2 pixelToScreen(glm::ivec2 vec, glm::ivec2 screenSize) {
	return pixelToNormal(vec, screenSize) * 2.0f - 1.0f;
}

glm::vec2 pixelToNormal(glm::ivec2 vec, glm::ivec2 screenSize) {
	return glm::vec2(vec) / glm::vec2(screenSize);
}

glm::ivec2 normalToPixel(glm::vec2 vec, glm::ivec2 screenSize) {
	return glm::ivec2(glm::round(vec * glm::vec2(screenSize)));
}

glm::ivec2 screenToPixel(glm::vec2 vec, glm::ivec2 screenSize) {
	return normalToPixel(screenToNormal(vec), screenSize);
}

glm::vec2 screenToNormal(glm::vec2 vec) {
	return (vec / 2.0f) + 0.5f;
}

glm::vec2 normalToScreen(glm::vec2 vec) {
	return (vec * 2.0f) - 1.0f;
}
