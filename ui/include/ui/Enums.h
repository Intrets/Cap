#pragma once

#include <string>

namespace ui
{
	namespace CURSOR
	{
		enum class TYPE
		{
			BLOCK,
			LINE,
		};
	}

	enum SIZETYPE
	{
		FH,
		PX,
		STATIC_PX,
		ABSOLUTE_HEIGHT,
		ABSOLUTE_WIDTH,
		RELATIVE_HEIGHT,
		RELATIVE_WIDTH,
	};

	namespace TEXTDISPLAY
	{
		enum class MODE
		{
			INSERT,
			NORMAL,
			VISUAL,
		};
	}

	namespace PAD
	{
		enum class TYPE
		{
			SHRINK,
			FREE,
			SIZETYPE,
		};
	}

	namespace WINDOW
	{
		enum TYPE
		{
			MINIMISE = 1 << 0,
			CLOSE = 1 << 1,
			RESIZE = 1 << 2,
			RESIZEHORIZONTAL = 1 << 3,
			RESIZEVERTICAL = 1 << 4,
			MOVE = 1 << 5,
			HIDE = 1 << 6,
		};
	}

	enum class TYPE
	{
		CONSTRAIN_SIZE,
		PAD,
		LIST,
		GRID,
		UNSPECIFIED,
	};

	template<class T>
	constexpr TYPE GET_TYPE();

	std::string GET_NAME(TYPE type);

	enum class ALIGNMENT
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		TOPLEFT,
		TOPRIGHT,
		BOTTOMLEFT,
		BOTTOMRIGHT,
		CENTER,
	};

	enum class DIR
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		LEFT_REVERSE,
		RIGHT_REVERSE,
		UP_REVERSE,
		DOWN_REVERSE,
	};

	class ConstrainSize;
	class Pad;
	class List;
	class Grid;

	template<>
	constexpr TYPE GET_TYPE<ConstrainSize>() {
		return TYPE::CONSTRAIN_SIZE;
	}

	template<>
	constexpr TYPE GET_TYPE<Pad>() {
		return TYPE::PAD;
	}

	template<>
	constexpr TYPE GET_TYPE<List>() {
		return TYPE::LIST;
	}

	template<>
	constexpr TYPE GET_TYPE<Grid>() {
		return TYPE::GRID;
	}

	template<class T>
	constexpr TYPE GET_TYPE() {
		return TYPE::UNSPECIFIED;
	}
}
