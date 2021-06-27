#include "include/rangles/rangles.h"

#include <tuple>


int main() {
	ra2::test();
	return 1;
}

int main_old() {
	[[maybe_unused]]
	auto f = std::make_tuple(
		ra::Map{ [](auto i) {return i + 1; } },
		ra::Filter{ [](auto i) {return i % 2 == 0; } }
	);

	// ra::Producer({1,2,3,4,5});

	auto f1 = ra::Map{ [](auto i) {return i + 1; } };
	auto f2 = ra::Filter{ [](auto i) {return i % 2 == 0; } };

	[[maybe_unused]]
	auto test1 = ra::Whole<int, decltype(f1), decltype(f2)>().run(2, f1, f2);
	[[maybe_unused]]
	auto test2 = ra::Whole<int, decltype(f1), decltype(f2)>().run(3, f1, f2);

	rand();
	return 1;
}