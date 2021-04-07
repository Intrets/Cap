#pragma once

#include <wglm/glm.hpp>

class ScreenRectangle;

class Rect
{
private:
	friend class Rect;
	friend class ScreenRectangle;

	glm::vec2 bot;
	glm::vec2 top;

public:
	Rect() = default;
	Rect(glm::vec2 b, glm::vec2 t);
	Rect(float botx, float boty, float topx, float topy);

	void set(glm::vec2 p1, glm::vec2 p2);
	void set(Rect rec);

	bool equals(Rect& other) const;

	bool contains(glm::vec2 p) const;
	bool contains(Rect& rec) const;

	float getLeft() const;
	float getRight() const;
	float getTop() const;
	float getBot() const;

	glm::vec2 getAbsSize() const;

	glm::vec2 getTopLeft() const;
	glm::vec2 getBottomRight() const;
	glm::vec2 getTopRight() const;
	glm::vec2 getBottomLeft() const;

	void setTopLeft(glm::vec2 p);
	void setBottomRight(glm::vec2 p);
	void setTopRight(glm::vec2 p);
	void setBottomLeft(glm::vec2 p);

	void setTop(float s);
	void setBot(float s);
	void setRight(float s);
	void setLeft(float s);

	void translate(glm::vec2 p);
	void translateLeft(float s);
	void translateRight(float s);
	void translateTop(float s);
	void translateBottom(float s);
	void setHeight(float h);
	void setWidth(float w);
	void setSize(glm::vec2 size);

	float getWidth() const;
	float getHeight() const;
};

class ScreenRectangle
{
private:
	glm::ivec2 screenPixels;

	glm::ivec2 bot;
	glm::ivec2 top;

public:
	ScreenRectangle() = default;
	ScreenRectangle(glm::ivec2 size);
	ScreenRectangle(glm::ivec2 bot_, glm::ivec2 size_, glm::ivec2 screenPixels_);

	glm::ivec2 size() const;
	glm::vec2 sizeScreen() const;

	bool operator==(ScreenRectangle const& other) const;
	bool operator!=(ScreenRectangle const& other) const;

	int32_t getLeft() const;
	int32_t getRight() const;
	int32_t getTop() const;
	int32_t getBot() const;

	glm::ivec2 getTopLeft() const;
	glm::ivec2 getBottomRight() const;
	glm::ivec2 getTopRight() const;
	glm::ivec2 getBottomLeft() const;
	glm::ivec2 getSize() const;

	glm::vec2 getTopLeftScreen() const;
	glm::vec2 getBottomRightScreen() const;
	glm::vec2 getTopRightScreen() const;
	glm::vec2 getBottomLeftScreen() const;

	void setLeft(int32_t i);
	void setRight(int32_t i);
	void setBot(int32_t i);
	void setTop(int32_t i);

	void setTopLeft(glm::ivec2 v);
	void setBottomRight(glm::ivec2 v);
	void setTopRight(glm::ivec2 v);
	void setBottomLeft(glm::ivec2 v);

	int32_t getHeight() const;
	int32_t getWidth() const;

	void setHeight(int32_t i);
	void setWidth(int32_t i);

	void setSize(ScreenRectangle rec);

	void translate(glm::ivec2 v);
	void translateTop(int32_t i);
	void translateBot(int32_t i);
	void translateLeft(int32_t i);
	void translateRight(int32_t i);

	bool contains(glm::ivec2 v) const;

	void setPixelSize(glm::ivec2 px);
	glm::ivec2 getPixelSize() const;
	glm::ivec2 getPixelPos() const;
};

glm::vec2 pixelToScreen(glm::ivec2 vec, glm::ivec2 screenSize);
glm::vec2 pixelToNormal(glm::ivec2 vec, glm::ivec2 screenSize);
glm::ivec2 normalToPixel(glm::vec2 vec, glm::ivec2 screenSize);
glm::ivec2 screenToPixel(glm::vec2 vec, glm::ivec2 screenSize);
glm::vec2 screenToNormal(glm::vec2 vec);
glm::vec2 normalToScreen(glm::vec2 vec);

