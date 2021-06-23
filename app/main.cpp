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
using prepent_t = prepend<P, Tuple>::value;


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
	using R = prepent_t<typename Next1::R1, typename Next2::R>;
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
	using R = prepent_t<typename Next1::R1, typename Next2::R>;
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


using Z = std::tuple<Map<int>, Map<int>, int, int, Map<int>, Map<float>>;

using Test = Take<Z, is_map>;
using Test2 = GroupBy<Z, is_map>;



int main(int argc, char* argv[]) {
	[[maybe_unused]]
	Test::R1 t1;
	Test::R2 t2;

	Test2::R t;

	rand();

	auto add1 = [](auto x) {
		return x + 1;
	};

	auto mult2 = [](auto x) {
		return x * 2;
	};

	auto even = [](auto i) {
		return i % 2 == 0;
	};

	auto e = Filter(even);

	auto bv = AThing(Filter(even), Map(add1));


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
