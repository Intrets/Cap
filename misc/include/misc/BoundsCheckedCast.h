#pragma once

#include <numeric>
#include <concepts>
#include <limits>
#include <type_traits>

#define BOUNDS_CHECKED_STATIC_CAST

namespace misc
{
	template<class To, class From>
	requires std::floating_point<To>
	bool checkBounds(From&& from) {
		if constexpr (std::is_same_v<From, double> && std::is_same_v<To, float>) {
			auto min = static_cast<From>(std::numeric_limits<To>::lowest());
			auto max = static_cast<From>(std::numeric_limits<To>::max());
			return min <= from && from <= max;
		}
		else {
			return true;
		}
	}

	template<class To, class From>
	requires std::floating_point<From>
	bool checkBounds(From&& from) {
		auto min = static_cast<From>(std::numeric_limits<To>::lowest());
		auto max = static_cast<From>(std::numeric_limits<To>::max());
		return min <= from && from <= max;
	}

	template<class To, class From>
	requires (std::integral<To>&& std::integral<From>)
	bool checkBounds(From&& from) {
		auto min = static_cast<intmax_t>(std::numeric_limits<To>::lowest());
		auto max = static_cast<intmax_t>(std::numeric_limits<To>::max());
		auto val = static_cast<intmax_t>(from);
		return min <= val && val <= max;
	}

	template<class To, class From>
	requires std::convertible_to<From, To> && (std::integral<From> || std::floating_point<From>)
	To sc(From&& from) {
#ifdef BOUNDS_CHECKED_STATIC_CAST
		assert(checkBounds<To>(std::forward<From>(from)));
#endif // BOUNDS_CHECKED_STATIC_CAST
		return static_cast<To>(std::forward<From>(from));
	};

	template<class To, class From>
	To sc(From&& from) {
		return static_cast<To>(from);
	};
}
