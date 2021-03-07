#pragma once

#include <string>

namespace ui
{
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
}

class UIOConstrainSize;
class UIOPad;
class UIOList;
class UIOGrid;

template<>
constexpr ui::TYPE ui::GET_TYPE<UIOConstrainSize>() {
	return ui::TYPE::CONSTRAIN_SIZE;
}

template<>
constexpr ui::TYPE ui::GET_TYPE<UIOPad>() {
	return ui::TYPE::PAD;
}

template<>
constexpr ui::TYPE ui::GET_TYPE<UIOList>() {
	return ui::TYPE::LIST;
}

template<>
constexpr ui::TYPE ui::GET_TYPE<UIOGrid>() {
	return ui::TYPE::GRID;
}

template<class T>
constexpr ui::TYPE ui::GET_TYPE() {
	return ui::TYPE::UNSPECIFIED;
}

