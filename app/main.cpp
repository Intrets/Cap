#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <chrono>

#include "MainLoop.h"
#include "Setup.h"

#include <sstream>
#include <ranges>
#include <concepts>
#include <functional>

#include <tepp/tepp.h>

// TODO: keep runtime option, get value from config/command line argument
bool OPENGL_DEBUG = true;

GLFWwindow* window;

#include <string_view>

using namespace std::string_view_literals;

template<template<class> class, class Tuple>
struct all;

template<template<class> class P, class... Args>
struct all<P, std::tuple<Args...>>
{
	static constexpr bool value = (P<Args>::value && ...);
};

template<template<class> class P, class Tuple>
constexpr bool all_v = all<P, Tuple>::value;


template<class P, class Tuple>
struct prepend;

template<class P>
struct prepend<P, void>
{
	using value = std::tuple<P>;
};

template<class Tuple>
struct prepend<void, Tuple>
{
	using value = Tuple;
};

template<class P, class... Args>
struct prepend<P, std::tuple<Args...>>
{
	using value = std::tuple<P, Args...>;
};

template<class P, class Tuple>
using prepend_t = prepend<P, Tuple>::value;


template<class Tuple, template<class> class T>
struct GroupBy2;

template<class Tuple, template<class> class T>
struct GroupBy;

template<class Tuple, template<class> class T>
struct Take;


template<template<class> class T, class Arg>
requires T<Arg>::value
struct Take<std::tuple<Arg>, T>
{
	using R1 = std::tuple<Arg>;
	using R2 = void;
};

template<template<class> class T, class Arg>
struct Take<std::tuple<Arg>, T>
{
	using R1 = void;
	using R2 = std::tuple<Arg>;
};


template<template<class> class T, class Arg, class... Args>
requires T<Arg>::value
struct Take<std::tuple<Arg, Args...>, T>
{
	using Next = Take<std::tuple<Args...>, T>;
	using R1 = typename prepend<Arg, typename Next::R1>::value;
	using R2 = typename Next::R2;
};

template<template<class> class T, class... Args>
struct Take<std::tuple<Args...>, T>
{
	using R1 = void;
	using R2 = std::tuple<Args...>;
};

template<class Tuple>
struct TakeOne;

template<class Arg>
struct TakeOne<std::tuple<Arg>>
{
	using R1 = Arg;
	using R2 = void;
};

template<class Arg, class... Args>
struct TakeOne<std::tuple<Arg, Args...>>
{
	using R1 = Arg;
	using R2 = std::tuple<Args...>;
};

template<template<class> class T>
struct GroupBy<void, T>
{
	using R = void;
};

template<template<class> class T, class Arg>
struct GroupBy<std::tuple<Arg>, T>
{
	using R = std::tuple<Arg>;
};

template<template<class> class T, class... Args>
struct GroupBy<std::tuple<Args...>, T>
{
	using Next1 = Take<std::tuple<Args...>, T>;
	using Next2 = GroupBy2<typename Next1::R2, T>;
	using R = prepend_t<typename Next1::R1, typename Next2::R>;
};


template<template<class> class T>
struct GroupBy2<void, T>
{
	using R = void;
};

template<template<class> class T, class Arg>
struct GroupBy2<std::tuple<Arg>, T>
{
	using R = std::tuple<Arg>;
};

template<template<class> class T, class... Args>
struct GroupBy2<std::tuple<Args...>, T>
{
	using Next1 = TakeOne<std::tuple<Args...>>;
	using Next2 = GroupBy<typename Next1::R2, T>;
	using R = prepend_t<typename Next1::R1, typename Next2::R>;
};

template<class... Args>
struct AThing
{
	std::tuple<Args...> stuff;

	AThing(Args&&... args) : stuff(std::forward<Args>(args)...) {};

	template<class T, std::forward_iterator It>
	std::vector<T> run(It start, It end) {

	};
};

template<class... Args>
AThing(Args&&... args)->AThing<Args...>;

template<class T>
struct is
{
	template<class>
	struct is_type : std::false_type {};

	template<class R>
	requires std::is_same_v<typename R::type, T>
	struct is_type<R> : std::true_type {};

	template<class R>
	static constexpr auto is_type_t = is_type<R>;
};

template<class T>
constexpr auto is_t = is<T>::is_type;

struct map_type {};

template<class T, class = void>
struct is_map : std::false_type {};

template<class T>
struct is_map<T, std::void_t<typename T::type>>
{
	static constexpr bool value = std::is_same_v<map_type, T::type>;
};

template<class F>
struct Map
{
	using type = map_type;

	F f;

	friend constexpr auto operator>>(auto x, Map<F>const& f) {
		return f.f(x);
	}
};

template<class F>
Map(F const&)->Map<F>;

struct filter_type {};

template<class F>
struct Filter
{
	using type = filter_type;

	F f;
};

template<class F>
Filter(F const&)->Filter<F>;

template<class N, class M>
using add = std::integral_constant<typename N::value_type, N::value + M::value>;
template<class N, class M>
constexpr auto add_v = sub<N, M>::value;

template<class N, class M>
using sub = std::integral_constant<typename N::value_type, N::value - M::value>;
template<class N, class M>
constexpr auto sub_v = sub<N, M>::value;

template<class N, class M>
using comp = std::integral_constant<bool, N::value < M::value>;
template<class N, class M>
constexpr auto comp_v = comp<N, M>::value;

template<class N, class M, class = void>
struct FromTo
{
	using R = void;
};

template<class N, class M>
struct FromTo<N, M, std::enable_if_t<comp_v<N, M>>>
{
	using Next = FromTo<std::integral_constant<int, N::value + 1>, M>;
	using R = prepend_t<N, typename Next::R>;
};

template<class N, class Sequence>
struct ShiftSequence;

template<class N, int... Ints>
struct ShiftSequence<N, std::integer_sequence<int, Ints...>>
{
	using value = std::integer_sequence<int, (Ints + N::value)...>;
};

template<class Start, class Length>
struct Sequence
{
	using value = ShiftSequence<Start, std::make_integer_sequence<int, Length::value>>::value;
};


template<class T>
struct Size;

template<class... Args>
struct Size<std::tuple<Args...>>
{
	using value = std::integral_constant<int, sizeof...(Args)>;
};

template<class T>
struct Size
{
	using value = std::integral_constant<int, 1>;
};

template<class T>
using Size_t = Size<T>::value;

template<class N, class Tuple>
struct Counts;

template<class N, class Arg>
struct Counts<N, std::tuple<Arg>>
{
	using c = Size_t<Arg>;
	using C = Sequence<N, c>::value;
	using R = std::tuple<C>;
};

template<class N, class Arg, class... Args>
struct Counts<N, std::tuple<Arg, Args...>>
{
	using c = Size_t<Arg>;
	using C = Sequence<N, c>::value;
	using Next = Counts<add<N, c>, std::tuple<Args...>>;
	using R = prepend_t<C, typename Next::R>;
};


//template<class A, class... Args>
//struct Run<A, std::tuple<Args...>, std::enable_if_t<all<is_map, std::tuple<Args...>>::value>>
struct Run2
{
	//template<class A, class Tuple, class Is>
	//constexpr static auto apply(A x, Tuple tuple);

	//template<class A, class Tuple, class Is...>
	//constexpr static auto apply<A, Tuple, std::integer_sequence<int, Is...>>(A x, Tuple tuple) {
	//	return 1;
	//}

	template<class A, class... Args, class = std::enable_if_t<all_v<is<map_type>::is_type, std::tuple<Args...>>>>
	constexpr static auto apply2(A x, Args const&... maps) {
		return (x >> ... >> maps);
	};

	//template<class A, class... Args, class = std::enable_if_t<true>>
	//constexpr static auto apply2(A x, Args const&... maps) {
	//	return (x >> ... >> maps);
	//};
};

template<class A, class... Args>
struct Run3;

template<class A, class... Args>
requires all_v<is_map, std::tuple<Args...>>
struct Run3<A, Args...>
{
	constexpr static auto apply2(A x, Args const&... maps) {
		return (x >> ... >> maps);
	};
};

template<class A, class... Args>
//requires all_v<is_filter, std::tuple<Args...>>
struct Run3
{

};


template<class A, class Tuple, class Is>
struct Run;

template<class A, class Tuple, int... Is>
struct Run<A, Tuple, std::integer_sequence<int, Is...>>
{
	constexpr static auto apply(A x, Tuple const& tuple) {
		return Run2::apply2(x, std::get<Is>(tuple)...);
	}
};


using Z = std::tuple<Map<int>, Map<int>, int, int, Map<int>, Map<float>>;

using Test = Take<Z, is_map>;
using Test2 = GroupBy<Z, is_map>;

int main(int argc, char* argv[]) {
	[[maybe_unused]]
	Test::R1 t1;
	Test::R2 t2;

	Test2::R t;

	using RangeTest = FromTo<std::integral_constant<int, 2>, std::integral_constant<int, 3>>::R;
	[[maybe_unused]]
	RangeTest rangeTest;

	using SequenceTest = Sequence<std::integral_constant<int, 3>, std::integral_constant<int, 5>>::value;
	using CountsTest = Counts<std::integral_constant<int, 0>, Test2::R>::R;
	//using RangesTest =

	[[maybe_unused]]
	SequenceTest aaa;
	CountsTest aaa2;

	auto add1 = [](auto x) {
		return x + 1;
	};

	auto mult2 = [](auto x) {
		return x * 2;
	};

	auto f = std::make_tuple(Map(add1), Map(mult2), Filter(1), Map(add1));
	using FF = GroupBy<decltype(f), is_map>;
	[[maybe_unused]]
	FF ff;

	constexpr auto test = (2 >> Map(add1)) >> Map(mult2);

	constexpr auto f1 = Map(add1);
	constexpr auto f2 = Map(mult2);

	constexpr auto g = std::make_tuple(f1, f2);

	[[maybe_unused]]
	constexpr auto test2 = Run<
		int,
		decltype(g),
		std::integer_sequence<int, 1, 1, 1, 1, 1>>::apply(2, g);

	rand();


	//auto even = [](auto i) {
	//	return i % 2 == 0;
	//};

	//auto e = Filter(even);

	//auto bv = AThing(Filter(even), Map(add1));


	rand();

	if (rand() > 0.0000001) {
		exit(0);
	}

	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	std::filesystem::path resourcesPath;
	if (argc > 1) {
		resourcesPath = std::filesystem::path(argv[1]);
	}
	else {
		std::cout << "No custom path specified, walking up current dir and looking for a resources folder.\n";
		std::cout << "Can take a single nameless argument to set resources folder.\n";

		resourcesPath = std::filesystem::current_path();
		auto rootPath = resourcesPath.root_path();

		// Limit the search in case something goes wrong somehow
		constexpr size_t max_search = 100;
		size_t i = 0;
		for (; i < max_search; i++) {
			if (std::filesystem::exists(resourcesPath / "resources")) {
				resourcesPath /= "resources";
				std::cout << "Found Resources path.\n";
				break;
			}

			resourcesPath = resourcesPath.parent_path();
		}
		if (i == max_search) {
			std::cerr << "Fatal error: could not find Resources folder.";
			return 0;
		}
	}

	std::cout << "Resources path: " << std::filesystem::absolute(resourcesPath) << "\n";

	if (!std::filesystem::exists(resourcesPath)) {
		std::cerr << "Fatal error: resources path does not specify an existing folder.\n";
		return 0;
	}

	if (!initGLFW(window, OPENGL_DEBUG)) {
		std::cerr << "Fatal error: initGLFW failed.\n";
		glfwTerminate();
		return 0;
	}

	initManagers(std::filesystem::absolute(resourcesPath).string());

	mainLoop(window, startTime);

	glfwTerminate();
}
