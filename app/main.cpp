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
#include <algorithm>
#include <functional>
#include <numeric>

#include <tepp/tepp.h>
#include <ranges>

// TODO: keep runtime option, get value from config/command line argument
bool OPENGL_DEBUG = true;

GLFWwindow* window;

#include <string_view>

using namespace std::string_view_literals;

//template<template<class> class, class Tuple>
//struct all;

template<class, class>
struct all;

template<template<class> class T>
struct wrapped_function {};

template<template<class> class P, class... Args>
struct all<wrapped_function<P>, std::tuple<Args...>>
{
	static constexpr bool value = (P<Args>::value && ...);
};

template<class P, class Tuple>
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
using prepend_t = typename prepend<P, Tuple>::value;


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
	struct t : std::false_type {};

	template<class R>
	requires std::is_same_v<typename R::type, T>
		struct t<R> : std::true_type {};
};

template<class T>
using is_ = wrapped_function<is<T>::template t>;

struct map_type {};

struct filter_type {};

template<class T, class = void>
struct is_map : std::false_type {};

template<class T>
struct is_map<T, std::void_t<typename T::type>>
{
	static constexpr bool value = std::is_same_v<map_type, T::type>;
};

template<class T, class = void>
struct is_filter : std::false_type {};

template<class T>
struct is_filter<T, std::void_t<typename T::type>>
{
	static constexpr bool value = std::is_same_v<filter_type, T::type>;
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

template<class F>
struct Filter
{
	using type = filter_type;

	F f;

	constexpr bool operator()(auto x) const {
		return this->f(x);
	}
};

template<class F>
Filter(F const&)->Filter<F>;

template<class F>
struct Fold1
{
	using type = filter_type;

	F f;

	constexpr bool operator()(auto x) const {
		return this->f(x);
	}
};

template<class F>
Fold1(F const&)->Fold1<F>;

template<class N, class M>
using add = std::integral_constant<typename N::value_type, N::value + M::value>;

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
	using value = typename ShiftSequence<Start, std::make_integer_sequence<int, Length::value>>::value;
};

template<class Is>
struct head_is;

template<int i, int... is>
struct head_is<std::integer_sequence<int, i, is...>>
{
	static constexpr int value = i;
};

template<class Is>
static constexpr int head_is_v = head_is<Is>::value;


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
using Size_t = typename Size<T>::value;

template<class N, class Tuple>
struct Counts;

template<class N, class Arg>
struct Counts<N, std::tuple<Arg>>
{
	using c = Size_t<Arg>;
	using C = typename Sequence<N, c>::value;
	using R = std::tuple<C>;
};

template<class N, class Arg, class... Args>
struct Counts<N, std::tuple<Arg, Args...>>
{
	using c = Size_t<Arg>;
	using C = typename Sequence<N, c>::value;
	using Next = Counts<add<N, c>, std::tuple<Args...>>;
	using R = prepend_t<C, typename Next::R>;
};


template<class A, class... Args>
struct Run3;

template<class A, class... Args>
requires all_v<is_<map_type>, std::tuple<Args...>>
struct Run3<A, Args...>
{
	constexpr static auto apply2(A x, Args const&... maps) {
		return (x >> ... >> maps);
	};
};

template<class A, class... Args>
requires all_v<is_<filter_type>, std::tuple<Args...>>
struct Run3<A, Args...>
{
	constexpr static bool apply2(A x, Args const&... filters) {
		return (filters(x) && ...);
	};
};

template<int I, class Tuple>
struct get_t
{
	using type = std::remove_cvref_t<decltype(std::get<I>(std::declval<Tuple>()))>;
};

template<class A, class Tuple, class Is>
struct Run;

template<class A, class Tuple, int... Is>
struct Run<A, Tuple, std::integer_sequence<int, Is...>>
{
	constexpr static auto apply(A x, Tuple const& tuple) {
		return Run3<A, typename get_t<Is, Tuple>::type...>::apply2(x, std::get<Is>(tuple)...);
	}
};

template<class A, class Tuple, class... Iss>
struct Whole3;

template<class A, class Tuple>
struct Whole3<A, Tuple>
{
	constexpr static std::optional<A> run(A&& x, Tuple const& tuple) {
		return std::optional(std::forward<A>(x));
	}
};

template<class A, class Tuple, class Is, class... Iss>
struct Whole3<A, Tuple, Is, Iss...>
{
	using t_type = get_t<head_is_v<Is>, Tuple>::type::type;

	using a = std::conditional_t<std::same_as<t_type, map_type>
		, te::return_type_t<decltype(&Run<A, Tuple, Is>::apply)>
		, A
	>;
	using R = te::return_type_t<decltype(&Whole3<a, Tuple, Iss...>::run)>::value_type;

	constexpr static std::optional<R> run(A&& x, Tuple const& tuple) {
		if constexpr (std::same_as<t_type, map_type>) {
			auto y = Run<A, Tuple, Is>::apply(std::forward<A>(x), tuple);
			return Whole3<decltype(y), Tuple, Iss...>::run(std::forward<decltype(y)>(y), tuple);
		}
		else if constexpr (std::same_as<t_type, filter_type>) {
			if (Run<A, Tuple, Is>::apply(x, tuple)) {
				return Whole3<A, Tuple, Iss...>::run(std::forward<A>(x), tuple);
			}
			else {
				return std::nullopt;
			}
		}
		else {
			return std::nullopt;
		}
	}
};

template<class A, class Tuple, class Is>
struct Whole4_;

template<class A, class Tuple, class... Iss>
struct Whole4_<A, Tuple, std::tuple<Iss...>>
{
	using type = Whole3<A, Tuple, Iss...>;
};

template<class A, class Tuple, class Is>
using Whole4 = typename Whole4_<A, Tuple, Is>::type;

template<class T>
using get_type = typename T::type;


template<class Tuple>
using head = get_t<0, Tuple>;

template<class To, class From, class F>
std::vector<To> run(std::vector<From> const& from, F fff) {
	std::vector<To> res;
	//res.reserve(100000000);

	//using FF = GroupBy<F, is_map>;
	//using GG = GroupBy<typename FF::R, is_filter>::R;
	//using C = Counts<std::integral_constant<int, 0>, GG>::R;

	using GG = te::map<std::tuple, F>::type;
	using C = Counts<std::integral_constant<int, 0>, GG>::R;

	for (auto const& f : from) {
		auto r = Whole4<decltype(f), F, C>::run(f, fff);
		if (r.has_value()) {
			res.push_back(r.value());
		}

	}

	return res;
};


using Z = std::tuple<Map<int>, Map<int>, int, int, Map<int>, Map<float>>;

//using Test = Take<Z, is_map>;
//using Test2 = GroupBy<Z, is_map>;

/*


		        |-- 'c' --|
		        |-- 'b' --|
start "a b c"---|-- 'a' --|-----"abc"
		     split(' ')  join




*/

int main(int argc, char* argv[]) {
	//[[maybe_unused]]
	//Test::R1 t1;
	//Test::R2 t2;

	//Test2::R t;

	//using RangeTest = FromTo<std::integral_constant<int, 2>, std::integral_constant<int, 3>>::R;
	//[[maybe_unused]]
	//RangeTest rangeTest;

	//using SequenceTest = Sequence<std::integral_constant<int, 3>, std::integral_constant<int, 5>>::value;
	//using CountsTest = Counts<std::integral_constant<int, 0>, Test2::R>::R;

	//[[maybe_unused]]
	//SequenceTest aaa;
	//CountsTest aaa2;

	auto add1 = [](auto x) {
		return x + 1;
	};

	auto mult2 = [](auto x) {
		return x * 2;
	};

	auto even = [](auto x) {
		return x % 2 == 0;
	};

	auto f = std::make_tuple(Map{ add1 }, Map{ mult2 }, Filter{ even }, Filter{ even }, Map{ add1 });
	//using FF = GroupBy<decltype(f), is_map>;
	//using GG = GroupBy<FF::R, is_filter>::R;
	//[[maybe_unused]]
	//FF ff;
	//[[maybe_unused]]
	//GG gg;

	constexpr auto test = (2 >> Map{ add1 }) >> Map{ mult2 };

	auto toDouble = [](int i) -> double {
		return i;
	};


	using TTT = get_t<1, std::tuple<int, float>>::type;
	[[maybe_unused]]
	TTT ttt;


	//[[maybe_unused]]
	//constexpr auto test2 = Run<
	//	int,
	//	decltype(g),
	//	std::integer_sequence<int, 1, 1, 1, 1, 1>
	//>::apply(2, g);



	constexpr auto f0 = Map{ [](auto x) { return x + 3; } };
	constexpr auto f1 = Map{ add1 };
	constexpr auto f2 = Map{ mult2 };
	constexpr auto f3 = Filter{ even };
	constexpr auto f4 = Map{ toDouble };
	constexpr auto f5 = Filter{ [](auto x) { return x % 3 == 0; } };

	constexpr auto g = std::make_tuple(f0, f1, f2, f1, f2, f1, f5, f4);
	using namespace std::ranges::views;




	std::vector<int> vec;
	vec.resize(100000000);
	std::iota(vec.begin(), vec.end(), 1);


	{
		constexpr auto vie = transform([](auto x) { return x + 3; }) | transform(add1) | transform(mult2) | transform(add1) | transform(mult2) | transform(add1) | filter([](auto x) { return x % 3 == 0; }) | transform(toDouble);
		auto vi = vec | vie;

		auto start1 = std::chrono::system_clock::now();
		std::vector<double> res3(vi.begin(), vi.end());
		std::chrono::duration<double> duration1 = std::chrono::system_clock::now() - start1;
		std::cout << "ranges: " << duration1 << "\n";

		auto start2 = std::chrono::system_clock::now();
		auto res2 = run<double>(vec, g);
		std::chrono::duration<double> duration2 = std::chrono::system_clock::now() - start2;
		std::cout << "weird: " << duration2 << "\n";

		std::cout << "same: " << (res3 == res2) << "\n";
		std::cout << "size ranges: " << res3.size() << "\n";
		std::cout << "size weird: " << res2.size() << "\n";
	}


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
	return 0;
}
