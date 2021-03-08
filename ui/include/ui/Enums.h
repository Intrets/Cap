#pragma once

#include <string>

namespace ui
{
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
