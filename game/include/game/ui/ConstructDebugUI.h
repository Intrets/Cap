#pragma once

namespace ui
{
	class Base;
	class List;
}

template<class, class>
class WeakReference;

namespace game
{
	WeakReference<ui::Base, ui::List> constructDebugUI();
}