#pragma once

#include <glm/glm.hpp>

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
	void set(ScreenRectangle rec);
	void set(Rect rec);

	bool equals(Rect& other) const;
	bool equals(ScreenRectangle& other) const;

	bool contains(glm::vec2 p) const;
	bool contains(Rect& rec) const;
	bool contains(ScreenRectangle& rec) const;

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

class ScreenRectangle : public Rect
{
private:
	glm::ivec2 screenPixels;

public:
	void setPixelSize(glm::ivec2 px);
	glm::ivec2 getPixelSize() const;
	glm::ivec2 getPixelPos() const;
};